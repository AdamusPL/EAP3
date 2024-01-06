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
    GeneticAlgorithm(int stopCriteria, Matrix*, double mutationRate, int mutationMethod, int initialPopulationSize);

    Matrix* matrix;
    int objectiveFunction1;
    int objectiveFunction2;
    std::vector<int> bestSolution;
    int bestObjectiveFunction;
    double whenFound;

    std::vector<std::pair<int, std::vector<int>>> population; //pair: fitness function + path
    std::vector<std::pair<int, std::vector<int>>> children; //pair: fitness function + path

    int stopCriteria;
    int initialPopulationSize;

    double mutationRate;
    int mutationMethod;


    double crossBreadingRate;
    void launchCrossBreading(Timer timer);
    void PMX(std::vector<int>, std::vector<int>);

    void launch(Timer timer);
    void transpositionMutation(std::vector<int>&);
    void inversionMutation(std::vector<int>&);

    std::vector<int> generateBegSolutionRandom();
    int calculateRoute(std::vector<int> track);


};


#endif //PEA3_GENETICALGORITHM_H
