#ifndef ATOMIC_QUEUE
#define ATOMIC_QUEUE

#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define QUEUE_CAPACITY 256
#define SLOT_SIZE 64

typedef struct {
	uint8_t buffer[QUEUE_CAPACITY][SLOT_SIZE];
	atomic_uint writeIndex;
	atomic_uint readIndex;
} AtomicQueue;

bool aqPush(AtomicQueue *aq, const void *data, size_t size);
bool aqPop(AtomicQueue *aq, void *out, size_t size);
#endif
