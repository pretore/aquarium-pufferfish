#ifndef _PUFFERFISH_STRING_POOL_H_
#define _PUFFERFISH_STRING_POOL_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <seahorse.h>
#include <pthread.h>

struct sea_turtle_string;
struct triggerfish_strong;

#define PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL                 1
#define PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED       2
#define PUFFERFISH_STRING_POOL_ERROR_STRING_IS_NULL                 3
#define PUFFERFISH_STRING_POOL_ERROR_OUT_IS_NULL                    4
#define PUFFERFISH_STRING_POOL_ERROR_CONCURRENCY_LIMIT_REACHED      5

struct pufferfish_string_pool {
    pthread_rwlock_t lock;
    struct seahorse_red_black_tree_map_s_wr map;
};

/**
 * @brief Initialize string pool.
 * @param [in] object instance to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to initialize string pool.
 */
bool pufferfish_string_pool_init(struct pufferfish_string_pool *object);

/**
 * @brief Invalidate string pool.
 * <p>The actual <u>string pool instance is not deallocated</u> since it may
 * have been embedded in a larger structure.</p>
 * @param [in] object instance to be invalidated.
 * @return On success true, otherwise false if an error has occurred.
 * @throws PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool pufferfish_string_pool_invalidate(struct pufferfish_string_pool *object);

/**
 * @brief Receive the matching strong reference in the string pool.
 * @param [in] object string pool instance.
 * @param [in] string to find in string pool.
 * @param [out] out strong reference of string in pool.
 * @return On success true, otherwise false if an error has occurred.
 * @throws PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws PUFFERFISH_STRING_POOL_ERROR_STRING_IS_NULL if string is <i>NULL</i>.
 * @throws PUFFERFISH_STRING_POOL_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws PUFFERFISH_STRING_POOL_ERROR_CONCURRENCY_LIMIT_REACHED if the
 * maximum number of concurrent operations on this string pool instance has
 * been reached.
 * @throws PUFFERFISH_STRING_POOL_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to retrieve pooled string instance.
 */
bool pufferfish_string_pool_get(struct pufferfish_string_pool *object,
                                const struct sea_turtle_string *string,
                                struct triggerfish_strong **out);

/**
 * @brief Remove all unused entries.
 * @param [in] object string pool instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws PUFFERFISH_STRING_POOL_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool pufferfish_string_pool_shrink(struct pufferfish_string_pool *object);

#endif  /* _PUFFERFISH_STRING_POOL_H_ */
