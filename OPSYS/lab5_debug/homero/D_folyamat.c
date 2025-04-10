#include "myinclude.h"
#include <math.h>

int main(int argc, char const *argv[])
{
    FILE * fifo;
    if ((fifo=fopen("myfifo","r"))==NULL) syserr("fifo");

    FILE * alert;
    if ((alert=fopen("alert.log","w"))==NULL) syserr("alert");

    double x;
    while (fscanf(fifo,"%lf",&x)==1){
        if (fabs(x)>2){
            fprintf(alert,"Alert: %lf\n",x);
        }
    }
    fclose(fifo);
    fclose(alert);
    return 0;
}
