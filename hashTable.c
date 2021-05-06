#include "hashTable.h"

/*
 * @param
 * str - stringul caruia vrem sa ii aflam hasul
 * @value
 * hashul stringului
 */
int hash(void *str)
{
    char *str_val = (char *)str;
    int val = 0;

    /*
     *  Traversam stringul si adunam codul ascii
     */
    for (int i = 0; i < strlen(str_val); ++i) {
        val += (int)str_val[i];
    }

    /* returnam valoarea data cu modulo ca sa pastram
     * capetele mapului
     */
    return val % HASH_LEN;
}

/*
 * @param
 * dimension -> Dimensiunea mapului
 * @value
 * Functia aceasta creeaza spatiu pentru map si pentru bucketuri
 */
struct hashTable *create_hash(unsigned int dimension)
{
    /*
     * creare hash
     */
    struct hashTable *map = (struct hashTable *)calloc(1, sizeof(struct hashTable));
    if (map == NULL)
        return NULL;
    
    /*
     * alocare spatiu pentru bucketuri
     */
    map->table = (doubly_linked_list_t **)calloc(dimension, sizeof(doubly_linked_list_t*));
    if (map->table == NULL) {
        free(map);
        return NULL;
    }
    map->size_hashTable = dimension;

    /*
     * Pointam catre null cu toate elementele
     */
    for (int i = 0; i < dimension; i++) {
        map->table[i] = NULL;
    }
    return map;
}

/*
 * @param
 * key - cheia pe care vrem sa o punem in hash
 * value - valoarea pe care vrem sa o adaugam in lista
 * map - map-ul nostru
 * @value
 * Functia adauga o pereche de forma cheie-valoare in hash
 */
void put(void *key, void *value, struct hashTable *map)
{
    char *key_value = (char *)key;
    char *value_list = (char *)value;
    int index = 0;
    struct buckets bucket_aux;
    
    /*
     * vedem daca cheia exista in mapa
     */
    int pos_key = find(map, key);
    if (pos_key == -1) {
        /*
         * Daca cheia nu a fost gasita in mapa, alocam memorie
         * pentru lista, si apoi adaugam in lista
         */
        int pos = hash(key_value);
        /* 
         * punem datele in buckets
         */
        bucket_aux.key = (char *)calloc(STRING_LEN, sizeof(char));
        bucket_aux.value = (char *)calloc(STRING_LEN, sizeof(char));
        strcpy(bucket_aux.key, key_value);
        strcpy(bucket_aux.value, value_list);
        if (map->table[pos] != NULL) {
            index = cmp_keys(map->table[pos], key);
            dll_add_nth_node((doubly_linked_list_t *)(map->table[pos]), 
                            index,
                            &bucket_aux);
        } else {
            map->table[pos] = dll_create(sizeof(struct buckets));
            dll_add_nth_node((doubly_linked_list_t *)(map->table[pos]), 
                            ((doubly_linked_list_t *)(map->table[pos]))->size,
                            &bucket_aux);
        }
    }
}


/*
 * @param
 * map - map-ul in care cautam
 * key - cheia pe care o cautam
 * @value
 * Functia aceasta cauta o cheie in hashmap
 */
int find(struct hashTable *map, void *key)
{   
    /*
     * vedem daca cheia asociata hash-ului key ca parametru
     * este diferita de null, caz in care cheia exista
     */
    int hash_val = hash(key);
    int rc = 0;

    if (map->table[hash_val] != NULL) {
        rc = return_index_elem(map->table[hash_val], key);
        return rc;
    }
    return -1;
}

/*
 * @param
 * map - map-ul in care cautam cheia
 * key - cheia pe care vrem sa o gasim
 * @value
 * intoarce valoarea de la cheia din map
 */
void *get(struct hashTable *map, void *key)
{   
    /* cautam sa vedem daca exista cheia in map */
    int pos = find(map, key), index = 0;
    if (pos == -1)
        return NULL;

    /* Luam indexul din lista al elementului */
    index = return_index_elem(map->table[hash(key)], key);

    /* Luam elementul aflat la acea pozitie */
    dll_node_t *aux = dll_get_nth_node(((doubly_linked_list_t *)map->table[hash(key)]), index - 1); 
    
    return (char *)((struct buckets *)aux->data)->value;
}

/*
 * @param
 * map - tabela pe care vrem sa o afisam
 * @value
 * afiseaza tot hashmap-ul
 */
void print(struct hashTable *map)
{
    int i = 0, contor = 0;
    char *value;

    for (i = 0; i < HASH_LEN; i++) {
        if (map->table[i] != NULL) {
            if (map->table[i]->size == 0) {
                fprintf(out, "VIDA\n");
            } else {
                fprintf(out, "%d: ", i);
                dll_print_string_list(map->table[i]);
            }
        }
    }
}

/*
 * @param
 * map - tabela din care vrem sa afisam
 * bucket_index - index-ul bucketului pe care vrem sa l afisam
 * @value
 * functia afiseaza valorile din bucket-ul specificat
 */
void print_bucket(struct hashTable *map, int bucket_index)
{
    if (bucket_index >= HASH_LEN) {
        return;
    }

    doubly_linked_list_t *list_aux = map->table[bucket_index];
    if (list_aux != NULL) {
        dll_print_string_list(list_aux);
    } else {
        fprintf(out, "VIDA\n");
    }
}

/*
 * @param
 * map - tabela din care vrem sa stergem
 * key - cheia pe care vrem s-o stergem
 * @value
 * Sterge o cheie din bucket in cazul in care exista
 */
void *remove_key(struct hashTable *map, void *key)
{
    int index = 0;
    int pos = 0;
    
    pos = find(map, key);
    if (pos != -1) {
        struct dll_node_t *aux = dll_remove_nth_node(map->table[hash(key)], pos - 1);
        remove_node(&aux);
        if (map->table[hash(key)]->size == 0) {
            free(map->table[hash(key)]);
            map->table[hash(key)] = NULL;
        }
    }
    return NULL;
}

/*
 * @param
 * map - tabela pe care vrem s-o stergem
 * @value
 * sterge hashmap-ul
 */
void free_map(struct hashTable **map)
{
    for (int i = 0; i < HASH_LEN; i++) {
        doubly_linked_list_t *list_aux = (*map)->table[i];
        if (list_aux != NULL)
            dll_free(&list_aux);
    }
    free((*map)->table);
    free(*map);
}