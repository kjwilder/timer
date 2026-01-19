CC = g++
CCOPTS = -Wall -g -std=c++11

# Note: For running tests, use Bazel: bazel test :all
# This Makefile is kept for reference/manual compilation

.PHONY: clean
clean :
	rm -rf *.dSYM *.o

