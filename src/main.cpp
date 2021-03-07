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
    raw_mode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // turn off control+Q and control+S [IXON] and conversion of carriage returns -> newlines [ICRNL]
    raw_mode.c_oflag &= ~(OPOST); // turn off output post-processing (converting \n to \r\n)
    raw_mode.c_cflag &= ~(CS8);
    raw_mode.c_lflag = raw_mode.c_lflag & ~(ECHO | ICANON | IEXTEN | ISIG); // use ICANON to read byte-by-byte, use ISIG to turn off SIGINT and SIGSTOP
    raw_mode.c_cc[VMIN] = 0; // min number of bytes needed before read() if fired
    raw_mode.c_cc[VTIME] = 1; // max amount of time to wait for input, in tenths of a second

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_mode);
};

void die(const char *s){
    perror(s);
    exit(1);
}

int main()
{

    setRawMode();

    while (1)
    {

        char c;
        read(STDIN_FILENO, &c, 1);

        /* returns 0 when reaches end of input */
        if (iscntrl(c)) // test for control characters (0-31 ascii codes -> http://asciitable.com/)
        {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    };
    return 0;
}