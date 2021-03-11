



function for_case () {
    pytime=`( time python3 changpu_v2.py $1 ) 2>&1 | grep real | sed 's/^.*m//g'`
    ctime=`( time ./changpu_v2 $1 ) 2>&1 | grep real | sed 's/^.*m//g'`
    echo "CASE $1: PYTHON=$pytime, C=$ctime"
}

function for_case_c () {
    ctime=`( time ./changpu_v2 $1 ) 2>&1 | grep real | sed 's/^.*m//g'`
    echo "CASE $1: PYTHON=......., C=$ctime"
}


for_case Q
for_case 42
for_case 5Q2
for_case 5A53
for_case 5669
for_case 54Q3
for_case A877
for_case KJA5
for_case K325
for_case Q6348
for_case AK6522
for_case QJ35482
for_case 9583JQ3A
for_case K98KK8239
for_case 45232A62A4
for_case A9JK95A8K98
for_case 437998Q7Q637
for_case 4K223JQ82Q6J5
for_case J43J744392A563
for_case 64322666KJK36AJ
for_case 8AAQ4355A626276K
for_case_c JQ68K2745574A5KA9
for_case_c 5978339223K9J73A7J
for_case_c 24KJ7Q7886592264278
for_case_c 3729J6JQ6JQ63QQ75KK2
for_case_c K3K2495AJ652J749J32K8



# for len in (seq 20)
#     for i in (seq $len)
#         echo -n (random choice A 2 3 4 5 6 7 8 9 J Q K)
#     end
#     echo ''
# end

