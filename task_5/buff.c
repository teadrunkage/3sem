#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#define buffsize 4096

int main(void) {
	int fd1, fd2;
	
	fd1 = open("Sunset_Limited.avi", O_RDONLY);
//	printf("%d\n", fd1);
	size_t length = lseek(fd1, 0, SEEK_END);
	lseek(fd1, 0, SEEK_SET);
	fd2 = open("Sunset_Limited_copy2.avi",  O_RDWR | O_CREAT | O_TRUNC, 0666);
//	printf("%d\n", fd2);
//	ftruncate(fd2, length);
	
	char buff[buffsize];
	
	int len1, len2;
	while (1) {
		len1 = read(fd1, buff, buffsize);
		len2 = write(fd2, buff, len1);
//		printf("%d\n", len1);
//		printf("%d\n", len2);
		if(len1 < buffsize)
			break;
	}
	
	close(fd1);
	close(fd2);
	
	return 0;
}
