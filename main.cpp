#include <array>
#include <chrono>
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

class Card {
private:
    const std::array<std::string, 4> suitsName = {"Clubs", "Diamonds", "Hearts", "Spades"};
    const std::array<std::string, 13> rankNames = {"Two", "Three", "Four", "Five", "Six", "Seven",
                                                   "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

    int indexSuit = -1, indexRank = -1;

public:
    Card() = default;

    Card(int indexSuit_, int indexRank_) : indexSuit(indexSuit_), indexRank(indexRank_) {}

    Card(const Card& oth) : indexSuit(oth.indexSuit), indexRank(oth.indexRank) {}

    friend std::ostream& operator<<(std::ostream& os, const Card& card) {
        os << " _______\n|";

        int number = -1;
        char Char = 0;
        if (card.indexRank < 9) {
            number = card.indexRank + 2;
            os << number;
        } else {
            if (card.indexRank == 9)
                Char = 'J';
            else if (card.indexRank == 10)
                Char = 'Q';
            else if (card.indexRank == 11)
                Char = 'K';
            else
                Char = 'A';
            os << Char;
        }

        if (card.indexSuit == 0)
            os << "  _   |\n|  ( )  |\n| (_'_) |\n|   |   |\n|     ";
        else if (card.indexSuit == 1)
            os << "  ^   |\n|  / \\  |\n|  \\ /  |\n|   .   |\n|     ";
        else if (card.indexSuit == 2) {
            if (number != 10)
                os << " _ _  |\n| ( v ) |\n|  \\ /  |\n|   .   |\n|     ";
            else
                os << "_ _  |\n| ( v ) |\n|  \\ /  |\n|   .   |\n|     ";
        }
        else
            os << "  .   |\n|  /.\\  |\n| (_._) |\n|   |   |\n|     ";
        if (number != -1) {
            if (number != 10)
                os << " " << number;
            else os << number;
        }
        else os << " " << Char;
        os << "|\n|_______|\n";
        return os;
    }

    Card& operator=(const Card& oth) {
        indexSuit = oth.indexSuit;
        indexRank = oth.indexRank;
        return *this;
    }

    bool operator==(const Card& oth) const {
        return (oth.indexSuit == indexSuit || oth.indexRank == indexRank);
    }

    [[nodiscard]] int getIndexRank() const { return indexRank; }

    [[nodiscard]] int getIndexSuit() const { return indexSuit; }

    [[nodiscard]] std::string getSuit(int index) const { return suitsName[index]; }

    ~Card() = default;
};


class Deck {
private:
    std::vector<Card> cards;

public:
    Deck () = default;

    Deck(int StartSuits, int StartRank) {
        for (int i = StartSuits; i < 4; i += 1)
            for (int j = StartRank; j < 13; j += 1) {
                Card helper(i, j);
                cards.emplace_back(helper);
            }
    }

    Deck(const Deck& oth) = default;

    Deck& operator=(const Deck& oth) = default;

    friend std::ostream& operator<<(std::ostream& os, const Deck& deck) {
        for (int i = 0; i < (int) deck.cards.size(); i += 1)
            os << deck.cards[i] << "    " << i << '\n';
        return os;
    }

    void Shuffle() {
        static std::random_device rd;
        static std::mt19937 g(rd() + std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    [[nodiscard]] bool isEmpty() const { return cards.empty(); }

    [[nodiscard]] int Size() const { return (int) cards.size(); }

    void add(const Card& newCard) {
        cards.emplace_back(newCard);
    }


    void addFromOther(Deck& oth, int index) {
        cards.push_back(oth.cards[index]);
        oth.cards.erase(oth.cards.begin() + index);
        if (oth.cards.empty()) { oth = Deck{0, 0}; }
    }

    void deleteNthCard(int n) { cards.erase(cards.begin() + n); }

    Card& getNthCard(int n) {
        Card& NthCard = cards[n];
        return NthCard;
    }

    ~Deck () = default;
};

class Game {
private:
    Card currentCard;
    Deck gameDeck{0, 0};
    std::vector<Deck> players;
    int changedCards = -1, skippedOver = 0, getCards = 0;

    void setUpPlayers() {
        for (int turns = 0; turns < 5; turns += 1)
            for (int i = 0; i < (int) players.size(); i += 1)
                players[i].addFromOther(gameDeck, 0);
    }

    static int randomIntGenerator(int Min, int Max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(Min, Max);

        int randomNumber = dis(gen);
        return randomNumber;
    }

    static double randomDoubleGenerator(double Min, double Max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(Min, Max);

        double randomNumber = dis(gen);
        return randomNumber;
    }

    void UpdateParameters(bool player) {
        if (currentCard.getIndexRank() == 0)
            getCards += 2;
        else if (currentCard.getIndexRank() == 1)
            getCards += 3;
        else if (currentCard.getIndexRank() == 2)
            getCards = 0;

        else if (currentCard.getIndexRank() == 5) {
            if (player) {

                std::cout << "Wow! You can now change the suit!\n";
                std::cout << "Press the number next to the suit you want to choose!\n\n";
                std::cout << "0   Clubs\n1   Diamonds\n2   Hearts\n3   Spades\n";

                std::cin >> changedCards;

                while (changedCards < 0 || changedCards >= 4) {
                    std::cout << "That suit does not exist! Choose again!\n";
                    std::cout << "0   Clubs\n1   Diamonds\n2   Hearts\n3   Spades\n";

                    std::cin >> changedCards;
                }
            } else {
                changedCards = randomIntGenerator(0, 3);
            }
        }

        else if (currentCard.getIndexRank() == 12)
            skippedOver += 1;
    }

    std::vector<int> CheckRank(int index, const std::vector<int>& values) {
        std::vector<int> possibleMoves;
        for (int i = 0; i < (int) values.size(); i += 1) {
            for (int j = 0; j < (int) players[index].Size(); j += 1) {
                if (players[index].getNthCard(j).getIndexRank() == values[i]) {
                    possibleMoves.emplace_back(j);
                }
            }
        }
        return possibleMoves;
    }

    std::vector<int> CheckSuit(int index, const std::vector<int>& values) {
        std::vector<int> possibleMoves;
        for (int i = 0; i < (int) values.size(); i += 1) {
            for (int j = 0; j < (int) players[index].Size(); j += 1) {
                if (players[index].getNthCard(j).getIndexSuit() == values[i]) {
                    possibleMoves.emplace_back(j);
                }
            }
        }
        return possibleMoves;
    }

    void PlayerPlacesCard(bool conditions, std::vector<int> correct = {}, bool suitOrRank = false) {
        bool done = false;
        while (!done) {
            std::cout << "Pick the index of the card you want to choose!\n";
            int index; std::cin >> index;
            while (index < 0 || index >= players[0].Size()) {
                std::cout << "You don't that that card! Pick again!\n";
                std::cin >> index;
            }

            Card chosenCard = players[0].getNthCard(index);

            if (conditions) {
                for (int i = 0; i < (int) correct.size(); i += 1) {
                    if ((chosenCard.getIndexRank() == correct[i] && !suitOrRank) || (chosenCard.getIndexSuit() == correct[i] && suitOrRank)) {
                        gameDeck.add(currentCard), currentCard = chosenCard;
                        players[0].deleteNthCard(index);
                        UpdateParameters(true);
                        done = true;
                    }
                }

                if (!done) { std::cout << "You can't place that card!"; }

                if (done && suitOrRank) { changedCards = -1; }
            } else {
                if (chosenCard == currentCard) {
                    gameDeck.add(currentCard), currentCard = chosenCard;
                    players[0].deleteNthCard(index);
                    UpdateParameters(true), done = true;
                } else { std::cout << "This card can't be placed upon the current one!"; }
            }

        }
    }

    void playChangedCards(int index) {
        if (index == 0) {
            std::cout << "The card was changed to " << currentCard.getSuit(changedCards) << '\n';
        }

        std::vector<int> correctValues = {changedCards};
        auto checked = CheckSuit(index, correctValues);

        if (checked.empty()) {
            if (index == 0) {
                std::cout << "You don't have any card with the suit " << currentCard.getSuit(changedCards) << '\n';
                std::cout << "A new card will be moved to your deck automatically :<\n";
            } else {
                std::cout << "Bot no. " << index << " drew a card\n";
            }
            players[index].addFromOther(gameDeck, 0);
        } else {
            if (index == 0) {
                std::cout << "You have some cards that you can put down! Do you want to?\n";
                std::cout << "Press 1 to put down cards\nPress 2 to be skipped over\n";

                int action; std::cin >> action;
                while (action != 1 && action != 2) {
                    std::cout << "            You can't do that! >:( \n";
                    std::cout << "Press 1 to draw cards\nPress 2 to be put down a card\n";
                    std::cin >> action;
                }

                if (action == 1) {
                    players[index].addFromOther(gameDeck, 0);
                } else {
                    PlayerPlacesCard(true, correctValues, true);
                }
            } else {
                double chance = randomDoubleGenerator(0.0, 1.0);
                if (chance < 0.2) {
                    std::cout << "Bot no. " << index << " drew a card\n";
                    players[index].addFromOther(gameDeck, 0);
                } else {
                    int indexMove = randomIntGenerator(0, (int) checked.size() - 1);
                    int indexCard = checked[indexMove];
                    Card chosenCard = players[index].getNthCard(indexCard);
                    std::cout << "Bot no. " << index << " put down \n" << chosenCard << '\n';
                    gameDeck.add(currentCard), currentCard = chosenCard;
                    players[index].deleteNthCard(indexCard);
                    UpdateParameters(false), changedCards = -1;
                }
            }
        }
    }

    void playSkippedOver(int index) {
        if (index == 0) {
            std::cout << "You are being skipped over :< \n";
        }

        std::vector<int> correctValues = {12};
        auto checked = CheckRank(index, correctValues);

        if (checked.empty()) {
            if (index == 0)
                std::cout << "There is nothing you can do... :(\n";
            else
                std::cout << "Bot no. " << index << " was skipped over\n";
            skippedOver -= 1;
        } else {
            if (index == 0) {
                std::cout << "You have some cards that you can put down! Do you want to?\n";
                std::cout << "Press 1 to be skipped over\nPress 2 to put down a card\n";

                int action; std::cin >> action;
                while (action != 1 && action != 2) {
                    std::cout << "          You can't do that! >:( \n";
                    std::cout << "Press 1 to be skipped over\nPress 2 to put down a card\n";
                    std::cin >> action;
                }

                if (action == 1) { skippedOver -= 1; }
                else { PlayerPlacesCard(true, correctValues); }
            } else {
                double chance = randomDoubleGenerator(0.0, 1.0);
                if (chance < 0.2) {
                    std::cout << "Bot no. " << index << " was skipped over\n";
                    skippedOver -= 1;
                } else {
                    int indexMove = randomIntGenerator(0, (int) checked.size() - 1);
                    int indexCard = checked[indexMove];
                    Card chosenCard = players[index].getNthCard(indexCard);
                    std::cout << "Bot no. " << index << " put down\n" << chosenCard << '\n';
                    gameDeck.add(currentCard), currentCard = chosenCard;
                    players[index].deleteNthCard(indexCard);
                    UpdateParameters(false);
                }
            }
        }
    }

    void playGetCards(int index) {
        if (index == 0) {
            std::cout << "You need to draw " << getCards << " cards :( \n";
            std::cout << "However! If you have a 2, 3 or 4 card you can put it down! :D \n";
        }

        std::vector<int> correctValues = {0, 1, 2};
        auto checked = CheckRank(index, correctValues);

        if (checked.empty()) {
            if (index == 0) {
                std::cout << "Unfortunately there are no cards that you can put down :<\n";
                std::cout << getCards << " cards will automatically be placed to your deck :/// \n";
            }
            else
                std::cout << "Bot no. " << index << " drew " << getCards << " cards\n";
            for (int i = 0; i < getCards; i += 1)
                players[index].addFromOther(gameDeck, 0);
            getCards = 0;
        } else {
            if (index == 0) {
                std::cout << "Press 1 to draw " << getCards << " cards\nPress 2 to place a card\n";
                int action; std::cin >> action;
                while (action != 1 && action != 2) {
                    std::cout << "There is no such action! Choose again!\n\n";
                    std::cout << "Press 1 to draw " << getCards << " cards\nPress 2 to place a card\n";
                }
                if (action == 1) {
                    for (int i = 0; i < getCards; i += 1)
                        players[index].addFromOther(gameDeck, 0);
                    getCards = 0;
                } else {
                    PlayerPlacesCard(true, correctValues);
                }
            } else {
                double chance = randomDoubleGenerator(0.0, 1.0);
                if (chance < 0.2) {
                    std::cout << "Bot no. " << index << " drew " << getCards << " cards\n";
                    for (int i = 0; i < getCards; i += 1)
                        players[index].addFromOther(gameDeck, 0);
                    getCards = 0;
                } else {
                    int indexMove = randomIntGenerator(0, (int) checked.size() - 1);
                    int indexCard = checked[indexMove];
                    Card chosenCard = players[index].getNthCard(indexCard);
                    std::cout << "Bot no. " << index << " put down\n" << chosenCard << '\n';
                    gameDeck.add(currentCard), currentCard = chosenCard;
                    players[index].deleteNthCard(indexCard);
                    UpdateParameters(false);
                }
            }
        }
    }

    void playNormal(int index) {
        std::vector<int> checked;
        for (int i = 0; i < players[index].Size(); i += 1) {
            Card card = players[index].getNthCard(i);

            if (card == currentCard)
                checked.emplace_back(i);
        }

        if (checked.empty()) {
            if (index == 0) {
                std::cout << "Unfortunately you have no card that you can put down :<\n";
                std::cout << "A new card will be put in your deck automatically automatically";
            } else {
                std::cout << "Bot no. " << index << " drew a card\n";
            }
            players[index].addFromOther(gameDeck, 0);
        } else {
            if (index == 0) {
                std::cout << "You can either draw a card or place one!\n";
                std::cout << "Press 1 to draw a card\nPress 2 to place a card\n";

                int action; std::cin >> action;
                while (action != 1 && action != 2) {
                    std::cout << "That is not a valid action! Pick again!\n";
                    std::cout << "Press 1 to draw a card\nPress 2 to place a card\n";
                    std::cin >> action;
                }

                if (action == 1) { players[index].addFromOther(gameDeck, 0); }
                else { PlayerPlacesCard(false); }
            } else {
                double chance = randomDoubleGenerator(0.0, 1.0);
                if (chance < 0.2) {
                    std::cout << "Bot no. " << index << " drew a card\n";
                    players[index].addFromOther(gameDeck, 0);
                } else {
                    int indexMove = randomIntGenerator(0, (int) checked.size() - 1);
                    int indexCard = checked[indexMove];
                    Card chosenCard = players[index].getNthCard(indexCard);
                    std::cout << "Bot no. " << index << " put down\n" << chosenCard << '\n';
                    gameDeck.add(currentCard), currentCard = chosenCard;
                    players[index].deleteNthCard(indexCard);
                    UpdateParameters(false);
                }
            }
        }
    }

    void playTurn(int index) {
        if (index == 0) {
            std::cout << "\nCurrent card: \n" << currentCard << "\n\n";
            std::cout << "Your deck: \n" << players[0] << "\n";

            std::cout << "If you think your card is bad, you can end the game right now!\n";
            std::cout << "Press -1 to end the game\nPress 1 to continue the game\n";

            int action; std::cin >> action;
            if (action == -1) {
                exit(0);
            }
        }

        if (changedCards != -1) {
            playChangedCards(index);
        } else if (skippedOver != 0) {
            playSkippedOver(index);
        } else if (getCards != 0) {
            playGetCards(index);
        } else {
            playNormal(index);
        }
    }

public:
    Game() = default;

    explicit Game(int numberPlayers) {
        players.resize(numberPlayers);
    }

    Game(const Game& oth) = default;

    Game& operator=(const Game& oth) {
        currentCard = oth.currentCard, gameDeck = oth.gameDeck;
        players = oth.players, changedCards = oth.changedCards;
        skippedOver = oth.skippedOver, getCards = oth.getCards;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Game& oth) {
        os << "Current card: " << oth.currentCard << '\n';
        os << "Game deck: " << oth.gameDeck << '\n';
        for (int i = 0; i < (int) oth.players.size(); i += 1)
            os << oth.players[i] << '\n';
        return os;
    }

    void Play() {
        gameDeck.Shuffle(), setUpPlayers();

        currentCard = gameDeck.getNthCard(0);
        gameDeck.deleteNthCard(0);

        int playerTurn = 0;
        int numberPlayers = (int) players.size();

        bool gameOver = false;
        while (!gameOver) {
            playTurn(playerTurn);

            if (playerTurn == 0) {
                std::cout << "\nPress any key to continue\n";
                std::string anyKey; std::cin >> anyKey; std::cout << '\n';

                if (players[0].Size() == 1 && anyKey != "macao") {
                    std::cout << "Ha! You forgot to type ""macao""! 3 cards will be moved to your deck automatically\n";
                    for (int i = 0; i < 3; i += 1)
                        players[0].addFromOther(gameDeck, 0);
                }
            }

            playerTurn += 1, playerTurn %= numberPlayers;
            for (int i = 0; i < (int) players.size() && !gameOver; i += 1)
                if (players[i].isEmpty()) {
                    if (i == 0)
                        std::cout << "Hooray! You have won the game!\n";
                    else
                        std::cout << "It appears you have lost... :< \n But you can try again!\n";
                    gameOver = true;
                }
        }
    }

    ~Game() = default;
};

int main() {
    std::cout << "Welcome to Macao! This is how the game is played:\n\n";
    std::cout << "   1. Each player initially receives 5 cards.\n";
    std::cout << "   2. One card is randomly selected. Players must repeatedly put down cards using certain rules.\n";
    std::cout << "   3. You can put down cards which have either the same suit or the same rank as the one already down.\n";
    std::cout << "   4. If you don't have any card that matches the one that is put down, you need to draw a card.\n";
    std::cout << "   5. You can put down more cards only if they are exactly the same.\n";
    std::cout << "   6. Using 2 or 3 will result in the following person drawing 2 or 3 cards, respectively.\n";
    std::cout << "   7. Using 4 will result in not drawing any cards.\n";
    std::cout << "   8. Using A will result in the following person being skipped over.\n";
    std::cout << "   9. Each of these effects are stackable: if I have to receive 2 cards and put down a 3, the next person"
                 " has to receive 5 cards. This is only available for the same type of effect: either drawing more cards"
                 " or being skipped past\n";
    std::cout << "  10. Using a 7 will result in you being able to change the current card to one of the suits.\n";
    std::cout << "  11. When you have only one card remaining, type 'macao', otherwise you will have to draw 3 cards.\n";
    std::cout << "  12. The player that puts down all of his cards wins the game!\n\n";
    std::cout << "   Good luck and have fun!!!\n\n";

    std::cout << "Unfortunately, because you need to see your deck, you can only play with bots :/// \n";
    std::cout << "Input the number of players: ";

    int numberPlayers; std::cin >> numberPlayers;

    while (numberPlayers > 6 || numberPlayers < 1) {
        std::cout << "\nWow! You either have many friends or too little friends... You can only play in 2-5 players :(\n\n";
        std::cout << "Input another number of players: "; std::cin >> numberPlayers;
    }

    Game someGame(numberPlayers);
    someGame.Play();
    return 0;
}
