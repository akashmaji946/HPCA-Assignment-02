#include "work.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define PAGE_SIZE (2 * 1024 * 1024)  // 2MB page size
#define NUM_PAGES 8

// Specified base addresses
long long int base_addresses[NUM_PAGES] = {
1309518331904,
1309155524608,
1309356851200,
1309931470848,
1309614800896,
1309054861312,
1309262479360,
1309291839488
};
// Array to hold the allocated addresses
void *allocated_addresses[NUM_PAGES];

int alloc() {
    
    for (int i = 0; i < NUM_PAGES; i++) {
        // Allocate memory using mmap at the specified address
        allocated_addresses[i] = mmap((void *)base_addresses[i], PAGE_SIZE, PROT_READ | PROT_WRITE,
                                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);

        if (allocated_addresses[i] == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }

        // Optionally, initialize the allocated memory
        // memset(allocated_addresses[i], 0, PAGE_SIZE);
        printf("Allocated page %d at address %p\n", i, allocated_addresses[i]);
    }

    return 0;
}

int dealloc(){
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
  alloc();

  if (work_run() == 0) {
    printf("Work completed successfully\n");
  }

  dealloc();

  printf("Hello Akash\n");

  return 0;
}

/*
Commands:
sudo sysctl vm.nr_hugepages=1024
sudo sysctl kernel.perf_event_paranoid=-1

-------------------------------
cd ~/Desktop/hpca
ls
make

sudo perf mem record ./main 24212
sudo perf mem report > perf.txt

sudo perf mem record -c 1000 ./main 24212
sudo perf mem report > perf_1000.txt

sudo perf mem record -c 100 ./main 24212
sudo perf mem report > perf_100.txt


sudo perf stat -e\
L1-dcache-loads,\
L1-dcache-load-misses,\
L1-dcache-stores,\
LLC-loads,\
LLC-load-misses,\
LLC-stores,\
LLC-store-misses \
./main 24212
*/

/*
 Performance counter stats for 'CPU(s) 10':

       207,346,260      L1-dcache-loads                                                         (42.85%)
        18,587,362      L1-dcache-load-misses            #    8.96% of all L1-dcache accesses   (57.14%)
       137,857,784      L1-dcache-stores                                                        (57.14%)
         7,198,589      LLC-loads                                                               (57.14%)
         1,254,792      LLC-load-misses                  #   17.43% of all L1-icache accesses   (57.15%)
         3,372,797      LLC-stores                                                              (28.57%)
         1,876,987      LLC-store-misses                                                        (28.57%)

      29.816626086 seconds time elapsed

 Performance counter stats for 'CPU(s) 10':

        45,933,034      L1-dcache-loads                                                         (42.85%)
         4,816,331      L1-dcache-load-misses            #   10.49% of all L1-dcache accesses   (57.15%)
        32,028,448      L1-dcache-stores                                                        (57.15%)
         2,397,312      LLC-loads                                                               (57.15%)
           210,888      LLC-load-misses                  #    8.80% of all L1-icache accesses   (57.15%)
           571,225      LLC-stores                                                              (28.56%)
           105,171      LLC-store-misses                                                        (28.56%)

      29.827088495 seconds time elapsed



*/