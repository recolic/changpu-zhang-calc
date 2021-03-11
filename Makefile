
CC ?= gcc -O3
CXX ?= g++ -O3

build_cython:
	cython --embed changpu_v2.py
	$(CC) changpu_v2.c -o changpu_v2 $$(python3-config --cflags --ldflags --embed)

build_cxx:
	$(CXX) changpu_v2.cc -o changpu_v2 -std=c++17

clean:
	rm -f *.c changpu changpu_v2

