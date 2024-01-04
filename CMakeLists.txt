cmake_minimum_required(VERSION 3.25)
project(PEA3)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA3 main.cpp Menu.cpp Menu.h Matrix.cpp Matrix.h Timer.cpp Timer.h FileReader.cpp FileReader.h GeneticAlgorithm.cpp GeneticAlgorithm.h)
