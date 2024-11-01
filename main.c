/*
Author: Akash Maji
Contact: akashmaji@iisc.ac.in
*/

#include "work.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

// 2MB large page size
#define PAGE_SIZE (2 * 1024 * 1024) 
// The address space has a maximum of 512 2MB regions
#define MAX_LARGE_PAGES 512
// The number of large pages touched by our main program
int NUM_LARGE_PAGES = 0;
// Type for a 64-bit binary logical address
typedef long long int LOGICAL_ADDRESS;
// Specified base addresses (to be read in from the text file)
LOGICAL_ADDRESS base_addresses[MAX_LARGE_PAGES];
// Array to hold the allocated  2MB logical addresses
void *mmaped_logical_addresses[MAX_LARGE_PAGES];


/* function to allocate memory to N 2MB regions using mmap() */
int allocate_adresses() {
    
    printf("Allocating.....\n");

    for (int i = 0; i < NUM_LARGE_PAGES; i++) {

        // Allocating memory using mmap() syscall at the specified 2MB start address
        mmaped_logical_addresses[i] = mmap((void *)base_addresses[i], PAGE_SIZE, PROT_READ | PROT_WRITE,
                                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_FIXED, -1, 0);

        if (mmaped_logical_addresses[i] == MAP_FAILED) {
            fprintf(stderr, "mmap() failed to allocate");
            exit(EXIT_FAILURE);
        }

        printf("     Allocated 2MB page %d at address %p\n", i, mmaped_logical_addresses[i]);
    }

    printf("Allocated.....\n");

    return 0;

}

/* function to deallocate memory from N 2MB regions using munmap() */
int deallocate_addresses(){

    printf("Dellocating.....\n");

    // Sanity Cleaning the allocatwed 2MB large pages
    for (int i = 0; i < NUM_LARGE_PAGES; i++) {

        if (munmap(mmaped_logical_addresses[i], PAGE_SIZE) == -1) {
            fprintf(stderr, "munmap() failed to deallocate");
            exit(EXIT_FAILURE);
        }

        printf("     Deallocated 2MB page %d at address %p\n", i, mmaped_logical_addresses[i]);

    }
    printf("Deallocated.....\n");

    return 0;
}

/* function to read in values from a file specifying the decimal adresses and store in array */
int read_regions_from_file(const char *filename,  LOGICAL_ADDRESS base_addresses[]) {

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr,"Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    // Number of adresses read
    int size = 0; 

    printf("Reading.....\n"); 

    while (1) {
        LOGICAL_ADDRESS number;
        // Exit loop, if reading number fails
        if (fscanf(file, "%lld", &number) != 1) {
            break; 
        }
        // Store the number and increment size
        (base_addresses)[size++] = number;
    }

     printf("Read.....\n");

    fclose(file);
    return size;

}

/* Driver Code */
int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: main <last 5 digits of your reg. no>\n");
    return EXIT_FAILURE;
  }
  work_init(atoi(argv[1]));

  // Put your changes here
  NUM_LARGE_PAGES = read_regions_from_file("largepages.txt", base_addresses);
  allocate_adresses();

  if (work_run() == 0) {
    printf("Work completed successfully\n");
  }

  deallocate_addresses();

  // printf("Hello! Akash Maji - 24212\n");

  return 0;
}

/*
Commands:
sudo sysctl vm.nr_hugepages=1024
sudo sysctl kernel.perf_event_paranoid=-1

-------------------------------
cd ~/Desktop/hpca
ls
make clean
make

sudo perf mem record ./main 24212
sudo perf mem report > perf.txt

sudo perf stat -e\
dTLB-loads,\
dTLB-load-misses,\
dTLB-stores,\
dTLB-store-misses \
./main 24212

*/
/*

 Performance counter stats for './main 24212':

    13,013,995,269      dTLB-loads                                                            
     6,866,430,671      dTLB-load-misses                 #   52.76% of all dTLB cache accesses
         8,594,696      dTLB-stores                                                           
            51,916      dTLB-store-misses                                                     

      30.001309407 seconds time elapsed

      30.000342000 seconds user
       0.000000000 seconds sys


 Performance counter stats for './main 24212':

    13,010,455,841      dTLB-loads                                                            
     4,689,985,549      dTLB-load-misses                 #   36.05% of all dTLB cache accesses
         6,330,210      dTLB-stores                                                           
            36,138      dTLB-store-misses                                                     

      20.449348980 seconds time elapsed

      20.448968000 seconds user
       0.000000000 seconds sys

*/
