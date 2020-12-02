# Installation

Ubuntu LTS 16.04 64-bit is the recommended platform.

## Requirements

* OS with 64-bit or 32-bit architecture
* **C++14** toolchain (GCC >= 5.4, Clang >= 3.8, ICC >= 17)
* CMake >= 3.5
* Git
* third parties:

  * Boost libraries:

    - optional: program_options;

  * [OSRM](https://github.com/Project-OSRM/osrm-backend) as a default routing engine;
  * [PROJ](https://proj.org/) for generic coordinate transformation;
  * optional: [libOsmium](https://github.com/osmcode/libosmium) for working with OpenStreetMap data;
  * optional: [Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser).

For more details, scroll down to find how to setup a specific Linux distribution.

## Instructions

### Installing from sources

#### Local workstation

##### Configuring

- Configure sources

```bash
mkdir ./extr_map-build && cd ./extr_map-build && cmake ../ExtractMap
```

- Adjusting the configuration (if needed)

  For this, you need to edit the file `./extr_map-build/CMakeCache.txt` using your favourite text editor. That file was automatically created by the prior invocation of `cmake`.

##### Compiling and installing

- Run `make` from the `../extr_map-build/` folder to compile sources

```bash
cd ./extr_map-build && make
```
- In order to run test use the following command
```bash
make && make south-sudan-osrm test
```
- Run `make install` to install `ExtractMap`

```bash
cd ./extr_map-build && make install
```
- you can also run tests with verbose output to trace testing errors
```bash
ctest -VV
```

<!-- - https://github.com/osmcode -->
<!-- FLOSS -->
