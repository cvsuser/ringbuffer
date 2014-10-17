
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned char u8;

typedef struct rb{
    
	pthread_mutex_t rwlock;
	pthread_cond_t cond;
    u8  *buff;
	
	size_t rb_capacity;
	int read_pos;
	int write_pos;
	int available_for_read;
	//int available_for_write = rb_capacity - available_for_read;
}rb;

rb* rb_new(size_t capacity);
void	rb_del(rb *rb);

size_t	rb_read(rb *rb, void *data, size_t count);
size_t	rb_write(rb *rb, const void *data, size_t count);

#endif
