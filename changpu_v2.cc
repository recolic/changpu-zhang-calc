
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
    constexpr auto ls = "0A23456789JQK";
    return ls[i];
}
inline uint8_t basic_char_to_int(char c) {
    constexpr auto ls {"0A23456789JQK"sv};
    return ls.find(c);
}



int main(int argc, char **argv) {
    rlib::opt_parser args(argc, argv);
    auto input = args.getSubCommand();
    assert(input.size() <= 64); // we're using uint64 to represent combination.

    std::for_each(input.begin(), input.end(), [](auto &c) {
        c = basic_char_to_int(c);
    });
    // Now input is a number array.

    // map sum_val to [combinations...]
    std::unordered_map<uint64_t, std::vector<uint64_t>> sum_val_map;
    std::vector<uint64_t> comb_sum_val_cache(1<<input.size(), 0);
    auto insert_pair_to_sum_val_map = [&sum_val_map](uint64_t k, uint64_t v) {
        // rlib::println("DEBUG: map_insert", k, v);
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
            // rlib::printfln("DEBUG: GET cache[{}]={}", comb-1, comb_sum_val_cache[comb-1]);
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

            // rlib::printfln("DEBUG: SET cache[{}]={}", comb, sum_val);
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
    for(const auto &answer : winned_answers) {
        rlib::println(naive_answer_to_string(answer.first), "===", naive_answer_to_string(answer.second));
    }





}
