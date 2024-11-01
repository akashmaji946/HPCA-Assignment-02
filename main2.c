#include "work.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define PAGE_SIZE (2 * 1024 * 1024)  // 2MB page size
#define NUM_PAGES 8


int main2() {
    // Specified base addresses
    long long int base_addresses[NUM_PAGES] = {
        1309518331904,
        1309155524608,
        1309931470848,
        1309262479360,
        1309608509440,
        1309331685376,
        1309054861312,
        1309071638528
    };

    // Array to hold the allocated addresses
    void *allocated_addresses[NUM_PAGES];

    for (int i = 0; i < NUM_PAGES; i++) {
        // Allocate memory using mmap at the specified address
        allocated_addresses[i] = mmap((void *)base_addresses[i], PAGE_SIZE, PROT_READ | PROT_WRITE,
                                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);

        if (allocated_addresses[i] == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }

        // Optionally, initialize the allocated memory
        memset(allocated_addresses[i], 0, PAGE_SIZE);
        printf("Allocated page %d at address %p\n", i, allocated_addresses[i]);
    }

    // Clean up: Unmap the allocated memory
    for (int i = 0; i < NUM_PAGES; i++) {
        if (munmap(allocated_addresses[i], PAGE_SIZE) == -1) {
            perror("munmap failed");
        }
    }

    return 0;
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: main <last 5 digits of your reg. no>\n");
    return EXIT_FAILURE;
  }
  work_init(atoi(argv[1]));

  // Put your changes here
  main2();

  if (work_run() == 0) {
    printf("Work completed successfully\n");
  }

  printf("Hello Akash\n");

  return 0;
}