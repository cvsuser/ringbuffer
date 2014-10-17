#include <stdio.h>
#include <string.h>

#include "ringbuffer.h"

#define min(a, b) (a)<(b)?(a):(b)
void *read_thread(void *);
void *write_thread(void *);
static rb *buf = NULL;
int main()
{
    fprintf(stdout, "Hello world!\n");
    pthread_t pid1,pid2;

    buf = rb_new(1024 * 10);

    pthread_create(&pid1,NULL,read_thread,(void *)NULL);
    pthread_create(&pid2,NULL,write_thread,(void *)NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    rb_del(buf);
    return 0;
}
void *read_thread(void *p){
    u8 tmp[1024] = {0};
    int len = 1024*30;
    while(len>0){
        //int ask = min(len,1024);
        int l = rb_read(buf, tmp, min(len,1024));
        len -= l;
        printf("read left = %d\n",len);
    }
}
void *write_thread(void *p){
    u8 tmp[1024] = {0};
    int len = 1024*30;
    while(len > 0){
        //int ask = min(len,1024);
        int l = rb_write(buf, tmp, min(len,1024));
        len -= l;
        printf("write left = %d\n",len);
    }
}

