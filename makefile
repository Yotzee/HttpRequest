all:
	g++ main.cpp -o main

debug:
	g++ main.cpp -o main -v -g
	lldb main

run:
	g++ main.cpp -o main
	./main localhost / 3000