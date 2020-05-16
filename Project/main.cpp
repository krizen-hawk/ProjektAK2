#include <iostream>
#include <conio.h>
#include <cstdlib>
#include "Euklides.h"
#include "NaivePrime.h"
#include "ChineseTestPrime.h"
#include "LittleFermat.h"


using namespace std;

void displayMainMenu();

int main()
{
    short option;

    do{
        displayMainMenu();
        cout << "Podaj opcje: ";
        cin >> option;

        switch(option){
        case 1:
            cout << "\n------ALGORYTM EUKLIDESA------" << endl;
            Euklides::enterValues();
            break;
        case 2:
            cout << "\n------NAIWNY ALGORYTM SPRAWDZANIA PIERWSZOSCI LICZB------" << endl;
            NaivePrime::enterValue();
            break;
        case 3:
            cout << "\n---------CHINSKI TEST PIERWSZOSCI---------" << endl;
            ChineseTestPrime::enterValue();
            break;
        case 4:
            cout << "\n---------MALE TWIERDZENIE FERMATA---------" << endl;
            LittleFermat::enterValue();
            break;

        case 0:
            exit(0);
            break;
        }
        cout << "\n\nNaci\230nij dowolny przycisk, aby przej\230\206 dalej!" << endl;
        getch();
        system("CLS");

    }while (option != 0);

    cout << "\nWyjscie z programu" << endl;

    return 0;
}

void displayMainMenu(){
    cout << endl;
    cout << "========== MENU GLOWNE ==========" << endl;
    cout << "1. Algorytm Euklidesa" << endl;
    cout << "2. Sprawdzanie pierwszo\230ci: naiwny algorytm" << endl;
    cout << "3. Chi\344ski test pierwszo\230ci" << endl;
    cout << "4. Ma\210e twierdzenie Fermata" << endl;
    cout << "0. Wyj\230cie" << endl;
}
