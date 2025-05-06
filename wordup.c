// Carter Corcuera
// Final Project
// 5/2/25

#include <stdio.h>

#define WORD_LENGTH 5
#define MAX_ATTEMPTS 6
#define BUFFER_SIZE 100

int my_strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int is_valid_input(const char *input) {
    if (my_strlen(input) != WORD_LENGTH) //length is exactly 5
        return 0;
    for (int i = 0; i < WORD_LENGTH; i++) { 
        if (input[i] < 'a' || input[i] > 'z') // ensures its a through z
            return 0;
    }
    return 1;
}

void read_secret_word(char *secret) { // open file in read
    FILE *file = fopen("word.txt", "r");
    if (file == NULL) {
        perror("Failed to open word.txt");
        secret[0] = '\0';  
        return;
    }

    if (fscanf(file, "%5s", secret) != 1) {
        printf("Failed to read the mystery word.\n");
        secret[0] = '\0';
    }

    fclose(file);
}

void get_valid_guess(char *buffer) { //prompt user for guess
    while (1) {
        printf("Enter your guess: ");

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) { 
            printf("Input error.\n");
            continue;
        }

        int len = my_strlen(buffer); //read the guess
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (!is_valid_input(buffer)) { //if prompt is invalid repeat invalid input
            printf("Invalid input. Please enter exactly %d lowercase letters.\n", WORD_LENGTH);
        } else {
            break;
        }
    }
}

void get_feedback(const char *guess, const char *secret, int *matchedGuess) { //checks for matches within the guess
    int matchedSecret[WORD_LENGTH] = {0};

    for (int i = 0; i < WORD_LENGTH; i++) { //checks for right letter right spot
        if (guess[i] == secret[i]) {
            matchedGuess[i] = 1;
            matchedSecret[i] = 1;
        } else {
            matchedGuess[i] = 0;
        }
    }

    for (int i = 0; i < WORD_LENGTH; i++) { // checks for right letter wrong spot
        if (matchedGuess[i]) continue;
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!matchedSecret[j] && guess[i] == secret[j]) {
                matchedGuess[i] = 2;
                matchedSecret[j] = 1;
                break;
            }
        }
    }
}

void print_feedback(const char *guess, const int *matchedGuess) { //prints after checking guess
    printf("Feedback: ");
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (matchedGuess[i] == 1) {
            printf("[%c]", guess[i]);  
        } else if (matchedGuess[i] == 2) {
            printf("(%c)", guess[i]);  
        } else {
            printf(" %c ", guess[i]);  
        }
    }
    printf("\n");
}

int is_correct_guess(const char *guess, const char *secret) { //checks full guess to see if correct
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] != secret[i])
            return 0;
    }
    return 1;
}

void print_previous_guesses(char guess[][WORD_LENGTH + 1], int attempts) { //reprints previous valid guesses
    printf("Previous guesses:\n");
    for (int i = 0; i < attempts; i++) {
        printf("  %d: %s\n", i + 1, guess[i]);
    }
}

int main() {
    char secret[WORD_LENGTH + 1];
    char guess[MAX_ATTEMPTS][WORD_LENGTH + 1];
    char inputBuffer[BUFFER_SIZE];
    int attempt = 0;

    read_secret_word(secret);
    if (secret[0] == '\0') {
        return 1;
    }

    printf("Guess the %d-letter word! You have %d attempts.\n", WORD_LENGTH, MAX_ATTEMPTS);
    printf("() indicates right letter wrong spot, [] indicates right letter right spot.\n");
    
    while (attempt < MAX_ATTEMPTS) { //the game itself, loops 6 times until won or loss
        printf("Attempt %d:\n", attempt + 1);

        get_valid_guess(inputBuffer);

        for (int i = 0; i < WORD_LENGTH; i++) {
            guess[attempt][i] = inputBuffer[i];
        }
        guess[attempt][WORD_LENGTH] = '\0';

        int matchedGuess[WORD_LENGTH];
        get_feedback(guess[attempt], secret, matchedGuess);
        print_feedback(guess[attempt], matchedGuess);

        if (is_correct_guess(guess[attempt], secret)) {
            printf("Correct! You guessed the word.\n");
            return 0;
        }

        print_previous_guesses(guess, attempt + 1);

        attempt++;
    }

    printf("Out of attempts! The correct word was '%s'.\n", secret);
    return 0;
}
