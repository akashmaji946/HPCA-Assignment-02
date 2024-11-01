import time
import sys
import os

MB_2 = 2*1024*1024
REGIONS_COUNTER = {}

def write_list_to_file(file_path, items):
    try:
        with open(file_path, 'w') as file:
            for item in items:
                file.write(f"{item[0]}\n")
    except Exception as e:
        print(f"An error occurred: {e}")



def get_useful_lines_and_obtain_list(large_pages, filename, outfile):
    print("The filename is:", filename)
    time.sleep(1)
    try:
        with open(filename, 'r') as file:
            for line in file:
                if line.startswith('#'):
                    pass
                    # print(line.strip()
                else:
                    row = (line.strip().split())
                    if row[6] == "work_run":
                        try:
                            hexa = row[9][2:]
                            deci = int(row[9][2:], 16)
                            base = int(row[9][2:], 16)//MB_2 * MB_2
                            print(row)
                            print(hexa, deci, base)
                            if base in REGIONS_COUNTER:
                                if row[-5] == "miss":
                                    REGIONS_COUNTER[base] += 1
                            else:
                                REGIONS_COUNTER[base] = 1
                        except Exception as e:
                            print(f"Convert error: {e}")


    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

    print("___________________________________________________")
    regions = []
    print("@MB Region Bases : TLB misses Count")
    for k in sorted(REGIONS_COUNTER.keys()):
        v = REGIONS_COUNTER[k]
        print(k, v)
        regions.append([k, v])

    sorted_regions = sorted(regions, key = lambda x:x[1], reverse=1)
    print('__________________________________________________________')
    print(len(REGIONS_COUNTER))
    print(sorted_regions[:large_pages])
    write_list_to_file(outfile, sorted_regions[:large_pages])

def execute_commands():
    os.system("gnome-terminal")
    os.system("cd $HOME/Desktop/HPCA-02/")
    os.system("make clean")
    os.system("make")
    os.system('sudo perf mem record ./main 24212')
    os.system("sudo perf mem report > perf_data.txt")

if __name__ == "__main__":
    n = len(sys.argv)
    assert(n > 0)
    large_pages = int(sys.argv[1])
    execute_commands()
    get_useful_lines_and_obtain_list(large_pages, "perf_data.txt", "largepages.txt")
