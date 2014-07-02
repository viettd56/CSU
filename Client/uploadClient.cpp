#include "uploadClient.h"

using namespace std;

namespace client{

	UploadClient::UploadClient(){
		cout << "Client created !" << endl;
	}

	UploadClient::~UploadClient(){
	 	cout <<"Client destroyed !"<< endl;
		close(sockfd);
	}

	int UploadClient::setConnection(const char* host, const int iPort){
		port=iPort;
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd < 0){
        	cout <<"ERROR opening socket"<< endl;
        	return -1;
    	}
    	server = gethostbyname(host);
    	if (server == NULL) {
        	fprintf(stderr,"ERROR, no such host\n");
        	return -1;
    	}
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    	serv_addr.sin_port = htons(port);
	   	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        	cout <<"ERROR connecting"<< endl;
        	return -1;
  		}

  		return 0;
	}

	bool UploadClient::upload(const char* filename){
		int n;
	    int length;
		char * buffer;
		length =strlen(filename)+3; // do dai ten file + 3 byte cho ma thong bao
		buffer = new char [length];
		// gui ten file
		strcpy(buffer,"200");
		strcat(buffer,filename);
		n = write(sockfd,buffer,length);
    	if (n < 0){
        	 cout <<"ERROR writting to socket"<< endl;
       	}
       	delete buffer;
       	length=0;
       	
	  	ifstream is;
	  	is.open (filename, ios::binary );
	 	// get length of file:
	  	is.seekg (0, ios::end);
	  	length = is.tellg();
 	 	cout << "length :" << length << endl;
 	 	
		//gui dung luong file -300length
		
		buffer=new char[259];
		strcpy(buffer,"300");
		stringstream strs;
  		strs << length;
  		string temp_str = strs.str();
  		char* length_mess = (char*) temp_str.c_str();
		strcat(buffer,length_mess);
		n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0){
        	 cout <<"ERROR writting to socket"<< endl;
       	}
       	delete buffer;

       	is.seekg (0, ios::beg);

	  	// allocate memory:
	  	buffer = new char [length];
	 	// read data as a block:
		is.read (buffer,length); 
		n = write(sockfd,buffer,length);
    	if (n < 0) 
        	 cout <<"ERROR writting to socket"<< endl;
		is.close();
		delete buffer;
	}	
}