
#include <algorithm>
#include <rlib/stdio.hpp>
#include <rlib/opt.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cassert>
using std::string;
using namespace std::literals;

// argument should be valid.
inline char basic_int_to_char(uint8_t i) {
    constexpr auto ls = "xA234567890JQK";
    return ls[i];
}
inline uint8_t basic_char_to_int(char c) {
    constexpr auto ls {"xA234567890JQK"sv};
    return ls.find(toupper(c));
}

// This function is only efficent on small string.
// With length < 15.
// length=16, time=67ms
// length=15, time=13ms
// length=14, time<6ms
// length<14, time<6ms
auto end2end_changpu_solve_short(string input) {
    assert(input.size() <= 64); // we're using uint64 to represent combination.

    std::for_each(input.begin(), input.end(), [](auto &c) {
        c = basic_char_to_int(c);
    });
    // Now input is a number array.

    // map sum_val to [combinations...]
    std::unordered_map<uint64_t, std::vector<uint64_t>> sum_val_map;
    std::vector<uint64_t> comb_sum_val_cache(1<<input.size(), 0);
    auto insert_pair_to_sum_val_map = [&sum_val_map](uint64_t k, uint64_t v) {
        auto iter = sum_val_map.find(k);
        if(iter == sum_val_map.end()) {
            sum_val_map.try_emplace(k, 1, v);
        }
        else {
            iter->second.emplace_back(v);
        }
    };

    // iterate all combinations, skip all-zero and all-one, because it's not useful.
    // lowest-bit means input[0], highest-bit means input[-1].
    for(uint64_t comb = 1; comb < 1<<input.size(); ++comb) {
        if((comb & 1) == 1) {
            // For odd number, no need to fill cache.
            const auto sum_val = comb_sum_val_cache[comb-1] + input[0];
            insert_pair_to_sum_val_map(sum_val, comb);
        }
        else {
            // For even number, just remove any true bit, and lookup cache.
            // Recommend to remove highest true bit, to make use of L3 cache.
            const auto leading_zeros = __builtin_clzll(comb);
            const auto highest_significant_bit_input_index = 63 - leading_zeros;

            // 00100110 ==> sum_val(00100000) + sum_val(00000110)
            const auto cache_slot_left = 1ull << (highest_significant_bit_input_index);
            const auto cache_slot_right = comb ^ cache_slot_left;
            const uint64_t sum_val = cache_slot_right == 0
                // In this case, cache_slot_left is not cached at all. We read and cache it.
                ? input[highest_significant_bit_input_index]
                // In this case, we use two cache.
                : comb_sum_val_cache[cache_slot_left] + comb_sum_val_cache[cache_slot_right];

            comb_sum_val_cache[comb] = sum_val;
            insert_pair_to_sum_val_map(sum_val, comb);
        }
    }

    // Good, we have filled sum_val_map. Now compose answers.
    std::vector<std::pair<uint64_t, uint64_t>> answers;
    for(const auto &kv : sum_val_map) {
        const auto &combinations = kv.second;
        if(combinations.size() == 1) continue;
        // Find all eligible pairs.
        for(auto i = 0; i < combinations.size(); ++i) {
            for(auto j = i+1; j < combinations.size(); ++j) {
                if((combinations[i] & combinations[j]) == 0)
                    answers.emplace_back(combinations[i], combinations[j]);
            }
        }
    }

    // We have got the answers! now filter answers to get top good answers.
    int max_cards = 0;
    decltype(answers) winned_answers;
    for(const auto &answer : answers) {
        const auto cards = __builtin_popcountll(answer.first) + __builtin_popcountll(answer.second);
        max_cards = std::max(cards, max_cards);
    }
    for(const auto &answer : answers) {
        const auto cards = __builtin_popcountll(answer.first) + __builtin_popcountll(answer.second);
        if(cards == max_cards)
            winned_answers.emplace_back(answer);
    }
    // still too many... only leave 5 winned answer.
    if(winned_answers.size() > 5)
        winned_answers.erase(winned_answers.begin() + 5, winned_answers.end());


    // Awesome. Print winners out!
    auto naive_answer_to_string = [&](const uint64_t &bits) -> auto {
        string res;
        for(auto i = 0; i < input.size(); ++i) {
            if((bits & 1<<i) != 0) {
                res += basic_int_to_char(input[i]);
            }
        }
        return res;
    };
    std::vector<std::pair<string, string>> result_in_string;
    for(const auto &answer : winned_answers) {
        result_in_string.emplace_back(naive_answer_to_string(answer.first), naive_answer_to_string(answer.second));
    }

    return result_in_string;
}


auto end2end_changpu_solve(string input) {
    if(input.size() <= 16)
        return end2end_changpu_solve_short(input);

    // try to split the input, find the best result for each piece.
    std::sort(input.begin(), input.end());
    auto splits = input.size() / 14 + 1;
    auto splited_base_size = input.size() / splits;
    auto xtra = input.size() % splits;

    string processedL, processedR;
    for(auto i = 0; i < splits; ++i) {
        auto begin_pos = i*splited_base_size + (i > xtra ? i : xtra);
        auto piece_len = splited_base_size + (i >= xtra ? 0 : 1);
        auto piece_result = end2end_changpu_solve_short(input.substr(begin_pos, piece_len));
        if(piece_result.empty())
            throw std::runtime_error("gg");
        processedL += piece_result[0].first;
        processedR += piece_result[0].second;
    }
    return decltype(end2end_changpu_solve_short(input)) {std::make_pair(processedL, processedR)};
}

int main(int argc, char **argv) {
    rlib::opt_parser args(argc, argv);
    auto input = args.getSubCommand();

    auto answers = end2end_changpu_solve(input);
    for(const auto &answer : answers) {
        rlib::println(answer.first, "===", answer.second);
    }
}
