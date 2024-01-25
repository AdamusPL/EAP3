//
// Created by adamc on 02.01.2024.
//

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int stopCriteria, Matrix *matrix, Timer timer, double mutationRate,
                                   int mutationMethod,
                                   int initialPopulationSize, double crossoverRate, int crossoverMethod, bool tests) {
    this->stopCriteria = stopCriteria;
    this->matrix = matrix;
    this->timer = timer;
    this->mutationRate = mutationRate;
    this->mutationMethod = mutationMethod;
    this->initialPopulationSize = initialPopulationSize;
    this->crossoverRate = crossoverRate;
    this->crossoverMethod = crossoverMethod;
    this->tests = tests;
}

std::vector<int> GeneticAlgorithm::generateBegSolutionRandom() {
    std::vector<int> track;
    std::vector<std::pair<bool, int>> visited; //vector to help to decide if town was already visited

    for (int i = 0; i < matrix->nrV; ++i) { //filling the vector with false
        visited.push_back(std::make_pair(false, i));
    }

    int x;

    for (int i = 0; i < matrix->nrV; ++i) {
        x = rand() % (matrix->nrV); //get random number

        while (visited[x].first) { //if it exists in track1, generate once again
            x = rand() % (matrix->nrV);
        }

        track.push_back(x);
        visited[x].first = true; //we visit every node once

    }

    return track;
}

//method to check if generated path is correct(if it doesn't have the same number more than once)
void checkPath(std::vector<int> track1) {
    std::vector<bool> visited;

    for (int i = 0; i < track1.size(); i++) {
        visited.push_back(false);
    }

    for (int i = 0; i < track1.size(); i++) {
        if (visited[track1[i]]) {
            std::cout << "Error!" << std::endl;
            return;
        } else {
            visited[track1[i]] = true;
        }
    }
}

std::vector<int> GeneticAlgorithm::OX(std::vector<int> track1, std::vector<int> track2) {
    std::vector<int> child;

    //1. two random cross-breading points [x1,x2]
    firstCrossoverPoint = rand() % (track1.size() - 1);
    secondCrossoverPoint = firstCrossoverPoint + 1 + rand() % (track1.size() - firstCrossoverPoint -
                                                               1);
    //fill the child with -1 value
    child.assign(track1.size(), -1);

    //vector with visited nodes
    std::vector<bool> visited;
    visited.assign(track1.size(), false);

    //copy towns from random range from parent1 to child
    for (int i = firstCrossoverPoint; i <= secondCrossoverPoint; i++) {
        child[i] = track1[i];
        //it becomes visited
        visited[track1[i]] = true;
    }

    //index in parent2
    int begin;

    //if second crossover point is at the end of the path, we will begin the comparison from index 0 of parent 2
    if(secondCrossoverPoint == track1.size() - 1){
        begin = 0;
    }
    //otherwise, we just take index one after second crossover point
    else{
        begin = secondCrossoverPoint + 1;
    }

    //j <- index in child
    int j = begin;

    //in range [begin, track1.size()-1]
    for (int i = begin; i < track1.size(); i++) {
        //if it's not visited, we put it in path
        if (!visited[track2[i]]) {
            child[j] = track2[i];
            visited[track2[i]] = true;
            j++;
        }
    }

    //if we reached last index in child, we must change it to 0
    if(j == track1.size()){
        j = 0;
    }

    //in range [0, secondCrossoverPoint]
    for (int i = 0; i <= secondCrossoverPoint; i++) {
        if (!visited[track2[i]]) {
            child[j] = track2[i];
            visited[track2[i]] = true;
            j++;
            //if we reached last index in child, we must change it to 0
            if(j == track1.size()){
                j=0;
            }
        }
    }

    return child;


}

void GeneticAlgorithm::transpositionMutation(std::vector<int> &track) {

    //mutation
    randomTownIndex1 = rand() % (track.size()); //1. random town
    randomTownIndex2 = rand() % (track.size()); //2. random town

    while (randomTownIndex1 == randomTownIndex2) { //it has to be 2 different towns
        randomTownIndex2 = rand() % (track.size()); //2. random town
    }

    std::swap(track[randomTownIndex1], track[randomTownIndex2]); //swap

}

void GeneticAlgorithm::inversionMutation(std::vector<int> &track) {

    //mutation
    randomTownIndex1 = rand() % (track.size() - 1); //we have to have at least 2 elements to invert
    randomTownIndex2 = randomTownIndex1 + 1 + rand() % (track.size() - randomTownIndex1 -
                                                        1); //2. random town from range (randomTownIndex1; track.size())

                                                        //put numbers from random range on stack
    for (int j = randomTownIndex1; j <= randomTownIndex2; j++) {
        substring.emplace(track[j]);
    }

    //get numbers from stack in the same range
    for (int j = randomTownIndex1; j <= randomTownIndex2; j++) {
        track[j] = substring.top();
        substring.pop();
    }

}

void GeneticAlgorithm::launch() {
    timer.startTimer();
    srand(time(NULL)); //initialize the seed

    int n, randomParent1, randomParent2;

    //generate random population
    while (population.size() < initialPopulationSize) {
        //generate random solution
        std::vector<int> solution = GeneticAlgorithm::generateBegSolutionRandom();
        //calculate path
        newObjectiveFunction = calculateRoute(solution);

        //if it's the first solution in population, set it to best solution
        if(population.empty()){
            bestObjectiveFunction = newObjectiveFunction;
            bestSolution = solution;
        }

        //check if it's better than the best solution
        else{
            isNewBestSolution(solution);
        }

        //put random solution in population
        population.emplace_back(newObjectiveFunction, solution);
    }

    while (timer.stopTimer() / 1000000.0 < stopCriteria) {

        //clear table for new children
        children.clear();

        //rank selection
        //1. sort
        std::sort(population.begin(), population.end());

        //number of surviving persons
        n = 0.25 * initialPopulationSize;

        //2. kill not used solutions
        for (int i = 0; i < initialPopulationSize - n; i++) {
            population.pop_back();
        }

        //while population+children size is lower than populationSize
        while (population.size() + children.size() < initialPopulationSize) {
            randomParent1 = rand() % population.size();
            randomParent2 = rand() % population.size();

            //it has to be 2 different parents to crossover
            while (randomParent2 == randomParent1) {
                randomParent2 = rand() % population.size();
            }

            //crossover
            if (static_cast<double>(rand() % 100 + 1) / 100.0 <= crossoverRate) {
                //generate two children
                //first - crossover parent1, parent2
                child1 = GeneticAlgorithm::OX(population[randomParent1].second, population[randomParent2].second);
                //second - crossover parent2, parent1
                child2 = GeneticAlgorithm::OX(population[randomParent2].second, population[randomParent1].second);

                //mutations
                if (static_cast<double>(rand() % 100 + 1) / 100.0 <= mutationRate) {
                    if (mutationMethod == 1) {
                        GeneticAlgorithm::transpositionMutation(child1);
                        GeneticAlgorithm::transpositionMutation(child2);

                    } else if (mutationMethod == 2) {
                        GeneticAlgorithm::inversionMutation(child1);
                        GeneticAlgorithm::inversionMutation(child2);

                    }
                }

                //put new children to children vector
                children.emplace_back(calculateRoute(child1), child1);
                children.emplace_back(calculateRoute(child2), child2);

                //changing best solution
                isNewBestSolution(child1);
                isNewBestSolution(child2);

                //clear children
                child1.clear();
                child2.clear();

            } else {
                //if it doesn't crossover under specific probability, new children are exactly as their parents
                children.emplace_back(calculateRoute(population[randomParent1].second),
                                      population[randomParent1].second);
                children.emplace_back(calculateRoute(population[randomParent2].second),
                                      population[randomParent2].second);
            }

        }

        //put children to population
        for (int i = 0; i < children.size(); i++) {
            population.push_back(children[i]);
        }

        //new population has been made
    }

}

void GeneticAlgorithm::isNewBestSolution(std::vector<int> &child) {
    newObjectiveFunction = calculateRoute(child);

    //if new solution is better, then change it
    if (newObjectiveFunction < bestObjectiveFunction) {
        bestObjectiveFunction = newObjectiveFunction;
        bestSolution = child;
//        std::cout << bestObjectiveFunction << std::endl;
        whenFound = timer.stopTimer() / 1000000.0;
        //if it's tests, then save score
        if (tests) {
            save.emplace_back(whenFound, bestObjectiveFunction);
        }
    }
}

int GeneticAlgorithm::calculateRoute(std::vector<int> track) {
    int objectiveFunction = 0;

    for (int i = 1; i < track.size(); ++i) {
        objectiveFunction += matrix->adjMatrix[track[i - 1]][track[i]];
    }

    objectiveFunction += matrix->adjMatrix[track[track.size() - 1]][track[0]];

    return objectiveFunction;
}

void GeneticAlgorithm::printSolution() {

    checkPath(bestSolution);

    for (int i = 0; i < bestSolution.size(); ++i) {
        std::cout << bestSolution[i] << "->";
    }

    std::cout << bestSolution[0];
    std::cout << std::endl;
    std::cout << bestObjectiveFunction << std::endl;

}