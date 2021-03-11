
CC ?= gcc -O3

build:
	cython --embed changpu_v2.py
	$(CC) changpu_v2.c -o changpu_v2 $$(python3-config --cflags --ldflags --embed)

clean:
	rm -f *.c changpu changpu_v2

