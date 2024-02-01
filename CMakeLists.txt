cmake_minimum_required(VERSION 3.26)
project(ZST_DME)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
add_executable(ZST_DME main.cpp
        util.cpp
        util.h
        trr.h
        trr.cpp
        toptodown.cpp
        toptodown.h
        ElmoreDelay.cpp
        ElmoreDelay.cpp
        TopoOperation.cpp
        TopoOperation.h
        Clock.cpp
        Clock.h
        FileOperation.cpp
        FileOperation.h
)
target_link_libraries(ZST_DME PRIVATE nlohmann_json::nlohmann_json)