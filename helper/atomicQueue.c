#include "atomicQueue.h"

bool aqPush(AtomicQueue *aq, const void *data, size_t size) {
	if (size > SLOT_SIZE) {
		return false;
	}

	unsigned w = atomic_load(&aq->writeIndex);
	unsigned r = atomic_load(&aq->readIndex);
	unsigned next = (w + 1) % QUEUE_CAPACITY;

	if (next == r) {
		return false;
	}

	memcpy(aq->buffer[w], data, size);
	atomic_store(&aq->writeIndex, next);
	return true;
}

bool aqPop(AtomicQueue *aq, void *out, size_t size) {
	unsigned r = atomic_load(&aq->readIndex);
	unsigned w = atomic_load(&aq->writeIndex);

	if (r == w) {
		return false;
	}

	memcpy(out, &aq->buffer[r], size);
	atomic_store(&aq->readIndex, (r + 1) % QUEUE_CAPACITY);
	return true;
}
