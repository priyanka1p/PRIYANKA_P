/*Rewrite fopen and _fillbuf with fields instead of explicit bit operations.
Compare code size and execution speed.*/
#include <stdio.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <stdlib.h>  

#define BUFFER_SIZE 1024
#define OPEN_MAX 20  
#define PERMS 0666  

typedef struct _iobuf {
    int cnt;        
    char *ptr;     
    char *base;     
    int fd;         
    int is_read;    
    int is_write;   
    int is_unbuf;   
    int is_eof;     
    int is_err;     
} FILEX;

static FILEX _iob[OPEN_MAX] = {
    { 0, NULL, NULL, 0, 1, 0, 0, 0, 0 },
    { 0, NULL, NULL, 1, 0, 1, 0, 0, 0 },
    { 0, NULL, NULL, 2, 0, 1, 1, 0, 0 }
};

void init_iob() {
    for (int i = 3; i < OPEN_MAX; i++) {
        _iob[i].fd = -1;
    }
}

FILEX *fopenx(char *name, char *mode) {
    int fd;
    FILEX *fp = NULL;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (int i = 0; i < OPEN_MAX; i++) {
        if (_iob[i].fd == -1) {  
            fp = &_iob[i];
            break;
        }
    }

    if (fp == NULL) {
        fprintf(stderr, "Error: No available file slots\n");
        return NULL;
    }

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else
        fd = open(name, O_RDONLY, 0);

    if (fd == -1) {
        fprintf(stderr, "Error opening file: %s\n", name);
        return NULL;
    }

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->is_read = (*mode == 'r');
    fp->is_write = (*mode == 'w' || *mode == 'a');
    fp->is_unbuf = 0;
    fp->is_eof = 0;
    fp->is_err = 0;

    return fp;
}

int _fillbuf(FILEX *fp) {
    int bufsize;

    if (!fp->is_read || fp->is_eof || fp->is_err)
        return EOF;

    bufsize = (fp->is_unbuf) ? 1 : BUFFER_SIZE;

    if (fp->base == NULL) { 
        if ((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF;  
    }
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);

    printf("Reading %d bytes\n", fp->cnt);  

    if (--fp->cnt < 0) {  
        if (fp->cnt == -1)
            fp->is_eof = 1;
        else
            fp->is_err = 1;
        fp->cnt = 0;
        return EOF;
    }

    return (unsigned char) *fp->ptr++;
}

int _flushbuf(int x, FILEX *fp) {
    int bufsize;
    int num_written;

    if (!fp->is_write || fp->is_eof || fp->is_err)
        return EOF;

    bufsize = (fp->is_unbuf) ? 1 : BUFFER_SIZE;

    if (fp->base == NULL) {  
        if ((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF;  
    } else {
        num_written = write(fp->fd, fp->base, fp->ptr - fp->base);
       if (num_written != fp->ptr - fp->base) {
            fp->is_err = 1;
            return EOF;
        }
    }

    fp->ptr = fp->base;
    fp->cnt = bufsize - 1;
    *fp->ptr++ = x;

    return x;
}

int fflushx(FILEX *fp) {
    if (!fp->is_write || fp->is_err)
        return EOF;

    return _flushbuf(0, fp);
}

int fclosex(FILEX *fp) {
    if (fp == NULL)
        return EOF;

    if (fp->is_write)
        fflushx(fp);

    close(fp->fd);

    if (fp->base)
        free(fp->base);

    fp->cnt = 0;
    fp->ptr = NULL;
    fp->base = NULL;
    fp->fd = -1;
    fp->is_read = 0;
    fp->is_write = 0;
    fp->is_unbuf = 0;
    fp->is_eof = 0;
    fp->is_err = 0;

    return 0;
}

#define getcx(p)     (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putcx(x,p)   (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x),p))
#define getcharx()   getcx(&_iob[0])
#define putcharx(x)  putcx((x), &_iob[1])

int main(int argc, char *argv[]) {
    FILEX *fp;
    int c;

    init_iob();  

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    fp = fopenx(argv[1], "r");
    if (fp == NULL) {
        return 1;
    }
  
    while ((c = getcx(fp)) != EOF) {
        putcharx(c);
    }

    fflushx(&_iob[1]); 

    fclosex(fp);
    return 0;
}
/*Debugging is easier with fp->is_read instead of fp->flags & _READ
Hello World!*?
