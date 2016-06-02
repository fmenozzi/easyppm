GCC = gcc -Wall -pedantic
GPP = g++ -Wall -pedantic
CLANG = clang -Wall -pedantic
CLANGPP = clang++ -Wall -pedantic

GCC_DEBUG = @$(GCC) -g
GPP_DEBUG = @$(GPP) -g
CLANG_DEBUG = @$(CLANG) -g
CLANGPP_DEBUG = @$(CLANGPP) -g

CMD = test.c easyppm.c -o test -lm && ./test

all: test-gcc clean

test-all: test-gcc test-gpp test-clang test-clangpp clean

test-gcc:
	@echo "Testing with gcc..."
	@$(GCC_DEBUG) $(CMD)
test-gpp:
	@echo "Testing with g++..."
	@$(GPP_DEBUG) $(CMD)

test-clang:
	@echo "Testing with clang..."
	@$(CLANG_DEBUG) $(CMD)
test-clangpp:
	@echo "Testing with clang++..."
	@$(CLANGPP_DEBUG) $(CMD)


clean:
	@rm -f test *.ppm *.pgm
