#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

struct hash_map
{
    struct pair_list **data;
    size_t size;
    void (*destroy_value)(void *);
};

// hash.c
size_t hash(const char *str);

// hash_map.c
struct hash_map *hash_map_init(size_t size, void (*destroy_value)(void *));
void hash_map_insert(struct hash_map *hash_map, const char *key, void *value);
const void *hash_map_get(const struct hash_map *hash_map, const char *key);
void hash_map_remove(struct hash_map *hash_map, const char *key);
void hash_map_print(struct hash_map *hash_map, void (*print_data)(void *value));
void hash_map_destroy(struct hash_map *hash_map);
struct list *hash_map_sort(struct hash_map *hm);

#endif /* !HASH_MAP_H */
