run: build.cpp  trie.cpp  trie.h partition.cpp partition.h
	g++ *.cpp -o run -O3 -Wall
debug:
	g++ *.cpp -o debug -g -pg -Wall
test:
	g++ *.cpp -o test -pg -O3 -Wall
clean: 
	rm run debug test -f
