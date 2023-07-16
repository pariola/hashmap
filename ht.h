struct ht;

struct ht *ht_create(int);
void ht_free(struct ht *table);

size_t ht_len(struct ht *table);
size_t ht_cap(struct ht *table);

// bool ht_del(struct ht *table, const char *key);
void *ht_get(struct ht *table, const char *key);
const char *ht_set(struct ht *table, const char *key, void *value);