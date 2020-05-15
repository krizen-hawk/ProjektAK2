#include <iostream>
#include "ChineseTestPrime.h"

extern "C" {int multiplicationModulo(int num1, int num2, int modNum);}
extern "C" {int powerModulo(int exponent, int modNum);}

using namespace std;

void ChineseTestPrime::enterValue()
{
    int num;
    cout << "Podaj liczbe: ";
    cin >> num;
    if(num == 2){
        cout << "Liczba 2 jest liczba pierwsza" << endl;
        return;
    }
    int result = powerModulo(num, num);
    if(result == 2)
        cout << "Liczba " << num << " jest liczba pierwsza" << endl;
    else
        cout << "Liczba " << num << " nie jest liczba pierwsza" << endl;
}

// Implementacja funkcji powerModulo
asm("\
        .global _powerModulo                                                            \n\
        _powerModulo:                                                                   \n\
        pushl %ebp              # zabezpieczenie starego %ebp                           \n\
        movl %esp, %ebp         # nowy %ebp zawiera aktualny wsk. stosu                 \n\
                                                                                        \n\
        # 8(%ebp) - parametr exponent                                                   \n\
        # 12(%ebp) - parametr modNum                                                    \n\
        # -4(%ebp) - zmienna lokalna bitMask                                            \n\
        # -8(%ebp) - zmienna lokalna rest - reszta z kolejnych poteg 2 mod modNum       \n\
        # -12(%ebp) - zmienna lokalna d - pomocnicza                                    \n\
                                                                                        \n\
        # Rezerwacja miejsca na zmienne lokalne                                         \n\
        pushl $1                # bitMask                                               \n\
        pushl $2                # rest                                                  \n\
        pushl $1                # d                                                     \n\
                                                                                        \n\
        while_loop2:                                                                    \n\
        cmpl $0, -4(%ebp)       # jesli bitMask = 0 -> wyjscie                          \n\
        je end_while_loop2                                                              \n\
            #Operacja bitowa bitMask AND exponent - jesli zero to przejscie dalej       \n\
            movl 8(%ebp), %eax                                                          \n\
            andl -4(%ebp), %eax                                                         \n\
            cmpl $0, %eax                                                               \n\
            je end_loop_if2                                                             \n\
                                                                                        \n\
                # Wywolanie funkcji multiplicationModulo(d, rest, exponent)             \n\
                # zwrocony wynik zapisany w zmiennej pomocniczej d                      \n\
                pushl 12(%ebp)                                                          \n\
                pushl -8(%ebp)                                                          \n\
                pushl -12(%ebp)                                                         \n\
                call _multiplicationModulo                                              \n\
                movl %eax, -12(%ebp)                                                    \n\
                addl $12, %esp              # wyrownanie stosu                          \n\
                                                                                        \n\
            end_loop_if2:                                                               \n\
                                                                                        \n\
            # Wywolanie funkcji multiplicationModulo(rest, rest, exponent)              \n\
            # zwrocony wynik zapisany w rest                                            \n\
            pushl 12(%ebp)                                                              \n\
            pushl -8(%ebp)                                                              \n\
            pushl -8(%ebp)                                                              \n\
            call _multiplicationModulo                                                  \n\
            movl %eax, -8(%ebp)                                                         \n\
            addl $12, %esp                  # wyrownanie stosu                          \n\
                                                                                        \n\
            # Przesuniecie w lewo bitow maski                                           \n\
            sall $1, -4(%ebp)                                                           \n\
            jmp while_loop2                                                             \n\
        end_while_loop2:                                                                \n\
                                                                                        \n\
        movl -12(%ebp), %eax        # d do %eax - zwracana wartosc                      \n\
        addl $12, %esp              # wyrownanie stosu po zmiennych lokalnych           \n\
        movl %ebp, %esp             # odtworzenie starego stosu                         \n\
        popl %ebp                   # odtworzenie starego %ebp                          \n\
        ret                         # powrot z funkcji                                  \n\
    "
);

// Implementacja funkcji multiplicationModulo
asm("\
        .global _multiplicationModulo                                                   \n\
        _multiplicationModulo:                                                          \n\
        pushl %ebp                  # zabezpieczenie starego %ebp                       \n\
        movl %esp, %ebp             # nowy %ebp zawiera aktualny wsk. stosu             \n\
                                                                                        \n\
        # 8(%ebp) - parametr num1                                                       \n\
        # 12(%ebp) - parametr num2                                                      \n\
        # 16(%ebp) - parametr modNum                                                    \n\
        # -4(%ebp) - zmienna lokalna bitMask                                            \n\
        # -8(%ebp) - zmienna lokalna d - pomocnicza                                     \n\
                                                                                        \n\
        # Rezerwacja miejsca na zmienne lokalne                                         \n\
        pushl $1                    # bitMask                                           \n\
        pushl $0                    # d                                                 \n\
                                                                                        \n\
        while_loop:                                                                     \n\
        cmpl $0, -4(%ebp)     # jesli bitMask = 0 -> wyjscie                            \n\
        je end_while_loop                                                               \n\
        # Operacja bitowa bitMask AND num2 - jesli zero to przejscie dalej              \n\
            movl 12(%ebp), %eax                                                         \n\
            andl -4(%ebp), %eax                                                         \n\
            cmpl $0, %eax                                                               \n\
            je end_loop_if                                                              \n\
                                                                                        \n\
                # Operacja d = (d + num1) % modNum                                      \n\
                # Wykonanie dzielenia: podwojne slowo w %edx:eax                        \n\
                # przez argument instr. div                                             \n\
                # Reszta w %edx - przypisanie do zmiennej d                             \n\
                movl $0, %edx                                                           \n\
                movl $0, %eax                                                           \n\
                addl -8(%ebp), %eax                                                     \n\
                addl 8(%ebp), %eax                                                      \n\
                divl 16(%ebp)                                                           \n\
                movl %edx, -8(%ebp)                                                     \n\
                                                                                        \n\
            end_loop_if:                                                                \n\
                                                                                        \n\
            # Przesuniecie bitowe w lewo num1                                           \n\
            movl 8(%ebp), %eax                                                          \n\
            sall $1, %eax                                                               \n\
                                                                                        \n\
            # Operacja num1 = num1 % modNum                                             \n\
            movl $0, %edx                                                               \n\
            movl 8(%ebp), %eax                                                          \n\
            sall $1, %eax                                                               \n\
            divl 16(%ebp)                                                               \n\
            movl %edx, 8(%ebp)                                                          \n\
                                                                                        \n\
            # Przesuniecie bitowe w lewo zmiennej bitMask                               \n\
            sall -4(%ebp)                                                               \n\
            jmp while_loop                                                              \n\
        end_while_loop:                                                                 \n\
                                                                                        \n\
        movl -8(%ebp), %eax         # d do %eax - zwracana wartosc                      \n\
        addl $8, %esp               # wyzerowanie stosu ze zmiennych lokalnych          \n\
        movl %ebp, %esp             # odtworzenie starego stosu                         \n\
        popl %ebp                   # odtworzenie starego %ebp                          \n\
        ret                         # powrot z funkcji                                  \n\
        "
);
