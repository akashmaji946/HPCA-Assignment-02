import time
MB_2 = 2*1024*1024
MY_DICT = {}

def write_list_to_file(file_path, items):
    try:
        with open(file_path, 'w') as file:
            for item in items:
                file.write(f"{item[0]}\n")
    except Exception as e:
        print(f"An error occurred: {e}")



def print_lines_starting_with_hash(filename):
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
                            if base in MY_DICT:
                                MY_DICT[base] += 1
                            else:
                                MY_DICT[base] = 1
                        except Exception as e:
                            print(f"Convert error: {e}")


    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

    print("___________________________________________________")
    regions = []
    
    for k, v in MY_DICT.items():
        print(k, v)
        regions.append([k, v])

    sorted_regions = sorted(regions, key = lambda x:x[1], reverse=1)
    print('__________________________________________________________')
    print(len(MY_DICT))
    print(sorted_regions[:8])
    write_list_to_file('output.txt', sorted_regions[:8])

print_lines_starting_with_hash("perf_1000.txt")