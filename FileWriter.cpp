//
// Created by adamc on 12/12/2023.
//

#include "FileWriter.h"
#include "GeneticAlgorithm.h"

FileWriter::FileWriter(){
    bestObjectiveFunction = INT_MAX;
}

void FileWriter::write(GeneticAlgorithm* geneticAlgorithm, int numberOfTest){

    //saving every improvement of path and when it was improved to file in format: "ftv170.atsp_2500_1_1.txt" (filename_populationSize_mutationMethod_nrOfTest.txt)
    std::string name = filename + "_" + std::to_string(initialPopulationSize) + "_" +
            std::to_string(mutationMethod) + "_" + std::to_string(numberOfTest) + ".txt";
    std::ofstream file(name);

    //<time, path> in every line
    for(auto entry: geneticAlgorithm->save){
        file << entry.first << " " << entry.second << std::endl;
    }

    file.close();

    //if that's the best found route in test, we save that info
    if(geneticAlgorithm->bestObjectiveFunction < bestObjectiveFunction){
        bestObjectiveFunction = geneticAlgorithm->bestObjectiveFunction;
        filenameOfBestSolution = name;
    }

    //saving path in format: "ftv170.atsp_2500_1_1_path.txt"
    name = filename + "_" + std::to_string(initialPopulationSize) + "_" + std::to_string(mutationMethod) + "_" +
            std::to_string(numberOfTest) + "_path" + ".txt";
    std::ofstream file1(name);

    //size of graph
    file1 << geneticAlgorithm->bestSolution.size() << std::endl;

    //path
    for(int town : geneticAlgorithm->bestSolution){
        file1 << town << " ";
    }

    file1.close();

    //if it's last test, we save where the best solution has been found
    if(numberOfTest == 9){
        name = filename + "_" + std::to_string(initialPopulationSize) + "_" + std::to_string(mutationMethod) + "_best" + ".txt";
        std::ofstream file2(name);
        file2<<filenameOfBestSolution;
        file2.close();
    }

}