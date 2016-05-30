all:
	g++ -o ./bin/MPTree ./src/*.cpp 
clean:
	rm ./bin/MPTree
	rm ./src/*.o
