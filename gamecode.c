#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int main() {
    int x;
    printf("Please enter a numeric grade: ");
    scanf("%d", &x);
    if (x < 0 || x > 100)
        printf("Your input was invalid.");
    else if (x < 101 && x >= 90)
        printf("Your grade is an A.");
    else if (x < 90 && x >= 80)
        printf("Your grade is a B.");
    else if (x < 80 && x >= 70)
        printf("Your grade is a C.");
    else if (x < 70 && x >= 60)
        printf("Your grade is a D.");
    else
        printf("Your grade is an F.");
    return 0;
}