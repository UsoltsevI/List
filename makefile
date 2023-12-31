all: List.o ListTest.o
	g++ List.o ListTest.o -o L.exe

List.o: List.cpp
	g++ List.cpp -c

ListTest.o: ListTest.cpp
	g++ ListTest.cpp -c 

clean: 
	rm -f List.o ListTest.o

cleandot:
	rm -f dump/graph000.dot dump/graph001.dot dump/graph002.dot dump/graph003.dot dump/graph004.dot dump/graph005.dot dump/graph006.dot dump/graph007.dot dump/graph008.dot dump/graph009.dot dump/graph010.dot
	rm -f dump/graph011.dot dump/graph012.dot dump/graph013.dot dump/graph014.dot dump/graph015.dot dump/graph016.dot dump/graph017.dot dump/graph018.dot dump/graph019.dot dump/graph020.dot dump/graph021.dot 

cleanpng:
	rm -f dump/graph000.png dump/graph001.png dump/graph002.png dump/graph003.png dump/graph004.png dump/graph005.png dump/graph006.png dump/graph007.png dump/graph008.png dump/graph009.png dump/graph010.png
	rm -f dump/graph011.png dump/graph012.png dump/graph013.png dump/graph014.png dump/graph015.png dump/graph016.png dump/graph017.png dump/graph018.png dump/graph019.png dump/graph020.png dump/graph021.png 

