
#include "myinclude.h"

#include <ctype.h>

int main ( int argc, char * argv[])
{

    if (argc != 3 ) {
        printf ("use: %s file1 file2 \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd1, fd2;// ide kerÜl az azonosító
    if ((fd1 = open(argv[1], O_RDONLY))<0) syserr("open file1");

    if ((fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1) {
        perror("open file 2");
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////

    off_t file_size;
    char ch;
    
    file_size = lseek(fd1, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    
    for (off_t pos = file_size - 1; pos >= 0; pos--) {
        if (lseek(fd1, pos, SEEK_SET) == -1) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        
        if (read(fd1, &ch, 1) != 1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        if (write(fd2, &ch, 1) != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    } 
        
    exit(EXIT_SUCCESS);

}
