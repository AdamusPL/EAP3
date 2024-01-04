//
// Created by adamc on 29.11.2023.
//

#include "Menu.h"
#include "FileReader.h"
//#include "FileWriter.h"
#include "GeneticAlgorithm.h"

Menu::Menu(){
    matrix = nullptr;
    stopCriteria = 0;
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
    std::cout<<"Give the cross breading rate"<<std::endl;
    std::cin>>crossBreadingRate;
}


void Menu::option6() {

    std::cout<<"Choose the cross breading method"<<std::endl;
    std::cout <<"1. PMX (Partially Matched Crossover)" << std::endl;
    std::cin >> crossBreadingMethod;

}

void Menu::option7() {

    std::cout<<"Choose the mutation method"<<std::endl;
    std::cout << "1. Transposition mutation" << std::endl;
    std::cout << "2. Inversion mutation" << std::endl;
    std::cin >> mutationMethod;

}

void Menu::option8(){
    if(stopCriteria == 0){
        std::cout<<"Stop criteria hasn't been set yet"<<std::endl;
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

void Menu::manualTests(){
    if(matrix == nullptr){
        std::cout<<"No data hasn't been read yet"<<std::endl;
        return;
    }

    GeneticAlgorithm* geneticAlgorithm = new GeneticAlgorithm(stopCriteria, matrix, mutationRate, mutationMethod);
    timer.startTimer();
//    geneticAlgorithm->launchCrossBreading(timer);
    geneticAlgorithm->launchMutation(timer);

    solution = geneticAlgorithm->bestSolution;
    objectiveFunction = geneticAlgorithm->bestObjectiveFunction;
    printSolution();

    std::cout << "Solution found in: " << geneticAlgorithm->whenFound << std::endl;

    delete geneticAlgorithm;
}

void Menu::automaticTests(){

//    FileWriter* fileWriter = new FileWriter();

    for(int i=0; i<10; i++){
//        SimulatedAnnealing* simulatedAnnealing = new SimulatedAnnealing(matrix, a, stopCriteria);
        timer.startTimer();
//        simulatedAnnealing->launchCrossBreading(timer);
//
//        simulatedAnnealing->printSolution();
//        std::cout << "Solution found in: " << simulatedAnnealing->whenFound << std::endl;

//        fileWriter->resultsTime[i] = simulatedAnnealing->whenFound;
//        fileWriter->resultsRoute[i] = simulatedAnnealing->bestObjectiveFunction;

        std::string name;
        std::cout << "Give name of file" << std::endl;
        std::cin >> name;

        std::ofstream file(name);

//        for(auto entry: simulatedAnnealing->save){
//            file << entry.first << " " << entry.second << std::endl;
//        }

        file.close();

        option7();

//        delete simulatedAnnealing;
    }

    std::string graph; //f.e. ftv55
    int i=0;
    while(fileReader.filename[i]!='.'){
        graph+=fileReader.filename[i];
        i++;
    }

//    fileWriter->write("SA", graph);

//    delete fileWriter;

}

void Menu::printSolution(){
    for (int i = 0; i < solution.size(); ++i) {
        std::cout << solution[i] << "->";
    }

    std::cout << solution[0];
    std::cout<<std::endl;
    std::cout<<objectiveFunction<<std::endl;
}