#ifndef __BUFFER_H__
#define __BUFFER_H__

#define Buffer_writeString(buf, msg) (buf)->interface->writeString(buf, msg)
#define Buffer_toString(buf) (buf)->interface->toString(buf)

typedef struct _Buffer Buffer;

typedef struct _BufferInterface {
    void (*writeString)(Buffer *self, const char *str);
    char* (*toString)(Buffer *self);
} BufferInterface;

typedef struct _Buffer {
    int size;
    int len;
    char *str;
    BufferInterface* interface;
} Buffer;

Buffer *newBuffer();
void freeBuffer(Buffer *buf);

#endif
