#include <iostream>
#include <time.h>
#include <cstdlib>
#include "LittleFermat.h"
#include "Euklides.h"

using namespace std;

extern "C" {unsigned int generateRandomNumber(unsigned int a, unsigned int b);}
extern "C" {unsigned int multiplicationModuloF(unsigned int num1,unsigned int num2, unsigned int modNum);}
extern "C" {unsigned int powerModuloF(unsigned int exponent, unsigned int modNum, unsigned int random);}
extern "C" {unsigned int controller(unsigned int number, int result, unsigned int arrayNumbers[]);}
extern "C" {unsigned int NWD(int a, int b);}


void LittleFermat::enterValue()
{
    srand(time(NULL));
    unsigned int arrayPrimeNumbers[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009 };
    int result = 1;
    unsigned int num;

    cout << "Algorytm ten przydatny jest do sprawdzania pierwszosci duzych liczb (np. wiekszych od 1000)\n";
    cout << "Podaj liczbe: ";
    cin >> num;
    cout << endl;

    if(num == 1){ cout << "Liczba 1 nie jest liczba pierwsza" << endl; return;}
    if(num == 2){ cout << "Liczba 2 jest liczba pierwsza" << endl; return;}

    //wywolanie funkcji odpowedzialnej za kontrole algorytmu Malego Twierdzenia Fermata
    result=controller(num,result,arrayPrimeNumbers);
    if(result == 1)     cout << "Liczba " << num << " jest liczba pierwsza" << endl;
    else                cout << "Liczba " << num << " nie jest liczba pierwsza" << endl;
}


//funkcja generuje losowa liczbe z zakresu <a,b>
unsigned int generateRandomNumber(unsigned int a, unsigned int b)
{
    return (rand()%(b-a+1))+a;
}


// Implementacja funkcji odpowedzialnej za kontrole algorytmu Malego Twierdzenia Fermata
asm("\
        .global _controller                                                           \n\
        _controller:                                                                  \n\
        pushl %ebp              # zabezpieczenie starego %ebp                         \n\
        movl %esp, %ebp         # nowy %ebp zawiera aktualny wsk. stosu               \n\
                                                                                      \n\
        # 8(%ebp) - parametr num                                                      \n\
        # 12(%ebp) - parametr result                                                  \n\
        # 16(%ebp) - parametr tablica                                                 \n\
        # -4(%ebp) - i (iterator)                                                     \n\
        # -8(%ebp) - zmienna lokalna num-1                                            \n\
        # -12(%ebp) - zmienna lokalna d - pomocnicza                                  \n\
        # -16(%ebp) - zmienna lokalna a                                               \n\
                                                                                      \n\
        # Rezerwacja miejsca na zmienne lokalne                                       \n\
        pushl $1                # i                                                   \n\
        pushl $0                # num - 1                                             \n\
        pushl $0                # d                                                   \n\
        pushl $0                # a                                                   \n\
                                                                                      \n\
    # w petli sprawdzane jest czy wczytana liczba nie jest liczba pierwsza            \n\
    # z zakresu 2-1000 lub czy nie jest wielokrotnoscia tych liczb                    \n\
    # odpowiednik kodu w C:                                                           \n\
    # for (int i = 0; i < 169; i++)                                                   \n\
    #    if ((num != arrayPrimeNumbers[i]) && (num % arrayPrimeNumbers[i] == 0))      \n\
    #        {                                                                        \n\
    #          result = 0;                                                            \n\
    #          break;                                                                 \n\
    #        }                                                                        \n\
                                                                                      \n\
    #petla for                                                                        \n\
        movl $0, %eax                                                                 \n\
        movl $2, %edi                                                                 \n\
                                                                                      \n\
        start_loop2:                                                                  \n\
        cmpl $169, %edi                                                               \n\
        je loop_exit2                                                                 \n\
                                                                                      \n\
        #if - (num != arrayPrimeNumbers[i])                                           \n\
            movl 16(%ebp, %edi,4), %ecx                                               \n\
            cmpl 8(%ebp), %ecx                                                        \n\
            je jump                                                                   \n\
                                                                                      \n\
        #if - (num % arrayPrimeNumbers[i] == 0)                                       \n\
            # OPERACJA MODULO                                                         \n\
            # Wykonanie dzielenia: podwojne slowo w %edx:eax                          \n\
            # przez argument instr. div                                               \n\
            # W tym przypadku num/arrayPrimeNumbers[i]. Resza dzielenia w %edx        \n\
            movl $0, %edx                                                             \n\
            movl 8(%ebp), %eax                                                        \n\
            movl %ecx, %ebx                                                           \n\
            divl %ebx                                                                 \n\
                                                                                      \n\
                                                                                      \n\
            cmpl $0, %edx                                                             \n\
            jne jump                                                                  \n\
                                                                                      \n\
            movl $0, 12(%ebp)   #jesli spelniony warunek to result = 0                \n\
            jmp loop_exit2                                                            \n\
            jump:                                                                     \n\
                                                                                      \n\
        incl %edi                                                                     \n\
        jmp start_loop2                                                               \n\
        loop_exit2:                                                                   \n\
                                                                                      \n\
        movl 8(%ebp), %eax              #zapis do -8(%ebp) wartosci (num -1)          \n\
        movl %eax, -8(%ebp)                                                           \n\
        decl -8(%ebp)                                                                 \n\
                                                                                      \n\
                                                                                      \n\
    # po spelnieniu warunkow z if'a, w petli sprawdzane jest czy dana liczba          \n\
    # na pewno jest pierwsza                                                          \n\
    # odpowiednik kodu w C:                                                           \n\
    #     if (result && (num > 1009))                                                 \n\
    #{                                                                                \n\
    #     for (int i = 1; i <= 10; i++)                                               \n\
    #     {                                                                           \n\
    #        a = generateRandomNumber(2, num - 1);                                    \n\
    #        if ((NWD(num, a) != 1) || (powerModuloF(num - 1, num, a) != 1))          \n\
    #        {                                                                        \n\
    #            result = 0;                                                          \n\
    #            break;                                                               \n\
    #        }                                                                        \n\
    #     }                                                                           \n\
    #}                                                                                \n\
                                                                                      \n\
    #if - result == 1                                                                 \n\
    cmpl $0, 12(%ebp)                                                                 \n\
    je end_while_loop3                                                                \n\
                                                                                      \n\
    #if - (num > 1009)                                                                \n\
    cmpl $1009, 8(%ebp)                                                               \n\
    jle end_while_loop3                                                               \n\
                                                                                      \n\
    #petla                                                                            \n\
    loop3:                                                                            \n\
                                                                                      \n\
        cmpl $10, -4(%ebp)                                                            \n\
        je end_while_loop3                                                            \n\
                                                                                      \n\
                                                                                      \n\
         # Wywolanie funkcji losuj                                                    \n\
         # zwrocony wynik zapisany w zmiennej pomocniczej a                           \n\
         #  a = generateRandomNumber(2, num - 1);                                     \n\
                pushl -8(%ebp)                                                        \n\
                pushl $2                                                              \n\
                call _generateRandomNumber                                            \n\
                movl %eax, -16(%ebp)                                                  \n\
                addl $8, %esp              # wyrownanie stosu                         \n\
                                                                                      \n\
         # Wywolanie funkcji powerModuloF(num - 1, num, a)                            \n\
         # zwrocony wynik zapisany bedzie w zmiennej pomocniczej d                    \n\
                pushl 8(%ebp)                                                         \n\
                pushl -8(%ebp)                                                        \n\
                pushl -16(%ebp)                                                       \n\
                call _powerModuloF                                                    \n\
                movl %eax, -12(%ebp)                                                  \n\
                addl $12, %esp              # wyrownanie stosu                        \n\
                                                                                      \n\
                cmpl $1, -12(%ebp)          #sprawdzenie czy wynik spelnia warunek    \n\
                jne resultToZero                                                      \n\
                                                                                      \n\
                                                                                      \n\
         # Wywolanie funkcji NWD(num, a)                                              \n\
         # zwrocony wynik zapisany bedzie w zmiennej pomocniczej d                    \n\
                pushl -16(%ebp)                                                       \n\
                pushl 8(%ebp)                                                         \n\
                call _NWD                                                             \n\
                movl %eax, -12(%ebp)                                                  \n\
                addl $8, %esp              # wyrownanie stosu                         \n\
                                                                                      \n\
                cmpl $1, -12(%ebp)         #sprawdzenie czy wynik spelnia warunek     \n\
                jne resultToZero                                                      \n\
                                                                                      \n\
                                                                                      \n\
                                                                                      \n\
                                                                                      \n\
       incl -4(%ebp)                                                                  \n\
       jmp loop3                                                                      \n\
                                                                                      \n\
                                                                                      \n\
    resultToZero:                                                                     \n\
        movl $0, %eax               # zapis nowego result (czyli 0) do %eax           \n\
        jmp tu                                                                        \n\
    end_while_loop3:                                                                  \n\
        movl 12(%ebp), %eax                                                           \n\
    tu:                                                                               \n\
                                                                                      \n\
    addl $16, %esp              # wyrownanie stosu po zmiennych lokalnych             \n\
    movl %ebp, %esp             # odtworzenie starego stosu                           \n\
    popl %ebp                   # odtworzenie starego %ebp                            \n\
    ret                         # powrot z funkcji                                    \n\
    "
);

// Implementacja funkcji powerModuloF
asm("\
        .global _powerModuloF                                                           \n\
        _powerModuloF:                                                                  \n\
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

// Implementacja funkcji multiplicationModuloF
asm("\
        .global _multiplicationModuloF                                                  \n\
        _multiplicationModuloF:                                                         \n\
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

unsigned int NWD(int a, int b){
        asm("\
        # 8(%ebp) - zmienna a                                               \n\
        # 12(%ebp) - zmienna b                                              \n\
        # %ecx - rejestr pomocniczy                                         \n\
                                                                            \n\
        start_loop:                                                         \n\
            cmpl $0, 12(%ebp)         # jesli b=0 to koniec                 \n\
            je end_loop                                                     \n\
            movl 12(%ebp), %ecx                                             \n\
                                                                            \n\
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
