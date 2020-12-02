# Data

## Automated preparation of data

Data for the examples, tests, and benchmarks can be automatically downloaded and extracted from OpenStreetMaps with the following command:
```
make data
```

## Downloading OSMs

```sh
wget -O - https://download.geofabrik.de/africa/south-sudan-latest.osm.pbf | ssh remote-host 'cat > ~/hidalgo/extr_map-build/data/south-sudan.osm.pbf'
```

```sh
wget --no-check-certificate https://download.geofabrik.de/${test_geo_region}-latest.osm.pbf # -o ${test_geo_region}.osm.pbf
mv ./${test_geo_region}-latest.osm.pbf ./${test_geo_region}.osm.pbf
scp ./${test_geo_region}* hawk.hww.hlrs.de:/lustre/cray/ws9/2/ws/hpcgogol-extr_map_maps
```

## Manual extraction of locations

Data about locations can be downloadted from the resources like [osmnames](https://osmnames.org/download/)
or extracted from OSM files directly.

### Preparation of data with `osmosis`

In order to use this method, one need to have `osmosis` and `xsltproc` installed on the system.
The latter can be done on Ubuntu with the following command:
```sh
sudo apt install osmosis xsltproc
```

The following code shows how to extract information about settlements (except villages with unknown population)
from OSM file in CSV format:
```sh
osmosis --read-pbf ${test_geo_region}.osm.pbf --tf accept-nodes place=city,town,village \
  --tf reject-relations --tf reject-ways --lp --write-xml ${test_geo_region}-cities.osm
xsltproc ~/Downloads/loc_graph_extract/know_pruning/HidGIS/data/cities.xslt \
  south-sudan-cities.osm | grep -v '"",village'
```
Note that `osmosis` and `xsltproc` can be pipelined
```sh
osmosis --read-pbf south-sudan.osm.pbf --tf accept-nodes place=city,town,village \
  --tf reject-relations --tf reject-ways --lp --wx /dev/stdout | \
  xsltproc ~/Downloads/loc_graph_extract/know_pruning/HidGIS/data/cities.xslt -  |\
  grep -v '"",village'
```
Alternatively, one can use Python script instead of `xsltproc` 
```sh
python3 $ExtractMap/data/scripts/osm-settlements-to-csv.py ./${test_geo_region}-cities.osm
```

### Preparation of data with `osmctools`

TODO: explain preparation of data with [osmctools](https://github.com/ramunasd/osmctools)

```sh
osmconvert south-sudan.osm.pbf -o=south-sudan.o5m
# `osmfilter` does not support stream input
osmfilter ./south-sudan.o5m --keep="place=city or place=town or (place=village and population>0)" --drop-ways -o=./south-sudan-c.osm
osmfilter ./south-sudan.o5m --keep="place=city or place=town or place=village" --drop-ways -o=./south-sudan-c.osm
# `osmfilter` extracts nodes with empty 
osmconvert ./south-sudan-c.osm --csv-headline --csv-separator=',' --csv='@id @lat @lon name place population' |  grep -v ,,, > ./south-sudan-c.csv
osmfilter ./south-sudan.o5m --out-key | grep name:uk
```

## OSRM files 

Preparing OSRM file from PBF
```sh
cd ${test_geo_region}
osrm-extract -p ${OSRM_DIR}/share/osrm/profiles/car.lua ./${test_geo_region}.osm.pbf
osrm-contraction ./${test_geo_region}.osrm
```
One can also use a PBS script to submit OSRM file extraction job:
```sh
test_geo_region=south-sudan qsub -V -l select=1:node_type=rome,walltime=24:00:00 ../ExtractMap/data/scripts/osm-to-osrm.sh
```

# Running benchmark

## HPE Apollo installation at HLRS (Hawk)

```
qsub -l select=1:node_type=rome,walltime=00:20:00 -I
cd $HOME/hidalgo/extr_map-build
```
