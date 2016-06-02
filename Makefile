GCC = gcc -Wall -pedantic

GCC_DEBUG = @$(GCC) -g

CMD = test.c easyppm.c -o test && ./test

all: run clean

run: 
	@$(GCC_DEBUG) $(CMD)

delete:
	@rm -f *.ppm *.pgm

clean:
	@rm -f test
