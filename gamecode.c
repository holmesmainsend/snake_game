#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

int main() {
    initscr();
    
    clear();
    move(10,20);
    addstr("Hello, world");
    move(LINES-1,0);

    refresh();
    getch();

    endwin();
}