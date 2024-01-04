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
    GeneticAlgorithm(int stopCriteria, Matrix*, double mutationRate, int mutationMethod);

    Matrix* matrix;
    std::vector<int> track1;
    std::vector<int> track2;
    int objectiveFunction1;
    int objectiveFunction2;
    std::vector<int> bestSolution;
    int bestObjectiveFunction;
    double whenFound;

    int stopCriteria;
    int initialPopulationSize;

    double mutationRate;
    double crossBreadingRate;

    int mutationMethod;

    void launchCrossBreading(Timer timer);
    void PMX();

    void launchMutation(Timer timer);
    void transpositionMutation();
    void inversionMutation();

    std::vector<int> generateBegSolutionRandom();
    int calculateRoute(std::vector<int> track);


};


#endif //PEA3_GENETICALGORITHM_H
