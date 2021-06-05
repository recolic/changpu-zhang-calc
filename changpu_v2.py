#cython: language_level=3

import sys
if len(sys.argv) == 2:
    s = sys.argv[1]
else:
    s = input("INPUT: ")

import itertools

def basic_char_to_int(ch):
    ls = 'xA234567890JQK'
    return ls.find(ch)
def basic_int_to_char(i):
    ls = 'xA234567890JQK'
    return ls[i]

def comb_exclude_border(ar):
    results = []
    for i in range(len(ar)):
        if i != 0:
            results += list(itertools.combinations(ar, i))
    return results
def sorted_list_dedup(ls):
    if len(ls) == 0:
        return ls
    prev = "Impossible to match"
    res = []
    for elem in ls:
        if elem != prev:
            prev = elem
            res.append(elem)
    return res

def arr_to_bitarray(arr):
    result = 0
    for ele in arr:
        result += 2**ele
    return result


ar = [basic_char_to_int(ch) for ch in s]
combinations = comb_exclude_border(range(len(ar)))
# contains sumval -> [[indexes...], ...]
# 100 -> [[0,1,2,3], [6,7,8]]
sum_buf = {}
for comb in combinations:
    sum_val = sum([ar[index] for index in comb])
    if sum_val in sum_buf:
        sum_buf[sum_val].append(arr_to_bitarray(comb))
    else:
        sum_buf[sum_val] = [arr_to_bitarray(comb)]

answers = []
def index_bitarray_to_answer(bitar):
    answer = []
    for rank in range(100):
        if 2**rank > bitar:
            return answer
        if 2**rank & bitar != 0:
            answer.append(ar[rank])
    return answer

for sum_val in sum_buf:
    ls = sum_buf[sum_val]
    if len(ls) < 2:
        continue
    # find pairs without collision
    for pair in itertools.combinations(ls, 2):
        if pair[0] & pair[1] == 0:
            # found an answer! decompress bitarray. 
            answers.append((index_bitarray_to_answer(pair[0]), index_bitarray_to_answer(pair[1])))
    

# now filter answers
quant = [len(answer[0]) + len(answer[1]) for answer in answers]
answers.sort(reverse=True, key=lambda answer: len(answer[0]) + len(answer[1]))
answers = sorted_list_dedup(answers)
answers = answers[:5] if len(answers) > 5 else answers

# print answers
for answer in answers:
    halfA_s = ''.join([basic_int_to_char(i) for i in answer[0]])
    halfB_s = ''.join([basic_int_to_char(i) for i in answer[1]])
    print(halfA_s, "===", halfB_s)
    

