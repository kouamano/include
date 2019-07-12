/* hash.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "hash.h"

#define MULTIPLIER 31

struct pair {
    char* key;
    void *val;
    struct pair *next;
};

struct hash {
    struct pair **table;
    size_t value_size;
    size_t table_size;
};

struct similarity_hash{
        int node_s;
        int node_e;
        float sim;
};


static void err_exit(const char *str);
static size_t hash_func(struct hash *hash, const char *key);
static void *make_pair(struct hash* hash, const char *key, const void *value);
static void free_pair(struct pair *pair);

HASH make_hash(size_t table_size, size_t value_size)
{
    struct hash *hash;
    int i;

    if ( (hash = (struct hash*)malloc(sizeof(struct hash))) == NULL)
        err_exit("cannnot allocate memory.");
    if ( (hash->table = (struct pair**)malloc(sizeof(struct pair*) * table_size)) == NULL)
        err_exit("cannnot allocate memory.");

    for (i = 0; i < table_size; i++)
        hash->table[i] = NULL;
    hash->value_size = value_size;
    hash->table_size = table_size;
        
    return (HASH)hash;
}

void free_hash(HASH hash)
{
    struct hash *h;
    struct pair *p, *next;
    int i;

    h = (struct hash*)hash;

    for (i = 0; i < h->table_size; i++) {
        if (h->table[i] != NULL) {
            for (p = h->table[i]; p != NULL; p = next) {
                next = p->next;
                free_pair(p);
                p = NULL;
            }
        }
    }
    
    free(h->table);
    h->table = NULL;
    h->value_size = 0;
    h->table_size = 0;

    free(h);
}

void hash_put(HASH hash, const char *key, const void *value)
{
    struct hash *h;
    void *result;

    h = (struct hash *)hash;
    result = hash_get(hash, key);

    if (result != HASH_NOVAL) {
        memcpy(result, value, h->value_size);
    } else {
        struct pair *p;
        size_t hash_val;

        p = make_pair(h, key, value);
        hash_val = hash_func(h, key);
        p->next = h->table[hash_val];
        h->table[hash_val] = p;
    }
}

void *hash_get(HASH hash, const char *key)
{
    struct hash *h;
    struct pair *p;

    h = (struct hash *)hash;

    for (p = h->table[hash_func(h, key)]; p != NULL; p = p->next) {
        if (strcmp(p->key, key) == 0)
            return p->val;
    }

    return HASH_NOVAL;
}

static void err_exit(const char *str)
{
    perror(str);
    exit(errno);
}

static size_t hash_func(struct hash *hash, const char *key)
{
    size_t result = 0;
    const unsigned char *p = (const unsigned char *)key;

    while (*p != '\0')
        result = MULTIPLIER * result + *p++;

    return result % hash->table_size;
}

static void *make_pair(struct hash* hash, const char *key, const void *value)
{
    struct pair *p;

    if ( (p = (struct pair*)malloc(sizeof(struct pair))) == NULL)
        err_exit("cannnot allocate memory.");
    if ( (p->key = (char *)malloc(sizeof(char) * (strlen(key) + 1))) == NULL)
        err_exit("cannnot allocate memory.");
    if ( (p->val = malloc(hash->value_size)) == NULL)
        err_exit("cannnot allocate memory.");

    strcpy(p->key, key);
    memcpy(p->val, value, hash->value_size);
    p->next = NULL;

    return p;
}
                
static void free_pair(struct pair *pair)
{
    free(pair->key);
    pair->key = NULL;
    
    free(pair->val);
    pair->val = NULL;

    free(pair);
}

struct similarity_hash *f_alloc_similarity_hash(int num, struct similarity_hash *sim_hash){
        if((sim_hash = malloc(sizeof(struct similarity_hash)*num)) == NULL){
                fprintf(stderr,"[E] failed malloc().\n");
                exit(1);
        }
        return(sim_hash);
}

int read_simmilarity_hash_from_stream(int num, FILE *IN, struct similarity_hash *sim_hash){
        int i = 0;
        char line[TMP_STR_LEN_LONG];
        while(line,TMP_STR_LEN_LONG,IN){
                sscanf(line,"%d %d %f",&(*(sim_hash+i)).node_s,&(*(sim_hash+i)).node_e,&(*(sim_hash+i)).sim);
                i++;
        }
        return(i);
}

