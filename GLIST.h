#ifndef _GLIST_H_
#define _GLIST_H_

typedef void (*DestructorFunction)(void *data);
typedef void *(*CopyFunction)(void *data);
typedef void (*VisitorFunction)(void *data);

typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef struct _GList {
  GNode *first, *last;
} GList;

GList *glist_create();

int glist_is_empty(GList * list);

void glist_add_start(GList * list, void *pData, CopyFunction cpy);

void glist_add_end(GList * list, void *pData, CopyFunction cpy);

void glist_remove_start(GList * list, DestructorFunction destroy);

void glist_traverse(GList * list, VisitorFunction visit);

void glist_destroy(GList * list, DestructorFunction destroy);

#endif
