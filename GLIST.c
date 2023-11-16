#include <stdio.h>
#include <stdlib.h>
#include "GLIST.h"

/* glist_create: void --> GList*
 * glist_create allocates memory for a GList and returns it;
*/
GList *glist_create() {
  GList *new_list = malloc(sizeof(GList));
  new_list->first = NULL;
  new_list->last = NULL;
  return new_list;
}

/* glist_is_empty: GList* --> int
 * glist_is_empty receives a pointer to a GList and returns an int representing if its empty.
 */
int glist_is_empty(GList * list) {
  return (list == NULL || list->first == NULL);
}

/* glist_add_start: GList* void* CopyFunction --> void
 * glist_add_start receives a pointer to a GList, a void* data and a Copy function. It uses the CopyFunction to add
 * cpy(pData) to the beginning of the list
 */
void glist_add_start(GList * list, void *pData, CopyFunction cpy) {
  if (list == NULL)
    return;

  GNode *newNode = malloc(sizeof(GNode));
  newNode->data = cpy(pData);

  if (list->first)
    newNode->next = list->first;
  list->first = newNode;

  if (list->last == NULL)
    list->last = newNode;
}

/* glist_add_end: GList* void* CopyFunction --> void
 * glist_add_end receives a pointer to a GList, a void* data and a Copy function. It uses the CopyFunction to add
 * cpy(pData) to the end of the list
 */
void glist_add_end(GList * list, void *pData, CopyFunction cpy) {
  if (list == NULL)
    return;

  GNode *newNode = malloc(sizeof(GNode));
  newNode->data = cpy(pData);
  newNode->next = NULL;
  if (list->last)
    list->last->next = newNode;
  list->last = newNode;

  if (list->first == NULL)
    list->first = newNode;
}

/* glist_remove_start: GList* DestructorFunction --> void
 * glist_remove_start receives a pointer to a GList\ and a Destructor function. It uses the DestructorFunction to
 * first invoke it on the first node of the GList and then free the node.
 */
void glist_remove_start(GList * list, DestructorFunction destroy) {
  if (list == NULL || list->first == NULL)
    return;
  int hasOneElement = list->first == list->last;

  GNode *aux = list->first;
  if (hasOneElement) {
    list->first = NULL;
    list->last = NULL;
  } else
    list->first = list->first->next;

  destroy(aux->data);
  free(aux);
}

/* glist_traverse: GList* VisitorFunction --> void
 * glist_traverse receives a pointer to a GList and a VisitorFunction. It traverses the entire list from the beginning
 * applying the function to each node's data.
 */
void glist_traverse(GList * list, VisitorFunction visit) {
  if (list == NULL)
    return;
  GNode *tmp = list->first;
  while (tmp) {
    visit(tmp->data);
    tmp = tmp->next;
  }
}

/* glist_destroy: GList* DestructorFunction --> void
 * glist_destroy receives a pointer to a GList and a DestructorFunction. It destroys the list recursively, first applying
 * the destructor function to the node's data before freeing it.
 */
void glist_destroy(GList * list, DestructorFunction destroy) {
  if (list == NULL)
    return;
  GNode *tmp, *nodeToDestroy;
  tmp = list->first;
  while (tmp) {
    nodeToDestroy = tmp;
    tmp = tmp->next;

    destroy(nodeToDestroy->data);
    free(nodeToDestroy);
  }

  free(list);
}
