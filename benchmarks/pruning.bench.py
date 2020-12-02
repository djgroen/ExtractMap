############################################################
## @file
## @copyright (C) 2020
##    All rights reserved.
##
## Use, modification, and distribution is subject to the license.
##
## @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
##
############################################################

#!@PYTHON_EXECUTABLE@

import os
import sys
import configparser
import json

binary_folder = r'@CMAKE_CURRENT_BINARY_DIR@'
data_folder   = r'@CMAKE_BINARY_DIR@/data'

template = r'''#!/usr/bin/bash
#PBS -N HiGIS.benchmark
#PBS -l select=1:node_type=rome:ompthreads=128
#PBS -l walltime=00:20:00

# Change to the direcotry that the job was submitted from
cd $PBS_O_WORKDIR

# Auto-generated script for running benchmarks
{}
'''

def main():
    if len(sys.argv) != 2:
        print('Usage:\n    $ {} config-file'.format(sys.argv[0]))
        exit(-1)
    config = configparser.ConfigParser()
    config.read(sys.argv[1])
    test_data_folder = config.get('pruning', 'data_folder', fallback=data_folder)
    bench_calls = []
    for region_full in json.loads(config.get('pruning', 'regions', fallback="[]")):
        region = region_full.split('/')[-1]
        input_csv = r'{}-locations.csv'.format(region)
        input_osrm = r'{}.osrm'.format(region)
        bench_calls.append(r'{binary} {osrm} {csv}'.format(binary=os.path.join(binary_folder,'bench-pruning'),
                                                           csv=os.path.join(test_data_folder, input_csv),
                                                           osrm=os.path.join(test_data_folder, input_osrm),))
    with open(os.path.join(binary_folder, 'pruning.bench.sh'), 'w') as bench_script:
        bench_script.write(template.format('\n'.join(bench_calls)))

if __name__ == "__main__":
    main()
