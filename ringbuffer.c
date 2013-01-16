/**
 * Project page: https://github.com/wangrn/ringbuffer
 * Copyright (c) 2013 Wang Ruining <https://github.com/wangrn>
 * @date 2013/01/16 13:33:20
 * @brief   a simple ringbuffer, DO NOT support dynamic expanded memory
 */

typedef struct {
    size_t rb_capacity;
    char  *rb_head;
    char  *rb_tail;
    char  *rb_buff;
}RingBuffer;

RingBuffer rb_new(size_t capacity)
{
    RingBuffer *rb = (RingBuffer *) malloc(sizeof(RingBuffer) + capacity);
    if (rb == NULL) return NULL;
    
    rb->rb_capacity = capacity;
    rb->rb_buff     = (char*)rb + sizeof(RingBuffer);
    rb->rb_head     = rb->rb_buff;
    rb->rb_tail     = rb->rb_buff;
};

void  rb_free(RingBuffer *rb)
{
    free((char*)rb);
}

size_t     rb_capacity(RingBuffer *rb)
{
    assert(rb != NULL);
    return rb->rb_capacity;
}
size_t     rb_can_read(RingBuffer *rb)
{
    assert(rb != NULL);
    if (rb->rb_head == rb->rb_tail) return 0;
    if (rb->rb_head < rb->rb_tail) return rb->rb_tail - rb->rb_head;
    return rb_capacity(rb) - (rb->rb_head - rb->rb_tail);
}
size_t     rb_can_write(RingBuffer *rb)
{
    assert(rb != NULL);
    return rb_capacity(rb) - rb_can_read(rb);
}

size_t     rb_read(RingBuffer *rb, void *data, size_t count)
{
    assert(rb != NULL);
    assert(data != NULL);
    if (rb->rb_head < rb->rb_tail)
    {
        int copy_sz = min(count, rb_can_read(rb));
        memcpy(data, rb->rb_head, copy_sz);
        rb->rb_head += copy_sz;
        return copy_sz;
    }
    else
    {
        if (count < rb_capacity(rb)-(rb->rb_head - rb->rb_buff))
        {
            int copy_sz = count;
            memcpy(data, rb->rb_head, copy_sz);
            rb->rb_head += copy_sz;
            return copy_sz;
        }
        else
        {
            int copy_sz = rb_capacity(rb) - (rb->rb_head - rb->rb_buff);
            memcpy(data, rb->rb_head, copy_sz);
            rb->rb_head = rb->rb_buff;
            copy_sz += rb_read(rb, data+copy_sz, count-copy_sz);
            return copy_sz;
        }
    }
}

size_t     rb_write(RingBuffer *rb, const void *data, size_t count)
{
    assert(rb != NULL);
    assert(data != NULL);
    
    if (count >= rb_can_write(rb)) return -1;
    
    if (rb->head <= rb->tail)
    {
        int tail_avail_sz = rb_capacity(rb) - (rb->rb_tail - rb->rb_buff);
        if (count <= tail_avail_sz)
        {
            memcpy(rb->rb_tail, data, count);
            rb->rb_tail += count;
            if (rb->rb_tail == rb->rb_buff+rb_capacity(rb))
                rb->rb_tail = rb->rb_buff;
            return count;
        }
        else
        {
            memcpy(rb->rb_tail, data, tail_avail_sz);
            rb->rb_tail = rb->rb_buff;
            
            return tail_avail_sz + rb_write(rb, data+tail_avail_sz, count-tail_avail_sz);
        }
    }
    else
    {
        memcpy(rb->rb_tail, data, count);
        rb->rb_tail += count;
        return count;
    }
}
