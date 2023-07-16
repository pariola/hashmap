#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NIL(a) a == NULL
#define NNIL(a) a != NULL

struct ht_entry
{
  void *value;
  const char *key; // key is NULL if this slot is empty
};

struct ht
{
  size_t len, cap;
  struct ht_entry *entries;
};

struct ht *ht_create(int cap)
{
  struct ht *table = malloc(sizeof(struct ht));
  if (NIL(table))
    return table; // cannot allocate memory

  table->len = 0;
  table->cap = cap;

  // allocate memory for entries with specified capacity
  table->entries = calloc(cap, sizeof(struct ht_entry));
  if (NIL(table->entries))
  {
    free(table); // free table before failing
    return NULL;
  }

  return table;
}

// Frees the memory used by the map
void ht_free(struct ht *table)
{
  // free keys
  for (size_t i = 0; i < table->cap; i++)
    if (NNIL(table->entries[i].key))
      free((void *)table->entries[i].key);

  free(table->entries); // free entries
  free(table);          // free table
}

// Returns the current length of the map
size_t ht_len(struct ht *table)
{
  return table->len;
}

// Returns the current capacity of the map
size_t ht_cap(struct ht *table)
{
  return table->cap;
}

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char *key)
{
  uint64_t hash = 2166136261u;
  for (const char *p = key; *p; p++)
  {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= 16777619;
  }
  return hash;
}

struct ht_entry *find_entry(struct ht_entry *entries, size_t cap, const char *key)
{
  size_t index = hash_key(key) % cap;

  // check taken spaces
  while (NNIL(entries[index].key))
  {
    if (!strcmp(entries[index].key, key)) // update at location
      break;

    index++; // linear probing

    // at end of entries array, go around to beginning.
    if (index >= cap)
      index = 0;
  }

  return &entries[index];
}

// Increase the capacity of the underlying entries array
bool ht_grow(struct ht *table)
{
  size_t new_len = 0;
  size_t new_cap = table->cap * 2;

  struct ht_entry *new_entries = calloc(new_cap, sizeof(struct ht_entry));
  if (NIL(new_entries))
    return false;

  for (size_t i = 0; i < table->cap; i++)
  {
    struct ht_entry entry = table->entries[i];

    // skip values that aren't set
    if (NIL(entry.key))
      continue;

    new_len++;

    struct ht_entry *e = find_entry(new_entries, new_cap, entry.key);
    e->key = entry.key;
    e->value = entry.value;
  }

  free(table->entries); // free previous allocation
  table->cap = new_cap;
  table->len = new_len;
  table->entries = new_entries;

  return true;
}

// Retuns the value set to the corresponding key, returns NULL if key doesn't exist
void *ht_get(struct ht *table, const char *key)
{
  if (table->len < 1) // no records
    return NULL;

  struct ht_entry *e = find_entry(table->entries, table->cap, key);
  if (NIL(e->key)) // no match
    return NULL;

  return e->value;
}

// Sets the value of a key in the hashmap
const char *ht_set(struct ht *table, const char *key, void *value)
{
  if (NIL(value))
    return NULL;

  // expand if neccessary
  if (table->len + 1 > table->cap * .75)
    if (!ht_grow(table))
      return NULL;

  struct ht_entry *e = find_entry(table->entries, table->cap, key);
  if (NIL(e->key)) // new records
  {
    table->len++;
    e->key = strdup(key); // duplicate key so we are in control
  }

  e->value = value;

  return key;
}
