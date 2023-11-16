#ifndef _GQUEUES_H_
#define _GQUEUES_H_
#include "GLIST.h"

typedef GList *Queue;

void *id(void *t);

void skip(void *x);

Queue queue_create();

int queue_is_empty(Queue queue);

void queue_destroy(Queue queue, DestructorFunction destroy);

void queue_enqueue(Queue queue, void *pData, CopyFunction cpy);

void *queue_start(Queue queue, CopyFunction cpy);

void queue_dequeue(Queue queue, DestructorFunction destroy);

#endif
