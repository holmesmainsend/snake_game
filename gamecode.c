//TODO: record screencast of gameplay
//TODO: add comments to program code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

int main() {
    char snake[] = "----->";
    char blank[] = "      ";
    int y = 0;
    int x = 0;

    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    // TODO: must change snake pit border to cover all available current terminal space
    int height = 20;
    int width = 80;
    WINDOW * win = newwin(height, width, (LINES - height) / 2, (LINES - height) / 2);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    // TODO: add game end if snake runs into itself/border or reverses direction
    // TODO: make snake movement continuous based on last recorded arrow key input
    while(1) {
        move(y,x);
        addstr(snake);
        refresh();
        sleep(1);
        
        //these lins are making the snake disapear
        //move(y,x);
        //addstr(blank);
        
        //I believe that we can use if(user input changes) then (run the switch code) 
        //That way we can continually move the snake even when there is no input
        //Also we can store the direction instead of updating x and y and increment the head of the snake by the direction
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
