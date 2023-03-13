#include "hash_map.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "ope_string.h"
#include "tools.h"

struct pair_list
{
    char *key;
    void *value;
    struct pair_list *next;
};

struct hash_map *hash_map_init(size_t size, void (*destroy_value)(void *))
{
    if (size == 0)
        errx(3, "hash_map init : could not have a hash_map of size 0\n");

    struct hash_map *hp = calloc(1, sizeof(struct hash_map));
    if (!hp)
        errx(3, "hash_map init : not enough memory (hash_map)\n");

    hp->data = calloc(size, sizeof(struct pail_list *));
    if (!hp->data)
        errx(3, "hash_map init : not enough memory (data)");
    hp->size = size;

    hp->destroy_value = destroy_value;

    return hp;
}

void hash_map_insert(struct hash_map *hash_map, const char *key, void *value)
{
    if (!hash_map)
        errx(3, "hash_map insert : hash_map is null\n");

    size_t h = hash(key) % hash_map->size;

    // search for update
    struct pair_list *pl = hash_map->data[h];
    for (; pl; pl = pl->next)
    {
        if (strcmp(pl->key, key) == 0)
        {
            hash_map->destroy_value(pl->value);
            pl->value = value;
            return;
        }
    }

    // create because not found
    struct pair_list *new = calloc(1, sizeof(struct pair_list));
    if (!new)
        errx(3, "hash_map insert : not enough memory (pair list)\n");

    // init var
    new->key = strdup(key);
    new->value = value;

    // insert
    new->next = hash_map->data[h];
    hash_map->data[h] = new;
}

const void *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (!hash_map)
        errx(3, "hash_map get : hash_map should not be null\n");

    if (!key)
        return NULL;

    size_t h = hash(key) % hash_map->size;
    for (struct pair_list *pl = hash_map->data[h]; pl; pl = pl->next)
    {
        if (strcmp(pl->key, key) == 0)
            return pl->value;
    }

    return NULL;
}

static void pl_destroy(struct pair_list *pl, void (*destroy_value)(void *))
{
    free(pl->key);
    destroy_value(pl->value);
    free(pl);
}

void hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (!hash_map)
        errx(3, "hash_map get : hash_map should not be null\n");

    size_t h = hash(key) % hash_map->size;
    struct pair_list *pl = hash_map->data[h];
    if (!pl)
        return;

    if (strcmp(pl->key, key) == 0)
    {
        hash_map->data[h] = pl->next;
        pl_destroy(pl, hash_map->destroy_value);
        return;
    }

    for (; pl->next; pl = pl->next)
    {
        if (strcmp(pl->key, key) == 0)
        {
            struct pair_list *next = pl->next->next;
            pl_destroy(pl, hash_map->destroy_value);
            pl->next = next;
            return;
        }
    }
}

void hash_map_print(struct hash_map *hash_map, void (*print_value)(void *))
{
    printf("hash_map : ");
    printf("{ ");
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *pl = hash_map->data[i];
        for (; pl; pl = pl->next)
        {
            printf("<%s=", pl->key);
            print_value(pl->value);
            printf("> ");
        }
    }
    printf("}\n");
}

void hash_map_destroy(struct hash_map *hash_map)
{
    if (!hash_map)
        errx(3, "hash_map_destroy : hash_map should no be null\n");

    for (size_t i = 0; i < hash_map->size; i++)
    {
        for (struct pair_list *pl = hash_map->data[i]; pl;)
        {
            struct pair_list *next = pl->next;
            pl_destroy(pl, hash_map->destroy_value);
            pl = next;
        }
    }

    free(hash_map->data);
    free(hash_map);
}

static char **sort_array(char **keys, size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - i - 1; j++)
        {
            if (strcmp(keys[j], keys[j + 1]) > 0)
            {
                char *tmp = keys[j];
                keys[j] = keys[j + 1];
                keys[j + 1] = tmp;
            }
        }
    }

    return keys;
}

static void destroy_nothing(void *value)
{
    (void)value;
}

struct list *hash_map_sort(struct hash_map *hm)
{
    struct list *list_keys = list_init();

    for (size_t i = 0; i < hm->size; i++)
    {
        struct pair_list *pl = hm->data[i];
        while (pl)
        {
            list_add(list_keys, strdup((void *)pl->key));
            pl = pl->next;
        }
    }

    size_t nb_keys = list_size(list_keys);
    char **keys = list_to_nt_array(list_keys, 0);
    list_destroy(list_keys, destroy_nothing);

    keys = sort_array(keys, nb_keys);
    struct list *res = arr_to_list(keys, nb_keys);
    free(keys);
    return res;
}
