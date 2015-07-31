#ifndef OKAPI_HASH_MAP_H
#define OKAPI_HASH_MAP_H

#include <stdbool.h>

#include "bstrlib/bstrlib.h"
#include "klib/khash.h"
KHASH_MAP_INIT_STR(str, int)

typedef struct {
    khash_t(str)* h;
} HashMap;

HashMap* HashMap_init();

void HashMap_destroy(HashMap* map);

void HashMap_put(HashMap* map, bstring key, int value);

bool HashMap_exists(HashMap* map, bstring key);

int HashMap_remove(HashMap* map, bstring key);

int HashMap_get(HashMap* map, bstring key);

void HasMap_test();

#endif
