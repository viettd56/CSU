all: client
client: main.o uploadClient.o
	g++ main.o uploadClient.o -o client
main.o: main.cpp
	g++ -c main.cpp

uploadClient.o: uploadClient.cpp
	g++ -c uploadClient.cpp

clean:
	rm -rf *.o client
