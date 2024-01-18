cmake_minimum_required(VERSION 3.26)
project(ZST_DME)

set(CMAKE_CXX_STANDARD 17)

add_executable(ZST_DME main.cpp
        util.cpp
        util.h
        topoparser.cpp
        topoparser.h
        reader.cpp
        reader.h
        netlist.cpp
        netlist.h
        trr.h
        trr.cpp
        toptodown.cpp
        toptodown.h
        ElmoreDelay.cpp
        ElmoreDelay.cpp
        TopoOperation.cpp
        TopoOperation.h
        Test.cpp
        Test.h
)
