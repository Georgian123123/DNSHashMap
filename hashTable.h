#ifndef HASHTABLE__C
#define HASHTABLE__C

#include "CircularDoublyLinkedList.h"

int HASH_LEN;
FILE *out;

#define STRING_LEN 256
typedef struct buckets {
    void *key;
    void *value;
} bucket;

typedef struct hashTable {
    doubly_linked_list_t **table;
    int size_hashTable;
} hashtable;

int hash(void *str);
struct hashTable *create_hash(unsigned int dimension);
void put(void *key, void *value, struct hashTable *map);
void *get(struct hashTable *map, void *key);
void *remove_key(struct hashTable *map, void *key);
int find(struct hashTable *map, void *key);
void print(struct hashTable *map);
void print_bucket(struct hashTable *map, int index_bucket);
void free_map(struct hashTable **map);
#endif //HASHTABLE__C

