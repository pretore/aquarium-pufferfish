#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <seagrass.h>
#include <triggerfish.h>
#include <seahorse.h>
#include <sea-turtle.h>
#include <pufferfish.h>

#ifdef TEST
#include <test/cmocka.h>
#endif

#define PUFFERFISH_STRING_POOL_ERROR_STRING_NOT_FOUND             (-1)

bool pufferfish_string_pool_init(struct pufferfish_string_pool *const object) {
    if (!object) {
        pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL;
        return false;
    }
    *object = (struct pufferfish_string_pool) {0};
    switch (pthread_rwlock_init(&object->lock, NULL)) {
        default: {
            seagrass_required_true(false);
        }
        case ENOMEM: {
            pufferfish_error =
                    PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
            return false;
        }
        case 0: {
            break;
        }
    }
    seagrass_required_true(seahorse_red_black_tree_map_s_wr_init(
            &object->map));
    return true;
}

bool pufferfish_string_pool_invalidate(
        struct pufferfish_string_pool *const object) {
    if (!object) {
        pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(!pthread_rwlock_destroy(&object->lock));
    seagrass_required_true(seahorse_red_black_tree_map_s_wr_invalidate(
            &object->map));
    *object = (struct pufferfish_string_pool) {0};
    return true;
}

/**
 * @brief Get matching string strong reference.
 * @param [in] object map instance.
 * @param [in] key of reference to retrieve.
 * @param [out] out receive strong string reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws PUFFERFISH_STRING_POOL_ERROR_STRING_NOT_FOUND if string was not
 * found in string pool.
 * @throws PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to retrieve the strong reference from string pool.
 */
static bool get(const struct seahorse_red_black_tree_map_s_wr *const object,
                const struct sea_turtle_string *const key,
                struct triggerfish_strong **const out) {
    assert(object);
    assert(key);
    assert(out);
    const struct triggerfish_weak *weak;
    if (seahorse_red_black_tree_map_s_wr_get(object, key, &weak)) {
        if (triggerfish_weak_strong(weak, out)) {
            return true;
        }
        seagrass_required_true(TRIGGERFISH_WEAK_ERROR_STRONG_IS_INVALID
                               == triggerfish_error);
    } else if (SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_MEMORY_ALLOCATION_FAILED
               == seahorse_error) {
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    } else {
        seagrass_required_true(
                SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_KEY_NOT_FOUND
                == seahorse_error);
    }
    pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_STRING_NOT_FOUND;
    return false;
}

static void on_destroy(void *a) {
    seagrass_required_true(sea_turtle_string_invalidate(a));
}

/**
 * @brief Add string to string pool.
 * @param [in] object string pool instance.
 * @param [in] string to be added.
 * @param [out] out strong reference of added string.
 * @return On success true, otherwise false if an error has occurred.
 * @throws PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to update the memory pool.
 */
static bool add(struct pufferfish_string_pool *const object,
                const struct sea_turtle_string *const string,
                struct triggerfish_strong **const out) {
    assert(object);
    assert(string);
    assert(out);
    seagrass_required_true(!pthread_rwlock_unlock(&object->lock));
    /* acquire write lock and recheck state */
    seagrass_required_true(!pthread_rwlock_wrlock(&object->lock));
    const struct seahorse_red_black_tree_map_s_wr_entry *entry;
    if (seahorse_red_black_tree_map_s_wr_get_entry(&object->map,
                                                   string,
                                                   &entry)) {
        const struct triggerfish_weak *weak;
        seagrass_required_true(seahorse_red_black_tree_map_s_wr_entry_get_value(
                &object->map, entry, &weak));
        if (triggerfish_weak_strong(weak, out)) {
            return true;
        }
        seagrass_required_true(TRIGGERFISH_WEAK_ERROR_STRONG_IS_INVALID
                               == triggerfish_error);
        seagrass_required_true(seahorse_red_black_tree_map_s_wr_remove_entry(
                &object->map, entry));
    } else if (SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_MEMORY_ALLOCATION_FAILED
               == seahorse_error) {
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    struct sea_turtle_string *key = malloc(sizeof(*key));
    if (!key) {
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    if (!sea_turtle_string_init_string(key, string)) {
        seagrass_required_true(SEA_TURTLE_STRING_ERROR_MEMORY_ALLOCATION_FAILED
                               == sea_turtle_error);
        free(key);
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    struct triggerfish_strong *strong;
    if (!triggerfish_strong_of(key, on_destroy, &strong)) {
        seagrass_required_true(TRIGGERFISH_STRONG_ERROR_MEMORY_ALLOCATION_FAILED
                               == triggerfish_error);
        seagrass_required_true(sea_turtle_string_invalidate(key));
        free(key);
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    struct triggerfish_weak *weak;
    if (!triggerfish_weak_of(strong, &weak)) {
        seagrass_required_true(TRIGGERFISH_WEAK_ERROR_MEMORY_ALLOCATION_FAILED
                               == triggerfish_error);
        seagrass_required_true(triggerfish_strong_release(strong));
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    const bool result = seahorse_red_black_tree_map_s_wr_add(
            &object->map, string, weak);
    if (!result) {
        seagrass_required_true(
                SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_MEMORY_ALLOCATION_FAILED
                == seahorse_error);
        seagrass_required_true(triggerfish_strong_release(strong));
        pufferfish_error =
                PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED;
    } else {
        *out = strong;
    }
    seagrass_required_true(triggerfish_weak_destroy(weak));
    return result;
}

bool pufferfish_string_pool_get(struct pufferfish_string_pool *const object,
                                const struct sea_turtle_string *const string,
                                struct triggerfish_strong **const out) {
    if (!object) {
        pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!string) {
        pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_STRING_IS_NULL;
        return false;
    }
    if (!out) {
        pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_OUT_IS_NULL;
        return false;
    }
    switch (pthread_rwlock_rdlock(&object->lock)) {
        default: {
            seagrass_required_true(false);
        }
        case EAGAIN: {
            pufferfish_error =
                    PUFFERFISH_STRING_POOL_ERROR_CONCURRENCY_LIMIT_REACHED;
            return false;
        }
        case 0: {
            /* fall-through */
        }
    }
    bool result = get(&object->map, string, out);
    if (!result) {
        switch (pufferfish_error) {
            default: {
                seagrass_required_true(false);
            }
            case PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED: {
                break;
            }
            case PUFFERFISH_STRING_POOL_ERROR_STRING_NOT_FOUND: {
                result = add(object, string, out);
            }
        }
    }
    seagrass_required_true(!pthread_rwlock_unlock(&object->lock));
    return result;
}

bool pufferfish_string_pool_shrink(
        struct pufferfish_string_pool *const object) {
    if (!object) {
        pufferfish_error = PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(!pthread_rwlock_wrlock(&object->lock));
    const struct seahorse_red_black_tree_map_s_wr_entry *entry;
    if (seahorse_red_black_tree_map_s_wr_first_entry(&object->map, &entry)) {
        const struct triggerfish_weak *weak;
        do {
            seagrass_required_true(
                    seahorse_red_black_tree_map_s_wr_entry_get_value(
                            &object->map, entry, &weak));
            struct triggerfish_strong *strong;
            if (triggerfish_weak_strong(weak, &strong)) {
                seagrass_required_true(triggerfish_strong_release(strong));
                if (!seahorse_red_black_tree_map_s_wr_next_entry(entry,
                                                                 &entry)) {
                    seagrass_required_true(
                            SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_END_OF_SEQUENCE
                            == seahorse_error);
                    break;
                }
                continue;
            }
            seagrass_required_true(TRIGGERFISH_WEAK_ERROR_STRONG_IS_INVALID
                                   == triggerfish_error);
            const struct seahorse_red_black_tree_map_s_wr_entry *next;
            const bool result = seahorse_red_black_tree_map_s_wr_next_entry(
                    entry, &next);
            if (!result) {
                seagrass_required_true(
                        SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_END_OF_SEQUENCE
                        == seahorse_error);
            }
            seagrass_required_true(
                    seahorse_red_black_tree_map_s_wr_remove_entry(
                            &object->map, entry));
            if (result) {
                entry = next;
            } else {
                break;
            }
        } while (true);
    } else {
        seagrass_required_true(
                SEAHORSE_RED_BLACK_TREE_MAP_S_WR_ERROR_MAP_IS_EMPTY
                == seahorse_error);
    }
    seagrass_required_true(!pthread_rwlock_unlock(&object->lock));
    return true;
}
