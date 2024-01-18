//
// Created by adamc on 12/12/2023.
//

#include <fstream>
#include <cstring>
#include <iostream>
#include "GeneticAlgorithm.h"

class FileWriter {
public:
    FileWriter();
    std::string filename;
    std::string filenameOfBestSolution;
    int initialPopulationSize;
    int bestObjectiveFunction;
    int mutationMethod;

    void write(GeneticAlgorithm*, int);
};