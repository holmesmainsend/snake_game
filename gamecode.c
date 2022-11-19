//TODO: record screencast of gameplay
//TODO: add comments to program code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

int main() {
    // Explanation of this code added below
    char snake[999];
    char eraser[999];
    char temporary[999];
    strcpy(snake, "----->");
    strcpy(eraser, "      ");
    
    int y = 0;
    int x = 0;

    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    // Creating snake pit border for given terminal window
    WINDOW * win = newwin(LINES - 1, COLS - 1, 0, 0);
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
        
        // these lines are making the snake disappear
        move(y,x);
        addstr(eraser);
        
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
        /*
            As far as I could determine, C does not contain a simple way to add characters to the beginning
            of an array (like the unshift() method in JavaScript). Therefore, the code below transfers the
            current snake to a temporary placeholder, sets a "-" segment as the new snake array, and then
            appends the rest of the former snake to this by copying it over from the snakeTemp. The same
            process is repeated for the eraser of the old snake, except with a " " instead of a "-" char.
            Currently, the rate of growth is one snake segment per key press.
        */
        strcpy(temporary, snake);
        strcpy(snake, "-");
        strncat(snake, temporary, 999);
        strcpy(temporary, eraser);
        strcpy(eraser, " ");
        strncat(eraser, temporary, 999);
    }
}
