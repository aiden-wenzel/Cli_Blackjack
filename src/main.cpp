#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../include/cli_blackjack/deck.hpp"
#include "../include/cli_blackjack/player.hpp"
#include "../include/cli_blackjack/dealer.hpp"

using namespace std;

// Welcomes the player and prompts them for the money they will be using and their name.
// @returns A player pointer to a player on the heap
Player* welcome();

// Initializes a deck on the heap
// @returns A deck pointer to a deck on the heap
Deck* initialize_deck();

// prints a 3 line gap to std::cout
void print_gap();

// clears the console;
void clear_console();

int main() {
    clear_console();
    // initialize objects
    Player* main_player = welcome();
    Dealer* dealer = new Dealer;
    bool playing = true;
    while (playing) {

        print_gap();
        int pot = main_player->place_bet(std::cout, std::cin);
        Deck* deck = initialize_deck();
        deck->shuffle_deck(1000);

        // first dealing round
        dealer->deal(main_player, deck);
        dealer->deal(dealer, deck);
        dealer->deal(main_player, deck);
        dealer->deal(dealer, deck);

        // print hand
        dealer->print_hand(std::cout);
        main_player->print_hand(std::cout);

        // player hit or stand
        bool hit = true;
        while (hit && main_player->get_card_sum() <= 21) {
            hit = main_player->hit(std::cout, std::cin);
            if (hit) {
                dealer->deal(main_player, deck);
                print_gap();
                main_player->print_hand(std::cout);
            }
        }

        if (main_player->get_card_sum() > 21) {
            std::cout << "BUST\n"; 
        }

        // dealer hit or stand
        else {
            hit = true;
            while (hit && dealer->get_card_sum() <= 21) {
                hit = dealer->hit(main_player->get_card_sum());
                if (hit) {
                    dealer->deal(dealer, deck);
                    print_gap();
                    std::cout << "The dealer draws a card.\n";
                    dealer->print_hand(std::cout);
                }
            }

            if (dealer->get_card_sum() > 21) {
                std::cout << "You win\n";
                main_player->set_money(pot * 2 + main_player->get_money());
            }

            else if (dealer->get_card_sum() > main_player->get_card_sum()) {
                std::cout << "You lose\n";
            }
        }

        std::string response;
        std::cout << "Would you like to play again?[y/n] ";
        std::cin >> response;
        if (response == "y") {playing = true;}
        else if (response == "n") {playing = false;}

        main_player->clear_hand();
        dealer->clear_hand();
        delete deck;
    }

    int player_money = main_player->get_money();

    delete main_player;
    delete dealer;
    
    std::cout << "Saving remaining balance.\n";
    std::ofstream save("../black_jack/money.txt");
    save << player_money;

    return 0;
}

Player* welcome() {
    std::ifstream money_save("../black_jack/money.txt");
    int saved_money = 0;
    if (money_save.is_open()) {
        money_save >> saved_money;
    }

    std::cout << "Welcome to command line blackjack!\n";
    std::cout << "You will be the only player at the table\n";
    
    std::string name_in;
    std::cout << "Please enter you name: ";
    std::cin >> name_in;
        
    double money_in;
    if (money_save.is_open() && saved_money != 0) {
        std::cout << "Using your balance from the previous game.\n";
        money_in = saved_money;
    } 
    
    else {
        std::cout << "Please enter the amount of money you're bringing to the table: ";
        std::cin >> money_in;
    }

    std::cout << "You're all set. Enjoy the game!\n";
    money_save.close();

    return new Player(name_in, money_in);
}		

Deck* initialize_deck() {
    std::ifstream suits_in("../black_jack/suits.in");
    std::ifstream ranks_in("../black_jack/ranks.in");

    if (!suits_in.is_open() || !ranks_in.is_open()) {
        std::cout << "error in opening file streams\n";
        return nullptr;
    }

    return new Deck(suits_in, ranks_in);
}

void print_gap() {
    for (int i = 0; i < 3; i++) {
        std::cout << "\n";
    }
}

void clear_console() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}
