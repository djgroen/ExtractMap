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
project(ExtractMap)

subdirs(extr_map)

###############################################################################
# Handle warnings as errors in library sources
###############################################################################
add_compile_options(
    -pedantic
    -Wall
    -Werror
)
