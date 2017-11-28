#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

int main(void) {
	int fd1, fd2;
	
	fd1 = open("Sunset_Limited.avi", O_RDONLY);
//	printf("%d\n", fd1);
	size_t length = lseek(fd1, 0, SEEK_END);
	fd2 = open("Sunset_Limited_copy.avi",  O_RDWR | O_CREAT | O_TRUNC, 0666);
//	printf("%d\n", fd2);
	ftruncate(fd2, length);
	
	void * ptr1;
	void * ptr2;
	
	ptr1 = mmap(NULL, length, PROT_READ, MAP_SHARED, fd1, 0);
	ptr2 = mmap(NULL, length, PROT_WRITE, MAP_SHARED, fd2, 0);
	
	memcpy(ptr2, ptr1, length);
	munmap(ptr1, length);
	munmap(ptr2, length);
	
	close(fd1);
	close(fd2);
	
	return 0;
}
