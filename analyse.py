#
# Author: Akash Maji
# Contact: akashmaji@iisc.ac.in
#

import os, sys, time, re

class FindOptimal2MBRegions(object):

    def __init__(self, num_large_pages, to_print=False):
        assert(num_large_pages > 0)
        self.num_large_pages = num_large_pages
        self.REGION_TO_MISSES = {}
        self.MB_2 = 2*1024*1024
        self.to_print = to_print

    def printer(self, *objects, sep=' ', end='\n', file=sys.stdout, flush=False):
        if self.to_print:
            print(*objects, sep, end, file, flush)
    

    def write_list_to_file(self, file_path, items):
        """
        function to read in a file at `file_path` and write the values in items to it
        """
        try:
            with open(file_path, 'w') as file:
                for item in items:
                    file.write(f"{item[0]}\n")
        except Exception as e:
            print(f"An error occurred: {e}")
    
    def run_terminal_commands(self):
        """
        function to run few terminal commands for building main program, running it, recording events,
        and reporting out a `perf.txt` file
        """
        # os.system("gnome-terminal")
        # os.system("cd $PWD")
        os.system("make clean")
        os.system("make")
        os.system('sudo perf mem record -c 100 ./main 24212')
        os.system("sudo perf mem report > perf.txt")

    def write_list_to_file_again(self, file_path, items):
        """
        function similar to above but writes more things
        """
        try:
            with open(file_path, 'a') as file:
                for item in items:
                    file.write(f"{item[0]} {item[1]}\n")
                file.write("---------------------------------\n")
        except Exception as e:
            print(f"An error occurred: {e}")
        
    def write_list_to_file_again(self, file_path, items):
        """
        function similar to above but writes more things
        """
        try:
            with open(file_path, 'a') as file:
                for item in items:
                    file.write(f"{item[0]} {item[1]}\n")
                file.write("---------------------------------")
        except Exception as e:
            print(f"An error occurred: {e}")


    def obtain_n_most_suitable_regions(self, filename, outfile):
        """
        function to identify the most suitable `large_pages` number of 2MB regions
        as per the text report at `filename` and obtain an output `outfile` with the
        addresses (in decimal) of those 2MB base regions
        """
        self.printer("The filename is:", filename)
        time.sleep(1)
        try:
            lineno = 0
            with open(filename, 'r') as file:
                for line in file:
                    if line.startswith('#'):
                        pass
                    else:
                        row = (line.strip().split())
                        pattern = r'^0x'
                        hexas = [s for s in row if re.match(pattern, s)]
                        if hexas != [] and row != [] and "work_run" in row:
                            try:
                                # time.sleep(1)
                                # self.printer(hexas, hexas[0])
                                hexa = hexas[0][2:]
                                deci = int(hexa, 16)
                                base = deci //self.MB_2 * self.MB_2
                                self.printer(lineno, row)
                                lineno += 1
                                self.printer(hexa, deci, base)
                                if base in self.REGION_TO_MISSES:
                                    if row[-5] == "miss":
                                        self.REGION_TO_MISSES[base] += 1
                                else:
                                    if row[-5] == "miss":
                                        self.REGION_TO_MISSES[base] = 1
                                    else:
                                        self.REGION_TO_MISSES[base] = 0
                            except Exception as e:
                                print(f"Convert error: {e}")


        except FileNotFoundError:
            print(f"Error: The file '{filename}' was not found.")
        except Exception as e:
            print(f"An error occurred: {e}")

        self.printer("___________________________________________________")
        self.printer("2MB Region to TLB Misses")
        self.printer("___________________________________________________")

        # regions hold <address, misses> pairs
        regions = []
        for k, v in self.REGION_TO_MISSES.items():
            self.printer(k, v)
            regions.append([k, v])

        # obtain the regions with most misses first
        sorted_regions = sorted(regions, key = lambda x:x[1], reverse=True)
        self.printer('__________________________________________________________')
        self.printer("Total 2MB Regions: ", len(self.REGION_TO_MISSES))
        self.printer('__________________________________________________________')
        self.printer("Top", self.num_large_pages, "2MB regions are:")
        self.printer(sorted_regions[:self.num_large_pages])
        self.printer('__________________________________________________________')
        self.write_list_to_file(outfile, sorted_regions[:self.num_large_pages])
        self.write_list_to_file_again("regions_to_misses.txt", sorted_regions)

    

# usage: python3 analyze.py 10
# where 10 is the argument specifying the number of 2MB regions
if __name__ == "__main__":
    n = len(sys.argv)
    assert(n > 0)
    large_pages = int(sys.argv[1])
    finder = FindOptimal2MBRegions(large_pages, False)
    finder.run_terminal_commands()
    finder.obtain_n_most_suitable_regions("perf.txt", "largepages.txt")