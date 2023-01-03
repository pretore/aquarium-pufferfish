#ifndef _PUFFERFISH_TEST_CMOCKA_H_
#define _PUFFERFISH_TEST_CMOCKA_H_

#ifdef TEST

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

/* aquarium-seagrass */
extern void *cmocka_test_malloc(size_t size, const char *file, int line);
#define malloc(size) cmocka_test_malloc(size, __FILE__, __LINE__)
extern bool malloc_is_overridden;

extern void *cmocka_test_calloc(size_t nmemb, size_t size, const char *file,
                                int line);
#define calloc(nmemb, size) cmocka_test_calloc(nmemb, size, __FILE__, __LINE__)
extern bool calloc_is_overridden;

extern void *cmocka_test_realloc(void *ptr, size_t size, const char *file,
                                 int line);
#define realloc(ptr, size) cmocka_test_realloc(ptr, size, __FILE__, __LINE__)
extern bool realloc_is_overridden;

extern void cmocka_test_free(void *ptr, const char *file, int line);
#define free(ptr) cmocka_test_free(ptr, __FILE__, __LINE__)

extern void cmocka_test_abort(const char *file, int line);
#define abort() cmocka_test_abort(__FILE__, __LINE__)
extern bool abort_is_overridden;

/* aquarium-coral */
extern int cmocka_test_posix_memalign(void **out, size_t alignment,
                                      size_t size, const char *file, int line);
#define posix_memalign(out, alignment, size) \
    cmocka_test_posix_memalign(out, alignment, size, __FILE__, __LINE__)
extern bool posix_memalign_is_overridden;

/* aquarium-triggerfish */
extern int cmocka_test_pthread_mutex_init(
        pthread_mutex_t *restrict mutex,
        const pthread_mutexattr_t *restrict attr,
        const char *file, int line);
#define pthread_mutex_init(mutex, attr) \
    cmocka_test_pthread_mutex_init(mutex, attr, __FILE__, __LINE__)
extern bool pthread_mutex_init_is_overridden;

extern int cmocka_test_pthread_mutex_destroy(pthread_mutex_t *mutex,
                                             const char *file, int line);
#define pthread_mutex_destroy(mutex) \
    cmocka_test_pthread_mutex_destroy(mutex, __FILE__, __LINE__)
extern bool pthread_mutex_destroy_is_overridden;

extern int cmocka_test_pthread_mutex_lock(pthread_mutex_t *mutex,
                                          const char *file, int line);
#define pthread_mutex_lock(mutex) \
    cmocka_test_pthread_mutex_lock(mutex, __FILE__, __LINE__)
extern bool pthread_mutex_lock_is_overridden;

extern int cmocka_test_pthread_mutex_trylock(pthread_mutex_t *mutex,
                                             const char *file, int line);
#define pthread_mutex_trylock(mutex) \
    cmocka_test_pthread_mutex_trylock(mutex, __FILE__, __LINE__)
extern bool pthread_mutex_trylock_is_overridden;

extern int cmocka_test_pthread_mutex_unlock(pthread_mutex_t *mutex,
                                            const char *file, int line);
#define pthread_mutex_unlock(mutex) \
    cmocka_test_pthread_mutex_unlock(mutex, __FILE__, __LINE__)
extern bool pthread_mutex_unlock_is_overridden;

extern int cmocka_test_pthread_rwlock_init(
        pthread_rwlock_t *restrict rwlock,
        const pthread_rwlockattr_t *restrict attr,
        const char *file, int line);
#define pthread_rwlock_init(rwlock, attr) \
    cmocka_test_pthread_rwlock_init(rwlock, attr, __FILE__, __LINE__)
extern bool pthread_rwlock_init_is_overridden;

extern int cmocka_test_pthread_rwlock_destroy(pthread_rwlock_t *rwlock,
                                              const char *file, int line);
#define pthread_rwlock_destroy(rwlock) \
    cmocka_test_pthread_rwlock_destroy(rwlock, __FILE__, __LINE__)
extern bool pthread_rwlock_destroy_is_overridden;

extern int cmocka_test_pthread_rwlock_rdlock(pthread_rwlock_t *rwlock,
                                             const char *file, int line);
#define pthread_rwlock_rdlock(rwlock) \
    cmocka_test_pthread_rwlock_rdlock(rwlock, __FILE__, __LINE__)
extern bool pthread_rwlock_rdlock_is_overridden;

extern int cmocka_test_pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock,
                                                const char *file, int line);
#define pthread_rwlock_tryrdlock(rwlock) \
    cmocka_test_pthread_rwlock_tryrdlock(rwlock, __FILE__, __LINE__)
extern bool pthread_rwlock_tryrdlock_is_overridden;

extern int cmocka_test_pthread_rwlock_wrlock(pthread_rwlock_t *rwlock,
                                             const char *file, int line);
#define pthread_rwlock_wrlock(rwlock) \
    cmocka_test_pthread_rwlock_wrlock(rwlock, __FILE__, __LINE__)
extern bool pthread_rwlock_wrlock_is_overridden;

extern int cmocka_test_pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock,
                                                const char *file, int line);
#define pthread_rwlock_trywrlock(rwlock) \
    cmocka_test_pthread_rwlock_trywrlock(rwlock, __FILE__, __LINE__)
extern bool pthread_rwlock_trywrlock_is_overridden;

extern int cmocka_test_pthread_rwlock_unlock(pthread_rwlock_t *rwlock,
                                             const char *file, int line);
#define pthread_rwlock_unlock(rwlock) \
    cmocka_test_pthread_rwlock_unlock(rwlock, __FILE__, __LINE__)
extern bool pthread_rwlock_unlock_is_overridden;

#endif //TEST

#endif /* _PUFFERFISH_TEST_CMOCKA_H_ */
