import os
import glob

def count_lines_in_dir(directory, extension):
    total_lines = 0
    for filename in glob.glob(f'{directory}/**/*.{extension}', recursive=True):
        with open(filename, 'r') as f:
            for line in f:
                total_lines += 1
    return total_lines

dir_path = 'src'
total_lines = count_lines_in_dir(dir_path, 'c') + count_lines_in_dir(dir_path, 'h')
print(f'Total lines of code: {total_lines}')