
#include <assert.h>
#include <string.h>
#include "ht.h"

int main()
{
  struct ht *h = ht_create(4);
  assert(h != NULL);

  assert(ht_len(h) == 0);
  assert(ht_cap(h) == 4);

  ht_set(h, "one", "hello");

  ht_set(h, "two", "12345");
  ht_set(h, "two", "98765");

  assert(ht_len(h) == 2);
  assert(ht_cap(h) == 4);

  ht_set(h, "three", "12324");

  assert(ht_len(h) == 3);
  assert(ht_cap(h) == 4);

  ht_set(h, "four", "12324");

  assert(ht_len(h) == 4);
  assert(ht_cap(h) == 8);

  char *val = ht_get(h, "one");
  assert(strcmp(val, "hello") == 0);

  ht_free(h);

  return 0;
}