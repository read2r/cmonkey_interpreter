#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#define BUFFER_LEN 512  
#define BUFFER_LEN_STEP 512

char *toString(Buffer *self) {
    return self->str;
}

void writeString(Buffer *self, const char *str) {
    int totalLen = self->len + strlen(str) + 1;
    if(totalLen >= self->size) {
        self->size += BUFFER_LEN_STEP;
        self->str = (char*)realloc(self->str, self->size);
    }
    self->len += sprintf(self->str + self->len, "%s", str);
}

BufferInterface *newBufferInterface() {
    BufferInterface *bi = (BufferInterface*)malloc(sizeof(BufferInterface));
    *(bi) = (BufferInterface) {
        .writeString = writeString,
        .toString = toString,
    };
    return bi;
}

Buffer *newBuffer() {
    Buffer* buf = (Buffer*)malloc(sizeof(Buffer));
    buf->size = BUFFER_LEN;
    buf->len = 0;
    buf->str = (char*)malloc(sizeof(char) * BUFFER_LEN);
    buf->interface = newBufferInterface();
    return buf;
}

void freeBuffer(Buffer *buf) {
    // free(buf->str);
    free(buf->interface);
    free(buf);
}
