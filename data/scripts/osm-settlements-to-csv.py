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

#!/usr/bin/python3

import pandas
import geopandas
import xml.etree.ElementTree as ET
import sys

def get_locations(filename):
    def iter_locations(nodes):
        for node in nodes:
            location_dict = {'osmid' : node.attrib['id'], 'lon' : float(node.attrib['lon']), 'lat' : float(node.attrib['lat'])}
            for tag_name in ('place', 'population', 'name', 'is_in', 'wikipedia'): # 'is_in:country', 
                tag = node.find(r'tag[@k="{}"]'.format(tag_name))
                location_dict[tag_name] = None if tag is None else tag.attrib['v']
            yield location_dict

    etree = ET.parse(filename) # create an ElementTree object 
    root = etree.getroot()
    locations = pandas.DataFrame(iter_locations(root.findall('node')))
    locations.set_index('osmid', inplace=True)
    # locations[locations.place.isin(['city', 'town'])]
    return locations

def to_gpd(locations):
    def parse_population(x):
        try: return int(x)
        except: return 0
    
    gdf=geopandas.GeoDataFrame(locations, geometry=geopandas.points_from_xy(locations.lon, locations.lat))
    gdf.population = gdf.population.apply(parse_population)
    gdf.sort_values('population', ascending=False, inplace=True)
    return gdf

def plot_gdf(gdf, region='South America')
    world = geopandas.read_file(geopandas.datasets.get_path('naturalearth_lowres'))

    # We restrict to the region.
    ax = world[world.continent == region].plot(
        color='white', edgecolor='black')
    gdf.plot(ax=ax, color='red',marker='.')
    plt.show()

def main():
    if len(sys.argv) < 2:
        sys.stderr.write('Usage:\n    $ {} input-file.osm\n'.format(sys.argv[0]))
        exit(-1)
    locations = get_locations(sys.argv[1])
    locations[locations.place.isin(['city', 'town'])].to_csv(r"{}.csv".format(sys.argv[1]))

if __name__ == "__main__":
    main()
