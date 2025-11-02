#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(0));
    int random = (rand() % 3)+1;
    int guess;

    printf("Welcome to Random number Guessing Game !!!");
    printf("\nYou have to guess a number between 1 to 3\n");
    printf("Enter your guess: ");
    scanf("%d", &guess);

    if(guess < 1 || guess > 3){
        printf("Invalid input! Please enter a number between 1 to 3.\n");
        return 1;
    }
    else{
        if(guess == random){
            printf("Congratulations! You guessed the correct number: %d\n", random);
        }
        else{
            printf("Sorry! The correct number was %d. Better luck next time!\n", random);
        }
    }
}