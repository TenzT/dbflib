cmake_minimum_required(VERSION 3.28)
project(dbf)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -O3)

include_directories(include)

add_library(dbf SHARED src/DBaseColDef.cpp
            src/DBaseField.cpp
            src/DBaseFieldProperty.cpp
            src/DBaseFile.cpp
            src/DBaseHeader.cpp
            src/DBaseRecord.cpp            
)