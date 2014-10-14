#ifndef _list_h
#define _list_h

struct point_t {
  int x;
  int y;
};

typedef struct point_t point;

struct node_t {
  point p;
  struct node_t* next;
  struct node_t* prev;
};

typedef struct node_t node;

struct list_t {
  node* head;
  node* tail;
};

typedef struct list_t list;

list* list_create();
node* list_insert(node* n, point p);
void list_delete(list* ll);

#endif
