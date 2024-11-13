#include "libmemcached/memcached.h"

void do_something(memcached_st *ptr) {
    memcached_return_t rc;

    // kv set
    rc = memcached_set(ptr, "test:kv01", strlen("test:kv01"),
                       "value01", strlen("value01"),
                       60 /* expiration  */, 0 /* flags */);
    if (memcached_failed(rc)) {
        fprintf(stderr, "memcached_set() failed, reason(%d)=%s\n", rc, memcached_detail_error_message(ptr, rc));
        return;
    }

    // kv get
    size_t value_length;
    uint32_t flags;
    char *value = memcached_get(ptr, "test:kv01", strlen("test:kv01"),
                                &value_length, &flags, &rc);
    if (!value) {
        fprintf(stderr, "memcached_get() failed, reason(%d)=%s\n", rc, memcached_detail_error_message(ptr, rc));
        return;
    }
    fprintf(stdout, "VALUE %d %d %s\n", flags, value_length, value);
    free(value);
}

int main() {
    memcached_return_t rc;
    memcached_st *memc = memcached_create(NULL);
    // memcached_set_sasl_auth_data(memc, "username", "password");
#ifdef ENABLE_ARCUS
    arcus_return_t a_rc = arcus_connect(memc, "127.0.0.1:2181", "test");
    if (memcached_failed(a_rc)) {
        fprintf(stderr, "arcus_connect() failed, reason=%s\n", arcus_strerror(a_rc));
        exit(EXIT_FAILURE);
    }
#else
    memcached_server_list_st servers = NULL;
    servers = memcached_server_list_append(servers, "127.0.0.1", 11211, &rc);
    if (memcached_failed(rc)) {
        fprintf(stderr, "memcached_server_list_append() failed, reason=%s\n", memcached_detail_error_message(memc, rc));
        exit(EXIT_FAILURE);
    }
    rc = memcached_server_push(memc, servers);
    if (memcached_failed(rc)) {
        fprintf(stderr, "memcached_server_push() failed, reason=%s\n", memcached_detail_error_message(memc, rc));
        exit(EXIT_FAILURE);
    }
#endif
    printf("\n\n{{{\n");

    do_something(memc);

    printf("\n}}}\n\n");
    memcached_free(memc);

    return EXIT_SUCCESS;
}