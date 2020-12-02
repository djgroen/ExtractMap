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

. ~/.bashrc
module load osrm
GEO_REGION=${test_geo_region:-europe}
OSRM_METHOD=${osrm_method:-CH}
WORK_DIR=`ws_find ${WS_OSM_MAPS:-higis_maps}`/${GEO_REGION}

cd ${WORK_DIR}
echo "Run for ${GEO_REGION} at ${WORK_DIR}"
osrm-extract -p ${OSRM_DIR}/share/osrm/profiles/car.lua ./${test_geo_region}.osm.pbf
# osrm-partition ./${test_geo_region}.osrm
# osrm-customize ./${test_geo_region}.osrm
osrm-contract ./${test_geo_region}.osrm
