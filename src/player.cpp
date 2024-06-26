#include "../include/cli_blackjack/card.hpp"
#include "../include/cli_blackjack/player.hpp"

Player::Player(const std::string &name_in, double money_in) {
    this->name = name_in;
    this->money = money_in;
    this->card_sum = 0;
    this->hand = {};
}

int Player::get_card_sum() {
    return this->card_sum;
}

void Player::add_card(const Card& card) {
    this->hand.push_back(card);
}

void Player::set_card_sum(int card_sum_in) {
    this->card_sum = card_sum_in;
}

double Player::get_money() {
    return this->money;
}

void Player::set_money(double money_in) {
    this->money = money_in;
}

std::string Player::get_name() {
    return this->name;
}

Card Player::get_card_at(int index) {
    return this->hand[index];
}

double Player::place_bet(std::ostream& output, std::istream& input) {
// prompt the user to bet a certain amount of money
// make sure that the user bets a valid amount of money (maybe do that in a while loop)
// subtract the bet from the players money balance
// return the bet amount
    double bet_amount;
    while (true) {
        output << "Enter the amount of money you would like to bet: ";
        input >> bet_amount;

        if (bet_amount > 0 && bet_amount <= this->money) {
            break;
        } 
        else {
            output << "You placed an invalid bet. Try again.\n";
        }
    }

    this->money -= bet_amount;
    return bet_amount;
}

bool Player::hit(std::ostream& output, std::istream& input) {
    output << "Would you like to hit or stand?\n";
    int response;
    while (true) {
        output << "[1]: Hit\n";
        output << "[2]: Stand\n";
        input >> response;
        if (response == 1) {
            return true;
        }
        else if (response == 2) {
            return false;
        }
        else {
            output << "Please enter a valid option\n";
        }
    }
    //return true;
}

void Player::print_hand(std::ostream& output) {
    output << "Your hand: [";
    for (size_t i = 0; i < this->hand.size(); i++) {
        if (i == this->hand.size()-1) {
            output << this->hand[i];
            break;
        }
        output << this->hand[i] << " | ";
    }
    output << "]\n";
}

void Player::clear_hand() {
    this->hand.erase(this->hand.begin(), this->hand.end());
    this->card_sum = 0;
}
