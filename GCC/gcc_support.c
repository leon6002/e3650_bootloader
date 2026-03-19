#include <stdint.h>

/* Dummy implementation of _set_tls to satisfy linker */
void _set_tls(void *tls_base) {
    (void)tls_base;
}
