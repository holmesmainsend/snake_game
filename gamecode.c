#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

#define LEFTEDGE    10
#define RIGHTEDGE   30

int main() {
    char snake[] = "--->";
    char blank[] = "    ";
    int y = LEFTEDGE;
    int x = 10;

    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    while(1) {
        move(y,x);
        addstr(snake);
        refresh();
        sleep(1);
        move(y,x);
        addstr(blank);
        int key = getch();
        switch(key) {
            case KEY_UP:
                y -= 1;
                break;
            case KEY_DOWN:
                y += 1;
                break;
            case KEY_LEFT:
                x -= 1;
                break;
            case KEY_RIGHT:
                x += 1;
                break;
            case 'q':
                endwin();
                return 0;
        }
    }
}