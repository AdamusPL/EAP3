//
// Created by adamc on 29.11.2023.
//

#include "Menu.h"

Menu::Menu(){
    matrix = nullptr;
    stopCriteria = 0;
    initialPopulationSize = 0;
    mutationMethod = 0;
    crossoverMethod = 0;
    mutationRate = 0;
    crossoverRate = 0;
    objectiveFunction = 0;
}

void Menu::option1() {

    if(matrix != nullptr){ //free memory if previous matrix still exist
        delete matrix;
    }

    matrix = fileReader.read(); //read matrix data from .txt file and change pointer to it
}

void Menu::option2() {
    std::cout<<"Give the stop criteria (in seconds):"<<std::endl;
    std::cin>>stopCriteria;
}

void Menu::option3() {
    std::cout<<"Give the initial population size:"<<std::endl;
    std::cin>>initialPopulationSize;
}

void Menu::option4(){

    std::cout<<"Give the mutation rate:"<<std::endl;
    std::cin>>mutationRate;

}

void Menu::option5(){
    std::cout<<"Give the crossover rate"<<std::endl;
    std::cin >> crossoverRate;
}


void Menu::option6() {

    std::cout<<"Choose the crossover method"<<std::endl;
    std::cout <<"1. PMX (Partially Matched Crossover)" << std::endl;
    std::cin >> crossoverMethod;

}

void Menu::option7() {

    std::cout<<"Choose the mutation method"<<std::endl;
    std::cout << "1. Transposition mutation" << std::endl;
    std::cout << "2. Inversion mutation" << std::endl;
    std::cin >> mutationMethod;

}

void Menu::option8(){
    if(stopCriteria == 0 || initialPopulationSize == 0 || mutationMethod == 0 || mutationRate == 0){
        std::cout<<"Not all criteria have been read"<<std::endl;
        return;
    }

    //some criteria hasn't been read ifs

    int methodT = 3;
    std::cout << "1. Manual tests" << std::endl;
    std::cout << "2. Automatic tests" << std::endl;
    std::cin>>methodT;

    if(methodT == 1){
        manualTests();
    }

    else if(methodT == 2){
        automaticTests();
    }
}

void Menu::option0(){
    stopCriteria = 240;
    initialPopulationSize = 100;
    mutationRate = 0.01;
    crossoverRate = 0.8;
    crossoverMethod = 1;
    mutationMethod = 1;
}

void Menu::manualTests(){

    bool tests = false;

    if(matrix == nullptr){
        std::cout<<"No data hasn't been read yet"<<std::endl;
        return;
    }

    GeneticAlgorithm* geneticAlgorithm = new GeneticAlgorithm(stopCriteria, matrix, timer,
                                                              mutationRate, mutationMethod, initialPopulationSize,
                                                              crossoverRate, crossoverMethod, tests);

    geneticAlgorithm->launch();

    solution = geneticAlgorithm->bestSolution;
    objectiveFunction = geneticAlgorithm->bestObjectiveFunction;
    printSolution();

    std::cout << "Solution found in: " << geneticAlgorithm->whenFound << std::endl;

    delete geneticAlgorithm;
}

void Menu::automaticTests(){

    FileWriter fileWriter;
    fileWriter.filename = fileReader.filename;
    fileWriter.initialPopulationSize = initialPopulationSize;
    fileWriter.mutationMethod = mutationMethod;

    std::string filenameOfBestSolution;
    bool tests = true;

    for(int i=0; i<10; i++){
        GeneticAlgorithm* geneticAlgorithm = new GeneticAlgorithm(stopCriteria, matrix, timer,
                                                                  mutationRate, mutationMethod, initialPopulationSize,
                                                                  crossoverRate, crossoverMethod, tests);
        timer.startTimer();
        geneticAlgorithm->launch();

        geneticAlgorithm->printSolution();
        std::cout << "Solution found in: " << geneticAlgorithm->whenFound << std::endl;

        fileWriter.write(geneticAlgorithm, i);

        delete geneticAlgorithm;
    }

}

void Menu::printSolution(){
    for (int i = 0; i < solution.size(); ++i) {
        std::cout << solution[i] << "->";
    }

    std::cout << solution[0];
    std::cout<<std::endl;
    std::cout<<objectiveFunction<<std::endl;
}