//
// Created by adamc on 02.01.2024.
//

#ifndef PEA3_GENETICALGORITHM_H
#define PEA3_GENETICALGORITHM_H


#include "Timer.h"
#include "Matrix.h"
#include <vector>
#include <list>
#include <bits/stdc++.h>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int stopCriteria, Matrix*, Timer, double mutationRate, int mutationMethod,
                     int initialPopulationSize, double crossoverRate, int crossoverMethod);

    Matrix* matrix;
    Timer timer;
    std::vector<int> bestSolution;
    int bestObjectiveFunction;
    double whenFound;

    std::vector<std::pair<int, std::vector<int>>> population; //pair: fitness function + path
    std::vector<std::pair<int, std::vector<int>>> children; //pair: fitness function + path

    int stopCriteria;
    int initialPopulationSize;

    double mutationRate;
    int mutationMethod;


    double crossoverRate;
    int crossoverMethod;
    void PMX(std::vector<int>, std::vector<int>);

    void launch();
    void transpositionMutation(std::vector<int>&);
    void inversionMutation(std::vector<int>&);

    std::vector<int> generateBegSolutionRandom();
    std::list<std::pair<double,int>> save;
    int calculateRoute(std::vector<int> track);
    void printSolution();


};


#endif //PEA3_GENETICALGORITHM_H
