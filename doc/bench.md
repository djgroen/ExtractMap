# Benchmark

## Downloading OSMs

```sh
wget --no-check-certificate https://download.geofabrik.de/${test_geo_region}-latest.osm.pbf # -o ${test_geo_region}.osm.pbf
mv ./${test_geo_region}-latest.osm.pbf ./${test_geo_region}.osm.pbf
scp ./${test_geo_region}* hawk.hww.hlrs.de:/lustre/cray/ws9/2/ws/hpcgogol-extr_map_maps
```

## Manual extraction of locations

```sh
test_geo_region=europe qsub -V -l select=1:node_type=rome,walltime=8:00:00 ~/hidalgo/ExtractMap/data/scripts/osm-to-osrm.sh
```

## Running 

```sh
test_geo_region=europe qsub -V -l select=1:node_type=rome,walltime=8:00:00 ~/hidalgo/ExtractMap/benchmarks/pruning.bench.any.sh
```
