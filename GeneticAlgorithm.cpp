//
// Created by adamc on 02.01.2024.
//

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int stopCriteria, Matrix* matrix, double mutationRate, int mutationMethod, int initialPopulationSize){
    this->stopCriteria=stopCriteria;
    this->matrix=matrix;
    this->mutationRate=mutationRate;
    this->mutationMethod=mutationMethod;
    this->initialPopulationSize=initialPopulationSize;
    objectiveFunction1 = 0;
    objectiveFunction2 = 0;
}

std::vector<int> GeneticAlgorithm::generateBegSolutionRandom(){
    std::vector<int> track;
    std::vector<std::pair<bool, int>> visited; //vector to help to decide

    for (int i = 0; i < matrix->nrV; ++i) { //filling the vector
        visited.push_back(std::make_pair(false,i));
    }

    int x;

    for (int i = 0; i < matrix->nrV; ++i) {
        x = rand()%(matrix->nrV); //get random number

        while(visited[x].first){ //if it exists in track1, generate once again
            x = rand()%(matrix->nrV);
        }

        track.push_back(x);
        visited[x].first = true; //we visit every node once

    }

    for (int i = 0; i < track.size(); ++i) {
        std::cout << track[i] << "->";
    }

    std::cout << track[0];
    std::cout<<std::endl;
    return track;
}

//Partially Matched Crossover
void GeneticAlgorithm::PMX(std::vector<int> track1, std::vector<int> track2){

    //two random cross-breading points [x1,x2]
    int firstCrossBreadingPoint = rand()%(track1.size()-1); //we can't cross only 1 last element
    int secondCrossBreadingPoint = rand()%(track1.size());

    //ALSO THESE TWO VARIABLES HAS TO DIFFER AT LEAST BY 2!!! STILL NEED TO WRITE IT
    //x2 > x1
    while(secondCrossBreadingPoint < firstCrossBreadingPoint){
        secondCrossBreadingPoint = rand()%(track1.size());
    }

    //two children
    std::vector<int> child1;
    std::vector<int> child2;

    //fill the children with "-1"
    for(int i=0; i<track1.size(); i++){
        child1.push_back(-1);
        child2.push_back(-1);
    }

    //swap
    for (int i = firstCrossBreadingPoint; i < secondCrossBreadingPoint; ++i) {
        child1[i]=track2[i];
        child2[i]=track1[i];
    }

    //inserting towns not making conflicts with parents
    bool shouldInsert1;
    bool shouldInsert2;
    //vectors with towns which are making conflicts (we use them later)
    std::vector<int> leftTowns1;
    std::vector<int> leftTowns2;

    for (int i = 0; i < track1.size(); ++i) {

        shouldInsert1 = true;
        shouldInsert2 = true;

        for (int j = firstCrossBreadingPoint; j < secondCrossBreadingPoint; ++j) {

            //if town exists already in new track, don't insert it
            if(track1[i] == child1[i]){
                shouldInsert1 = false;
            }

            if(track2[i] == child2[i]){
                shouldInsert2 = false;
            }

            if(!shouldInsert1 && !shouldInsert2){
                break;
            }

        }

        //insert town if it doesn't exist
        if(shouldInsert1){
            track1[i] = child1[i];
        }

        else{
            leftTowns1.push_back(track1[i]);
        }

        if(shouldInsert2){
            track2[i] = child2[i];
        }

        else{
            leftTowns2.push_back(track2[i]);
        }

    }

    //left towns: insert going from left side, watching the mapping table
    for (int i = 0; i < leftTowns1.size(); ++i) {
        for (int j = 0; j < track1.size(); ++j) {
            if(child1[j] == -1){
                child1[j] = leftTowns1[i];
                break;
            }
        }
    }

    for (int i = 0; i < leftTowns2.size(); ++i) {
        for (int j = 0; j < track2.size(); ++j) {
            if(child2[j] == -1){
                child2[j] = leftTowns2[i];
                break;
            }
        }
    }

    //mutations
    if (mutationMethod == 1) {
        GeneticAlgorithm::transpositionMutation(child1);
        GeneticAlgorithm::transpositionMutation(child2);

        children.emplace_back(calculateRoute(child1), child1);
        children.emplace_back(calculateRoute(child2), child2);

    } else if (mutationMethod == 2) {
        GeneticAlgorithm::inversionMutation(child1);
        GeneticAlgorithm::inversionMutation(child2);

        children.emplace_back(calculateRoute(child1), child1);
        children.emplace_back(calculateRoute(child2), child2);

    }

}

void GeneticAlgorithm::transpositionMutation(std::vector<int>& track){

    int randomTownIndex1;
    int randomTownIndex2;

    for(int i=0; i<matrix->nrV*mutationRate; i++){
        randomTownIndex1 = rand() % (track.size()); //1. random town
        randomTownIndex2 = rand() % (track.size()); //2. random town

        while(randomTownIndex1 == randomTownIndex2){ //it has to be 2 different towns
            randomTownIndex2 = rand() % (track.size()); //2. random town
        }

        std::swap(track[randomTownIndex1], track[randomTownIndex2]); //swap

    }

    int newObjectiveFunction = calculateRoute(track);

    if(newObjectiveFunction < bestObjectiveFunction) {
        bestObjectiveFunction = newObjectiveFunction;
        bestSolution = track;
        std::cout << bestObjectiveFunction << std::endl;
    }

}

void GeneticAlgorithm::inversionMutation(std::vector<int>& track){

    //stack to reverse the numbers
    std::stack<int> substring;
    int randomTownIndex1;
    int randomTownIndex2;

    for(int i=0; i<matrix->nrV*mutationRate; i++){
        randomTownIndex1 = rand() % (track.size() - 1); //we have to have at least 2 elements to invert
        randomTownIndex2 = (rand() % track.size()) + 1; //random towns

        for(int j=randomTownIndex1; j<randomTownIndex2; j++){
            substring.emplace(track[j]);
        }

        for(int j=randomTownIndex1; j<randomTownIndex2; j++){
            track[j] = substring.top();
            substring.pop();
        }

    }

    int newObjectiveFunction = calculateRoute(track);

    if(newObjectiveFunction < bestObjectiveFunction) {
        bestObjectiveFunction = newObjectiveFunction;
        bestSolution = track;
        std::cout << bestObjectiveFunction << std::endl;
    }

}

void GeneticAlgorithm::launch(Timer timer){

    srand(time(NULL)); //initialize the seed

    int n, randomParent1, randomParent2;

    //generate random population
    while(population.size() < initialPopulationSize){
        std::vector<int> solution = GeneticAlgorithm::generateBegSolutionRandom();
        int objectiveFunction = calculateRoute(solution);
        population.emplace_back(objectiveFunction, solution);
    }

    while(timer.stopTimer() / 1000000.0 < stopCriteria) {

        //clear table for new children
        children.clear();

        //rank selection
        //1. sort
        std::sort(population.begin(), population.end());

        //number of surviving persons
        n = 30;

        //2. kill not used solutions
        for (int i = 0; i < initialPopulationSize-n; i++) {
            population.pop_back();
        }

        //crossover
        while(population.size() + children.size() < initialPopulationSize){
            randomParent1 = rand()%population.size();
            randomParent2 = rand()%population.size();

            while(randomParent2 == randomParent1){
                randomParent2 = rand()%population.size();
            }

            GeneticAlgorithm::PMX(population[randomParent1].second, population[randomParent2].second);

        }

        //new population has been made
    }

}

int GeneticAlgorithm::calculateRoute(std::vector<int> track){
    int objectiveFunction = 0;

    for (int i = 1; i < track.size(); ++i) {
        objectiveFunction+=matrix->adjMatrix[track[i - 1]][track[i]];
    }

    objectiveFunction+=matrix->adjMatrix[track[track.size() - 1]][track[0]];

    return objectiveFunction;
}