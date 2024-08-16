#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void delay(unsigned short milliseconds)
{
#ifdef _WIN32
    // Windows
    Sleep(milliseconds);
#else
    // Unix-like (Linux, macOS)
    usleep(milliseconds * 1000); // usleep takes microseconds
#endif
}

int main()
{
    unsigned char O = 0, X = 0, Q = 0;
    char B[9] = "---------";
    short win[8] = {448, 56, 7, 292, 146, 73, 84, 273};
    unsigned char I;
    unsigned char draw;
    while (1)
    {
        for (unsigned char i = 0; i < 8; ++i)
            if ((X & win[i]) == win[i])
            {
                printf("%c win", "OX"[Q]);
                delay(500);
                return 0;
            }
        draw = 1;
        for (unsigned char i = 0; i < 9; ++i)
        {
            if (B[i] == '-')
            {
                draw = 0;
                break;
            }
        }
        if (draw)
        {
            printf("Tie");
            delay(500);
        }
        Q ^= 1;
        printf("Enter: ");
        scanf("%hhu", &I);
        if (I < 0 || I > 8)
            I = 8;
        B[I] = "OX"[Q];
        O |= 1 << I;
        X ^= O;
        O ^= X;
        X ^= O;
        printf("%s\n", B);
    }
    return 0;
}