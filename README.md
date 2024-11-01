### Assignment 02
We are using the perf mem tool to obtain a sampled report of TLB misses occurring at different logical addresses. We then compute the top N large page regions where the number of TLB misses is higher. Then we allocate N large pages to these 2MB regions, and see if any improvement is seen or not. perf is a powerful performance analysis tool for Linux that provides various commands to monitor and analyze system and application performance.

We are using a linux system, with necessary tools installed and proper configurations made.
#### Install perf:
```bash
sudo apt-get install linux-tools-common linux-tools-generic linux-tools-`uname -r`
Configure kernel settings and huge pages:
sudo sysctl vm.nr_hugepages=1024
sudo sysctl kernel.perf_event_paranoid=-1
```
#### Clean build the files:
```bash
make clean
make
```
#### Collect the report:
```bash
sudo perf mem record ./main 24212
sudo perf mem report > perf.txt
```

### How to analyze collected sampled data?
Once the perf.txt file is available, we can analyze it using a Python program to figure out how many TLB misses are occurring in each of the 2MB regions accessed by the program. 
Our Python script (analyse.py) takes in an argument (N) and produces a list of top N addresses in decimal in a text file largepages.txt. Internally, we obtain a sorted list of <2MB Region, TLB Misses> pairs, and then obtain the top N suitable 2MB base regions that we should target to allocate N 2MB large pages. This can be done using mmap() syscall in the modified main program. The Python script will produce a text file of those top N 2MB regions, specifying addresses in decimal. The modified main program will read in these addresses from the text file , and allocate large pages dynamically. We can then rerun the commands in step4, and obtain the new perf.txt report. We can analyze this file to see reductions in TLB misses. 

We can additionally use the inbuilt tool “perf stat” to see the improvements in these parameters:
                       DTLB loads, DTLB load misses, DTLB stores, DTLB store misses
We run this command before and after making changes to main program:
```bash
make clean
make
sudo perf stat -e dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses ./main 24212
```

##### Author: Akash Maji
##### Contact: akashmaji@iisc.ac.in
