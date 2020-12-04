#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
// M: the above line is added to enable file input output

#include <cstdlib>

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " data.osrm\n";
        return EXIT_FAILURE;
    }

    using namespace osrm;

    // Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
    EngineConfig config;

    config.storage_config = {argv[1]};
    config.use_shared_memory = false;

    // We support two routing speed up techniques:
    // - Contraction Hierarchies (CH): requires extract+contract pre-processing
    // - Multi-Level Dijkstra (MLD): requires extract+partition+customize pre-processing
    //
    config.algorithm = EngineConfig::Algorithm::CH;
    // config.algorithm = EngineConfig::Algorithm::MLD;

    // Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
    const OSRM osrm{config};
    
    // The following shows how to use the Route service; configure this service
    // M: configured route to table and input file locations.csv should be located in OSRM folder, not example folder. 
    TableParameters params;
    std::ifstream ip(argv[2]);
    // M: initializing lon, lat, id as string to take from each line, coordinates and index.
    std::string lon;
    std::string lat;
    std::string id;
    std::string name;
    std::string region;
    std::string country;
    std::string location_type;
    std::string conflict_date;
    std::string population;

    if(!ip.is_open()) std::cout << "Error file open" << "\n";
    int i=0;
    while(ip.good()){
	i=i+1;
	std::getline(ip,name,',');
	std::getline(ip,region,',');
        std::getline(ip,country,',');
	std::getline(ip,lat,',');
	std::getline(ip,lon,',');
	std::getline(ip,location_type,',');
	std::getline(ip,conflict_date,',');
	std::getline(ip,population,'\n');
	// M: using location file as documented
	if (lon=="lon"){continue;}
	// M: at each line, lon, lat, id are given values
	std::cout<<lon<<"   ";
	// M: at each line, lon adn lat are converted to float (note that the losing digit problem happened here, the string has full digits but the float are truncated)
	float lonf=std::atof(lon.c_str());
        float latf=std::atof(lat.c_str());
	// M: print out lonf, latf to see they are ready
	std::cout<<lonf<<"   ";
	std::cout<<id<<"   ";
	std::cout<<latf<<"\n";
	// M: push back coordinate pairs to params.coordinates
	params.coordinates.push_back({util::FloatLongitude{lonf}, util::FloatLatitude{latf}});
    }
	// M: Distances means that the program will return distances, it can also be durations, but remember to change below as well.
    params.annotations = TableParameters::AnnotationsType::Distance;

    // Response is in JSON format
    engine::api::ResultT result = json::Object();

    // Execute routing request, this does the heavy lifting
    const auto status = osrm.Table(params, result);
 
    auto &json_result = result.get<json::Object>();

    if (status == Status::Ok)
    {
    // M: open a file for writing, note that the file will be saved in OSRM folder instead of example or build folder, delete all std:cout lines after debugging
    std::ofstream myfile;
    myfile.open("myfileee.csv");
    std::cout << "Table service worked fine probably\n" ;
    // M: the following data reading is iterating the json structure to get the distances, the json structure is described here http://project-osrm.org/docs/v5.22.0/api/#general-options.
    auto &distance_array = json_result.values.at("distances").get<json::Array>().values;
    std::cout << "Table service worked fine probably\n" ;
    
    for (int a=0;a<(i-2);a=a+1){
    	auto &distance_matrix = distance_array[a].get<json::Array>().values;
	std::cout << "Table service worked fine probably\n" ;
	myfile<<"\n";
	std::cout <<"\n";
    	for (int b=0;b<(i-2);b=b+1){
		try{
			auto distance=distance_matrix[b].get<json::Number>().value;	
			myfile << distance<<",";
			std::cout <<distance<<",";
			std::cout <<"a is "<<a<<"\n";
			std::cout <<"b is "<<b<<"\n";
		   }
		catch (mapbox::util::bad_variant_access& e)
// M: seems to me that no connection between two coordinates throws bad variant access error so whenever this happens the distances is written as 1.010101010101 and in command window prints exception handled~~~~~~~
		    {
			auto distance=40000000;
			std::cout<< "exception handled~~~~~~~~~~";
			myfile << distance<<",";
			std::cout <<distance<<",";
			std::cout <<"a is "<<a<<"\n";
			std::cout <<"b is "<<b<<"\n";
		    }

		}
    }

    //std::cout << duration;
    myfile.close();
    }
    
    else if (status == Status::Error)
    {
        const auto code = json_result.values["code"].get<json::String>().value;
        const auto message = json_result.values["message"].get<json::String>().value;

        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";
        return EXIT_FAILURE;
    }
}
