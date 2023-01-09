CFLAGS=-Wall -Wextra -pedantic -std=c++17 -lX11 -lstdc++fs

stack: stack.cpp 
	g++ $(CFLAGS) -o stack stack.cpp

stack.cpp:
	

.PHONY : clean
clean:
	rm stack 
