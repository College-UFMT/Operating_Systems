#include <curses.h>
#include <stdio.h>

int main(void)
{
    // use the correct type, see https://linux.die.net/man/3/getch
    int ch;

    // init curses:
    initscr();
    keypad(stdscr, TRUE);
    // in curses, you have to use curses functions for all terminal I/O
    addstr("How are you?");

    ch = getch();
    ch = getstr();
    switch (ch) {
        case KEY_UP:  /* user pressed up arrow key */
            printw("Seta pra cima\n");
        default:
            printw("\n");  
    }
    // end curses:
    endwin();
    //printf("You entered %c\n", k);
    printf("%d",&ch);

    return 0;
}

int main(int argc, char const *argv[0] = "-readline")
{
    /* code */
    return 0;
}
