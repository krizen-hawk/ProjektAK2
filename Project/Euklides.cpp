#include <iostream>
#include "Euklides.h"

using namespace std;

void Euklides::enterValues(){
    int a, b;
    cout << "Obliczna NWD(a,b)" << endl;
    cout << "Podaj wartosc a: ";
    cin >> a;
    cout << "Podaj wartosc b: ";
    cin >> b;
    cout << "\nNWD(" << a << "," << b << ") = " << NWD(a,b) << endl;
}

int Euklides::NWD(int a, int b){
        asm("\
        # 8(%ebp) - zmienna a                                               \n\
        # 12(%ebp) - zmienna b                                              \n\
        # %ecx - rejestr pomocniczy                                         \n\
                                                                            \n\
        start_loop:                                                         \n\
            cmpl $0, 12(%ebp)         # jesli b=0 to koniec                 \n\
            je end_loop                                                     \n\
            movl 12(%ebp), %ecx       # przechowanie wartosci b             \n\
                                                                            \n\
            # OPERACJA MODULO                                               \n\
            # Wykonanie dzielenia: podwojne slowo w %edx:eax                \n\
            # przez argument instr. div                                     \n\
            # W tym przypadku a/b. Resza dzielenia w %edx                   \n\
            movl $0, %edx                                                   \n\
            movl 8(%ebp), %eax                                              \n\
            movl 12(%ebp), %ebx                                             \n\
            divl %ebx                                                       \n\
                                                                            \n\
            # Przypisanie reszty z dzielenia do b                           \n\
            movl %edx, 12(%ebp)                                             \n\
            # Stara wartosc b jest nowa dzielna                             \n\
            movl %ecx, 8(%ebp)                                              \n\
            jmp start_loop                                                  \n\
        end_loop:                                                           \n\
    ");

    return a;
}
