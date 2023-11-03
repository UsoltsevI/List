all: List.o ListTest.o
	g++ List.o ListTest.o -o L.exe

List.o: List.cpp
	g++ List.cpp -c

ListTest.o: ListTest.cpp
	g++ ListTest.cpp -c 

clean: 
	rm -f List.o ListTest.o

cleandot:
	rm -f graph000.dot graph001.dot graph002.dot graph003.dot graph004.dot graph005.dot graph006.dot graph007.dot graph008.dot graph009.dot graph010.dot

cleanpng:
	rm -f graph000.png graph001.png graph002.png graph003.png graph004.png graph005.png graph006.png graph007.png graph008.png graph009.png graph010.png
