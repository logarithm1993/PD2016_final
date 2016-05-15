all:
	g++ -o ./bin/exe ./src/test.cpp
clean:
	rm ./bin/*
	rm ./src/*.o
