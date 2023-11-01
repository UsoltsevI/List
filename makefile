all: List.o ListTest.o
	g++ List.o ListTest.o -o L.exe

List.o: List.cpp
	g++ List.cpp -c

ListTest.o: ListTest.cpp
	g++ ListTest.cpp -c 

clean: 
	rm -f List.o ListTest.o
