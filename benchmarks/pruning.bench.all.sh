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

#!/usr/bin/bash

for test_geo_region in central-america south-america australia-oceania north-america central-america asia north-america south-america europe
do
    test_geo_region=$test_geo_region qsub -V -l select=1:node_type=rome,walltime=12:00:00 @HiGIS_SOURCE_DIR@/data/scripts/osm-to-osrm.sh
done
