// io_bound.c
#include "types.h"
#include "user.h"

int main() {
    while (1) {
        sleep(10);  // Simulate I/O wait
        // Short computation burst
        for (int i = 0; i < 100; i++) {}
    }
    exit();
}
