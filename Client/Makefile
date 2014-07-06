all: client
client: main.o UploadClient.o Connection.o ConnectException.o
	g++ main.o UploadClient.o Connection.o ConnectException.o -o client
main.o: main.cpp
	g++ -c main.cpp
UploadClient.o: UploadClient.cpp
	g++ -c UploadClient.cpp
Connection.o : Connection.cpp
	g++ -c Connection.cpp
ConnectException.o : ConnectException.cpp
	g++ -c ConnectException.cpp
clean:
	rm -rf *.o client
