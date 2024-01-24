#include <iostream>
#include "Menu.h"

int main() {
    int option=0;
    Menu menu;

    while(option!=10){ //menu

        std::cout << std::endl;
        std::cout << "1. Read data from file and print it" << std::endl;
        std::cout << "2. Read stop criteria" << std::endl;
        std::cout << "3. Set initial population size" << std::endl;
        std::cout << "4. Set mutation rate" << std::endl;
        std::cout << "5. Set crossover rate" << std::endl;
        std::cout << "6. Set method of crossover" << std::endl;
        std::cout << "7. Set method of mutation" << std::endl;
        std::cout << "8. Launch algorithm and print scores" << std::endl;

        std::cin >> option;
        std::cout << std::endl;

        switch (option) {
            case 1:
                menu.option1();
                break;

            case 2:
                menu.option2();
                break;

            case 3:
                menu.option3();
                break;

            case 4:
                menu.option4();
                break;

            case 5:
                menu.option5();
                break;

            case 6:
                menu.option6();
                break;

            case 7:
                menu.option7();
                break;

            case 8:
                menu.option8();
                break;

            case 9:
                menu.autoTests();
                break;

            case 0: //custom setup
                menu.option0();
                break;

        }

    }

}