#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

int main() {
    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);
    move(0,0);
    addstr("Press any key...");
    while(1) {
        int direction = getch();
        move(0,0);
        switch(direction) {
            case KEY_UP:
                addstr("You pressed the UP arrow");
                break;
            case KEY_DOWN:
                addstr("You pressed the DOWN arrow");
                break;
            case KEY_LEFT:
                addstr("You pressed the LEFT arrow");
                break;
            case KEY_RIGHT:
                addstr("You pressed the RIGHT arrow");
                break;
            case 'q':
                endwin();
                return 0;
        }
        refresh();
    }
}