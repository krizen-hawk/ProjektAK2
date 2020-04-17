#include <iostream>
#include <cmath>
#include "NaivePrime.h"

using namespace std;
// algorytm polega na dzieleniu liczby a przez liczby z zakresu 2 do sqrt(a) i badaniu reszty
// z dzielenia liczby a przez każdą z liczb z powyższego zakresu; jesli reszta dla każdego dzielelenia
// to 0 to nie jest to liczba pierwsza
// Wystarczy przebadać podzielność p  przez liczby z przedziału [ 2, [ √p  ] ], aby wykluczyć
// liczby złożone.
void NaivePrime::enterValue(){
    int a;
    cout << "Sprawdzane b\251dzi\251 czy dana liczba jest liczb\245 pierwsz\245" << endl;
    cout << "Podaj liczb\251: ";
    cin >> a;
    int temp = checkPrime(a);

    if ( temp == 1 ) cout << "\nLiczba "<< a <<" JEST liczb\245 pierwsz\245!\n\n ";
    if ( temp == 0 ) cout << "\nLiczba "<< a <<" NIE JEST liczb\245 pierwsz\245!\n\n ";
}

int NaivePrime::checkPrime(int a){

    int root=sqrt(a);

    asm("\
        # 8(%ebp) - zmienna a                                                   \n\
        # -12(%ebp) - sqrt(a)                                                   \n\
                                                                                \n\
        movl 8(%ebp), %ecx       # zapis do %ecx sprawdzanej liczby             \n\
        movl $2, %ebx            # licznik pierwszosci do symulowania zakresu   \n\
                                 # a do sqrt(a)                                 \n\
                                                                                \n\
        cmpl $1, %ecx            # jesli liczba jest 1, to nie jest pierwsza    \n\
        je niePierwsza                                                          \n\
                                                                                \n\
        start_loop:                                                             \n\
        cmpl %ebx, -12(%ebp)     # jesli róznica: sqrt(a)-licznik pierw. jest   \n\
        jl loop_exit             # mniejsze od 0 to koniec petli                \n\
        movl %ecx, %eax          # zapis do %eax sprawdzanej liczby             \n\
                                                                                \n\
        movl $0, %edx            # dzielenie %edx:%eax przez arg instr. divl    \n\
        divl %ebx                # czyli wartosc licznika pierwszosci           \n\
                                                                                \n\
                                 # reszta z dzielenia jest w %edx               \n\
        cmpl $0, %edx            # jesli reszta rowna 0 to liczba               \n\
        je niePierwsza           # nie jest pierwsza - koniec petli             \n\
                                                                                \n\
                                                                                \n\
        incl %ebx                #zwieksz licznik                               \n\
        jmp start_loop                                                          \n\
                                                                                \n\
        loop_exit:                                                              \n\
        movl $1, 8(%ebp)         # jesli jest pierwsza to a=1                   \n\
        jmp koniec                                                              \n\
                                                                                \n\
        niePierwsza:             # jesli nie jest pierwsza to a=0               \n\
        movl $0, 8(%ebp)                                                        \n\
                                                                                \n\
        koniec:                                                                 \n\
    ");

    return a;
}
