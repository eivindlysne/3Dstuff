#include <stdio.h>

#include "hash_map.h"


HashMap* HashMap_init() {

    HashMap* map = malloc(sizeof(HashMap));
    map->h = kh_init(str);
    return map;
}

void HashMap_destroy(HashMap* map) {

    for (khint_t k = kh_begin(map->h); k != kh_end(map->h); k++) {
        if (kh_exist(map->h, k)) {
            free((char*) kh_key(map->h, k));
        }
    }

    kh_destroy(str, map->h);
    free(map);
}

void HashMap_put(HashMap* map, bstring key, int value) {

    int ret;
    khint_t k = kh_put(str, map->h, bdata(key), &ret);

    if (!ret) {
        printf("Failed to insert: %s (%d)\n", bdata(key), value);
        kh_del(str, map->h, k);
        return;
    }

    kh_key(map->h, k) = bdata(key);
    kh_value(map->h, k) = value;
}

bool HashMap_exists(HashMap* map, bstring key) {

    khint_t k = kh_get(str, map->h, bdata(key));
    return kh_exist(map->h, k);
}

int HashMap_remove(HashMap* map, bstring key) {

    int value = -1;
    khint_t k = kh_get(str, map->h, bdata(key));
    if (kh_exist(map->h, k)) {
        value = kh_value(map->h, k);
        kh_del(str, map->h, k);
    }
    return value;
}

int HashMap_get(HashMap* map, bstring key) {

    int value = -1;
    khint_t k = kh_get(str, map->h, bdata(key));
    if (kh_exist(map->h, k)) {
        value = kh_value(map->h, k);
    }
    return value;
}

void HasMap_test() {
    HashMap* m = HashMap_init();

    bstring b1 = bfromcstr("1");
    bstring b2 = bfromcstr("2");
    bstring b3 = bfromcstr("3");
    bstring b4 = bfromcstr("4");
    bstring b5 = bfromcstr("5");
    bstring b6 = bfromcstr("6");
    bstring b7 = bfromcstr("7");

    HashMap_put(m, b1, 1);
    HashMap_put(m, b2, 2);
    HashMap_put(m, b3, 3);
    HashMap_put(m, b4, 4);
    HashMap_put(m, b5, 5);
    HashMap_put(m, b6, 6);

    printf("=== HashMap exists ===\n");
    printf("%d: %s\n", 1, HashMap_exists(m, b1) ? "true" : "false");
    printf("%d: %s\n", 2, HashMap_exists(m, b2) ? "true" : "false");
    printf("%d: %s\n", 3, HashMap_exists(m, b3) ? "true" : "false");
    printf("%d: %s\n", 4, HashMap_exists(m, b4) ? "true" : "false");
    printf("%d: %s\n", 5, HashMap_exists(m, b5) ? "true" : "false");
    printf("%d: %s\n", 6, HashMap_exists(m, b6) ? "true" : "false");
    printf("%d: %s\n", 7, HashMap_exists(m, b7) ? "true" : "false");

    printf("\n=== Remove 6 ===\n");
    int val = HashMap_remove(m, b6);
    printf("%d == %d\n", val, 6);
    printf("%d: %s\n", 6, HashMap_exists(m, b6) ? "true" : "false");

    printf("\n=== Get 4 ===\n");
    val = HashMap_get(m, b4);
    printf("%d == %d\n", val, 4);
    printf("%d: %s\n", 4, HashMap_exists(m, b4) ? "true" : "false");

    HashMap_destroy(m);
}
