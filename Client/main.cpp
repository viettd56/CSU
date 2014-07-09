#include <iostream>
#include "UploadClient.h"

using std::cerr;
using std::cout;
using std::endl;
using client::UploadClient;
using namespace std;

void usage(void);
int checkStep(char* s);

int main(int argc, char* argv[]) {

	if (argc < 3) {
		cout << "\n\tUsage: " << argv[0] << " [hostname] [port]\n" << endl;
		exit(0);
	}
	int portno = atoi(argv[2]);
	int check = 0;
	char* p;
	char s[256];
	try {
		UploadClient* client = new UploadClient(argv[1], portno);
		usage();
		do {
			cout << ">";
			cin.getline(s, 256);
			p = strtok(s, " ");
			if (strcmp(p, "upload") == 0) {
				p = strtok(NULL, " ");
				check = client->upload(p);
				if (check == 0) {
					cout << "upload fail " << endl;
				} else {
					cout << "upload success" << endl;
				}
			} else if (strcmp(p, "resume") == 0) {
				p = strtok(NULL, " ");
				check = client->resume(p);
				if (check == 0) {
					cout << "resume fail " << endl;
				} else {
					cout << "resume success" << endl;
				}
			} else if(strcmp(p,"quit")==0){
				client->quit();
				delete client;
				client =NULL;
				exit(0);
			}
			else {
				usage();
			}
		} while (true);

	} catch (exception&e) {
		cerr << e.what() << endl;
	}
	return 0;
}
void usage(void) {
	cout << "\tUsage: client [hostname] [port]" << endl;
	cout << "\t  	upload [filename] : upload file" << endl;
	cout << "\t   	resume [filename] : resume file" << endl;
	cout << "\t   	quit              :	quit " << endl;
}

