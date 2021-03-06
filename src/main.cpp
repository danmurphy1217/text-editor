#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

struct termios original_termios; // store original terminal attributes here

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    printf("Goodbye!\n");
}

void setRawMode()
{

    tcgetattr(STDIN_FILENO, &original_termios); // read terminal attrs into original_termios
    atexit(disableRawMode);

    struct termios raw_mode = original_termios;
    raw_mode.c_lflag = raw_mode.c_lflag & ~(ECHO | ICANON); // use ICANON to read byte-by-byte
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_mode);
};

int main()
{

    setRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    {
        /* returns 0 when reaches end of input */
        if (iscntrl(c)) // test for control characters (0-31 ascii codes -> http://asciitable.com/)
        {
            printf("%d\n", c);
        }
        else
        {
            printf("%d ('%c')\n", c, c);
        }
    };
    return 0;
}