/**
 * Project page: https://github.com/wangrn/ringbuffer
 * Copyright (c) 2013 Wang Ruining <https://github.com/wangrn>
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdlib.h>

struct RingBuffer;

RingBuffer rb_new(size_t capacity);
void       rb_free(RingBuffer *rb);

size_t     rb_capacity(RingBuffer *rb);
size_t     rb_can_read(RingBuffer *rb);
size_t     rb_can_write(RingBuffer *rb);

size_t     rb_read(RingBuffer *rb, void *data, size_t count);
size_t     rb_write(RingBuffer *rb, const void *data, size_t count);

#endif
