GCC = gcc -Wall -pedantic

GCC_DEBUG = @$(GCC) -g

CMD = test.c easyppm.c -o test && ./test

all: run

run: 
	@$(GCC_DEBUG) $(CMD)

clean:
	@rm -f test *.ppm *.pgm
