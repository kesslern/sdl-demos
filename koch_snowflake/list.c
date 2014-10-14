#include <stdlib.h>
#include "list.h"

list* list_create()
{
  list* ll = malloc(sizeof(ll));

  ll->head = malloc(sizeof(node));
  ll->tail = malloc(sizeof(node));

  ll->head->next = ll->tail;
  ll->head->prev = NULL;

  ll->tail->next = NULL;
  ll->tail->prev = ll->head;

  return ll;
}

void list_delete(list* ll)
{
  node* n;

  n = ll->head;

  while (n->next != NULL) {
    n = n->next;
    free(n->prev);
  }

  free(n);
  free(ll);
}

node* list_insert(node* n, point p)
{
  node* new = malloc(sizeof(node));

  new->p.x = p.x;
  new->p.y = p.y;
  
  new->next = n->next;
  new->prev = n;

  n->next = new;
  new->next->prev = new;

  return new;
}
