CC = g++
CCOPTS = -Wall -g

timertest : timer.h timertest.cc
	$(CC) $(CCOPTS) timertest.cc -o timertest

clean :
	rm -rf *.dSYM timertest

