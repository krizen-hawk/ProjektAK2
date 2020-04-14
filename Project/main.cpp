#include <iostream>

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

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        }
    }while (option != 0);

    cout << "\nWyjscie z programu" << endl;

    return 0;
}

void displayMainMenu(){
    cout << endl;
    cout << "========== MENU GLOWNE ==========" << endl;
    cout << "1. Algorytm Euklidesa" << endl;
    cout << "2. Chinski test pierwszosci" << endl;
    cout << "3. Male twierdzenie Fermata" << endl;
    cout << "0. Wyjscie" << endl;
}
