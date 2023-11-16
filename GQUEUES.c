#include <stdio.h>
#include <stdlib.h>
#include "GQUEUES.h"

typedef GList *Queue;

/* id: void* --> void*
 * id is an auxiliary function that receives a void* and returns it.
*/
void *id(void *t) {
  return t;
}

/* skip: void* --> void
 * skip is an auxiliary function that receives a void* and does nothing.
 */
void skip(void *x) {
}

/* queue_create: void --> Queue
 * queue_create returns a new empty Queue.
 */
Queue queue_create() {
  return glist_create();
}

/* queue_is_empty: Queue --> int
 * queue_is_empty receives a Queue and returns an int representing its emptiness.
 */
int queue_is_empty(Queue queue) {
  return glist_is_empty(queue);
}

/* queue_destroy: Queue DestructorFunction --> void
 * queue_destroy receives a Queue and a DestructorFunction, and destroys the Queue while previously applying
 * the DestructorFunction to each of the elements stored in it.
 */
void queue_destroy(Queue queue, DestructorFunction destroy) {
  glist_destroy(queue, destroy);
}

/* queue_enqueue: Queue void* CopyFunction --> void
 * queue_enqueue receives a Queue, a void* and a CopyFunction, and adds CopyFunction(void*) to the end of the queue.
 */
void queue_enqueue(Queue queue, void *pData, CopyFunction cpy) {
  glist_add_end(queue, pData, cpy);
}

/* queue_start: Queue CopyFunction --> void*
 * queue_start receives a Queue and a CopyFunction, and returns CopyFunction(< first >) where < first > is the first element 
 * in the queue.
 */
void *queue_start(Queue queue, CopyFunction cpy) {
  if (queue == NULL || queue->first == NULL)
    return NULL;
  return cpy(queue->first->data);
}

/* queue_dequeue: Queue DestructorFunction --> void
 * queue_dequeue receives a Queue and a DestructorFunction. It first applies the DestructorFunction to the first element of
 * the queue and then removes it from the Queue.
 */
void queue_dequeue(Queue queue, DestructorFunction destroy) {
  glist_remove_start(queue, destroy);
}
