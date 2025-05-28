// cpu_bound.c
#include "types.h"
#include "user.h"

int main() {
    while (1) {
        // Intensive computation (e.g., Fibonacci)
        int a = 0, b = 1, c;
        for (int i = 0; i < 100000; i++) {
            c = a + b;
            a = b;
            b = c;
        }
    }
    exit();
}
