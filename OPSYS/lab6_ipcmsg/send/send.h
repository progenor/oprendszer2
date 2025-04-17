// az uzenet hossza
#define SIZE 128

// az üzenet típusa
typedef struct {
    long mtype;
    char mtext[SIZE];
} message_buf;

#define FTFILE "."
#define FTPRJ 'a'