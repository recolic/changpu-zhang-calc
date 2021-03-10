#cython: language_level=3

s = 'A8492QJ835433'
s = input("INPUT: ")

import itertools

def basic_char_to_int(ch):
    ls = '0A23456789JQK'
    return ls.find(ch)
def basic_int_to_char(i):
    ls = '0A23456789JQK'
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
def set_try_sum_to_number(ls, target_int):
    ls = ls.sort()
    return "TODO" # TODO: speed up

# we devide array to two half: halfA+halfB. 
# Then find halfA == one of halfB.subset()
def combinations_pair_gen(ar):
    combinations = comb_exclude_border(range(len(ar)))
    results = []
    for combination in combinations:
        halfA, halfB = [], []
        for i in range(len(ar)):
            (halfA if i in combination else halfB).append(ar[i])
        results.append((halfA, halfB))
    return results


ar = [basic_char_to_int(ch) for ch in s]
possibilities = combinations_pair_gen(ar)
answers = []
for possib in possibilities:
    halfA, halfB = possib
    if sum(halfA) > sum(halfB):
        continue
    elif sum(halfA) == sum(halfB):
        answers.append((halfA, halfB))
    else:
        # let's split halfB
        for possib_halfB in comb_exclude_border(halfB):
            if sum(halfA) == sum(possib_halfB):
                answers.append((halfA, possib_halfB))

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
    

