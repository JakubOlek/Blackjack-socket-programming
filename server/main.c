#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/unistd.h>

#define allCards 52
#define maxNameOfSuits 9
#define maxRanks 13
#define maxSuits 4
#define maxCards 10
#define printSize 100
#define inputSize 20

typedef struct {
    int rank;
    char suit[maxNameOfSuits];
} CARD;

char buffer[printSize];
char userInput[inputSize];
char starsPrint[printSize] = "***************************\n";
char dealerPrint[printSize] = "Dealer:\n";
char playerPrint[printSize] = "Player:\n";
char resultPrint[printSize] = "RESULT:\n\n";
char bustPrint[printSize] = "BUST!\n";
char blackjackPrint[printSize] = "BLACKJACK!\n";
char winPrint[printSize] = "YOU WIN!\n\n";
char drawPrint[printSize] = "DRAW!\n\n";
char lostPrint[printSize] = "YOU LOST!\n\n";
char choicePrint[printSize] = "Type HIT/STAND\n";
char gameOverPrint[printSize] = "Thanks for the game!\n";

int clientSocket = 0;

int cardCounter = 0;
int playerCardCounter = 2;
int dealerCardCounter = 2;

bool blackJackPlayer = false;
bool blackJackDealer = false;

int rankNames[maxRanks] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

char suitNames[maxSuits][maxNameOfSuits] = {"Clubs", "Diamonds", "Hearts", "Spades"};


void sendMessage(const char *message) {
    write(clientSocket, message, strlen(message));
}

void readMessage() {
    read(clientSocket, userInput, inputSize);
}

void shuffle(CARD deck[]) {
    int index;

    CARD temp;
    srand(time(NULL));
    for (int i = 0; i < allCards; i++) {
        index = rand() % allCards;
        temp = deck[i];
        deck[i] = deck[index];
        deck[index] = temp;
    }
}

void initializeDeck(CARD deck[]) {
    for (int i = 0; i < allCards; i++) {
        deck[i].rank = rankNames[i % maxRanks];
        strncpy(deck[i].suit, suitNames[i / maxRanks], maxNameOfSuits);
    }
}

void displayCards(const CARD cards[]) {
    for (int i = 0; cards[i].rank != 0; i++) {

        if (cards[i].rank == 1) {
            sprintf(buffer, "Ace of %s \n", cards[i].suit);
            sendMessage(buffer);

        } else if (cards[i].rank == 11) {
            sprintf(buffer, "Jack of %s \n", cards[i].suit);
            sendMessage(buffer);

        } else if (cards[i].rank == 12) {
            sprintf(buffer, "Queen of %s \n", cards[i].suit);
            sendMessage(buffer);

        } else if (cards[i].rank == 13) {
            sprintf(buffer, "King of %s \n", cards[i].suit);
            sendMessage(buffer);

        } else {
            sprintf(buffer, "%d of %s \n", cards[i].rank, cards[i].suit);
            sendMessage(buffer);
        }
    }
}


int isBlackjack(const CARD cards[]) {
    bool ace = false;
    int score = 0;
    for (int i = 0; i < 2; i++) {
        if (cards[i].rank == 11 || cards[i].rank == 12 || cards[i].rank == 13) {
            score += 10;
        } else {
            score += cards[i].rank;

            if (cards[i].rank == 1) {
                ace = true;
            }
        }
    }

    if (score == 21) {
        return true;
    } else if (ace && score == 11) {
        return true;
    } else {
        return false;
    }
}

int score(const CARD cards[]) {
    int result = 0;
    bool ace = false;

    for (int i = 0; i < 10; i++) {
        if (cards[i].rank == 11 || cards[i].rank == 12 || cards[i].rank == 13) {
            result += 10;
        } else {
            result += cards[i].rank;
            if (cards[i].rank == 1) {
                ace = true;
            }
        }
    }

    if (ace == true && result <= 10) {
        sprintf(buffer, "result cards is: %d or: %d\n\n", result, result + 10);
        sendMessage(buffer);

    } else {
        sprintf(buffer, "result cards is: %d\n\n", result);
        sendMessage(buffer);
    }
    return result;
}

void displayDealerFirst(const CARD dealerCards[]) {
    for (int i = 0; i < dealerCardCounter - 1; i++) {

        if (dealerCards[i].rank == 1) {
            sprintf(buffer, "Ace of %s \n\n", dealerCards[i].suit);
            sendMessage(buffer);

        } else if (dealerCards[i].rank == 11) {
            sprintf(buffer, "Jack of %s \n", dealerCards[i].suit);
            sendMessage(buffer);

        } else if (dealerCards[i].rank == 12) {
            sprintf(buffer, "Queen of %s \n", dealerCards[i].suit);
            sendMessage(buffer);

        } else if (dealerCards[i].rank == 13) {
            sprintf(buffer, "King of %s \n", dealerCards[i].suit);
            sendMessage(buffer);

        } else {
            sprintf(buffer, "%d of %s \n", dealerCards[i].rank, dealerCards[i].suit);
            sendMessage(buffer);
        }
    }
}

void dealingCards(const CARD deck[], CARD dealerCards[], CARD playerCards[]) {
    bool endDrawPlayerCards = false;
    bool endDrawDealerCards = false;


    for (int i = 0; i < 2; i++) {

        dealerCards[i].rank = deck[cardCounter].rank;
        strncpy(dealerCards[i].suit, deck[cardCounter].suit, maxNameOfSuits);
        cardCounter++;
        playerCards[i].rank = deck[cardCounter].rank;
        strncpy(playerCards[i].suit, deck[cardCounter].suit, maxNameOfSuits);
        cardCounter++;
    }

    if (isBlackjack(playerCards) == true) {
        blackJackPlayer = true;
    }

    if (isBlackjack(dealerCards) == true) {
        blackJackDealer = true;
    }

    sendMessage(dealerPrint);
    displayDealerFirst(dealerCards);
    sendMessage(starsPrint);
    sendMessage(playerPrint);

    do {
        displayCards(playerCards);

        if (blackJackPlayer) {
            sendMessage(blackjackPrint);
            break;
        }

        if (score(playerCards) > 21) {
            sendMessage(bustPrint);
            endDrawPlayerCards = true;
            endDrawDealerCards = true;
        } else {
            sendMessage(choicePrint);

            readMessage();

            if (strcmp(userInput, "HIT") == 0) {
                playerCards[playerCardCounter].rank = deck[cardCounter].rank;
                strncpy(playerCards[playerCardCounter].suit, deck[cardCounter].suit, maxNameOfSuits);
                playerCardCounter++;
                cardCounter++;

            } else {
                endDrawPlayerCards = true;
            }
        }

    } while (!endDrawPlayerCards);

    sendMessage(starsPrint);
    sendMessage(dealerPrint);

    do {
        displayCards(dealerCards);

        if (blackJackDealer) {
            sendMessage(blackjackPrint);
            break;
        }

        if (score(dealerCards) < 17) {
            dealerCards[dealerCardCounter].rank = deck[cardCounter].rank;
            strncpy(dealerCards[dealerCardCounter].suit, deck[cardCounter].suit, maxNameOfSuits);
            dealerCardCounter++;
            cardCounter++;
        } else {
            endDrawDealerCards = true;
        }
    } while (!endDrawDealerCards);
}

void gameResult(const CARD dealerCards[], const CARD playerCards[]) {

    sendMessage(starsPrint);
    sendMessage(resultPrint);

    int playerScore;
    int dealerScore;

    sendMessage(playerPrint);
    if (blackJackPlayer) {
        playerScore = 21;
        sprintf(buffer, "result cards is: %d\n\n", playerScore);
        sendMessage(buffer);
    } else {
        playerScore = score(playerCards);
    }

    sendMessage(dealerPrint);
    if (blackJackDealer) {
        dealerScore = 21;
        sprintf(buffer, "result cards is: %d\n\n", dealerScore);
        sendMessage(buffer);
    } else {
        dealerScore = score(dealerCards);
    }

    if (playerScore == dealerScore) {
        sendMessage(drawPrint);
    } else if (playerScore > dealerScore && playerScore < 22) {
        sendMessage(winPrint);

    } else if (dealerScore > 21 && playerScore < 22) {
        sendMessage(winPrint);
    } else {
        sendMessage(lostPrint);
    }

}

void clean(CARD dealerCards[], CARD playerCards[]) {

    for (int i = 0; i <= maxCards; i++) {
        playerCards[i].rank = 0;
        dealerCards[i].rank = 0;
    }

    cardCounter = 0;
    playerCardCounter = 2;
    dealerCardCounter = 2;
    blackJackPlayer = false;
    blackJackDealer = false;
    memset(userInput, 0, inputSize);
}

void game() {
    CARD deck[allCards];
    CARD playerCards[maxCards];
    CARD dealerCards[maxCards];

    clean(playerCards, dealerCards);
    initializeDeck(deck);
    shuffle(deck);
    dealingCards(deck, dealerCards, playerCards);
    gameResult(dealerCards, playerCards);
    sendMessage(gameOverPrint);
    close(clientSocket);
}

int main(void) {

    int port = 4444;
    int mySocket;
    struct sockaddr_in server, client;
    socklen_t size = sizeof(struct sockaddr_in);

    mySocket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(mySocket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Bind failed.\n");
        return 1;
    }

    if (listen(mySocket, 10) < 0) {
        printf("Listen failed.\n");
        return 1;
    }

    printf("Waiting for incoming connections on: 127.0.0.1/4444\n");
    while ((clientSocket = accept(mySocket, (struct sockaddr *) &client, &size)) > 0) {

        printf("Player successful connected\n");
        game();


    }

    return 0;
}

