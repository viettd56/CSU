#include "uploadClient.h"

using namespace client;

int main(){
	UploadClient client;
	char * filename = new char[256];
	int n =client.setConnection("localhost",123);
	if(n == 0){
		cout << "Nhap ten file can upload : " << endl;
		cin.getline(filename,256);
		cout << filename << endl;
		client.upload(filename);
	}
	return 0;
}