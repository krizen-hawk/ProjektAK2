#include <iostream>
#include <time.h>
#include <cstdlib>
#include "MillerRabin.h"

using namespace std;

extern "C" {unsigned int generateRandomNumberMR(unsigned int a, unsigned int b);}
extern "C" {unsigned int multiplicationModuloMR(unsigned int num1,unsigned int num2, unsigned int modNum);}
extern "C" {unsigned int powerModuloMR(unsigned int exponent, unsigned int modNum, unsigned int random);}


void MillerRabin::enterValue()
{
      srand(time(NULL));

      int a, d, num, x, s = 0, n;
      int t = 1;

      cout << "Podaj liczbe: ";
      cin >> num;
      cout << "Podaj ilosc powtorzen testu: ";
      cin >> n;
      cout << endl;

      if(num == 1){ cout << "Liczba 1 nie jest liczba pierwsza" << endl; return;}
      if(num == 2){ cout << "Liczba 2 jest liczba pierwsza" << endl; return;}

      for(d = num-1; d%2==0; s++) d=d/2;

      for(int i=1; i <= n; i++ )
      {
            a = generateRandomNumberMR (2, num - 2);
            x = powerModuloMR (d, num, a);

            if((x == 1)||(x == num - 1)) continue;

            for(int j = 1; (j < s) && (x != num-1); j++)
            {
              x = multiplicationModuloMR (x, x, num);
              if(x == 1)
              {
                t = 0;
                break;
              }
            }

            if(t == 0) break;

            if(x != num-1)
            {
              t = 0;
              break;
            }
      }

    if(t == 1)     cout << "Liczba " << num << " jest liczba pierwsza" << endl;
    else           cout << "Liczba " << num << " nie jest liczba pierwsza" << endl;

}

//funkcja generuje losowa liczbe z zakresu <a,b>
unsigned int generateRandomNumberMR(unsigned int a, unsigned int b)
{
    return (rand()%(b-a+1))+a;
}


// Implementacja funkcji powerModuloMR
asm("\
        .global _powerModuloMR                                                          \n\
        _powerModuloMR:                                                                 \n\
        pushl %ebp              # zabezpieczenie starego %ebp                           \n\
        movl %esp, %ebp         # nowy %ebp zawiera aktualny wsk. stosu                 \n\
                                                                                        \n\
        # 8(%ebp) - parametr exponent                                                   \n\
        # 12(%ebp) - parametr modNum                                                    \n\
        # 16(%ebp) - parametr random                                                    \n\
        # -4(%ebp) - zmienna lokalna bitMask                                            \n\
        # -8(%ebp) - zmienna lokalna rest - reszta z kolejnych poteg 2 mod modNum       \n\
        # -12(%ebp) - zmienna lokalna d - pomocnicza                                    \n\
                                                                                        \n\
        # Rezerwacja miejsca na zmienne lokalne                                         \n\
        pushl $1                # bitMask                                               \n\
        pushl 16(%ebp)          # rest - zapis wylosowanej liczby                       \n\
        pushl $1                # d                                                     \n\
                                                                                        \n\
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
                # Wywolanie funkcji multiplicationModuloF(d, rest, exponent)            \n\
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
            # Wywolanie funkcji multiplicationModuloF(rest, rest, exponent)             \n\
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

// Implementacja funkcji multiplicationModuloMR
asm("\
        .global _multiplicationModuloMR                                                 \n\
        _multiplicationModuloMR:                                                        \n\
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

