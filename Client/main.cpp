#include <iostream>
#include "UploadClient.h"

using std::cerr;
using std::cout;
using std::endl;
using client::UploadClient;
using namespace std;
int main() {

	char s[256];
	try {
		UploadClient* client = new UploadClient("localhost", 1234);
		cout << "Nhap ten file :";
		cin.getline(s,256);
		client->upload(s);
	} catch (exception&e) {
		cerr << e.what();
	}
	return 0;
}
