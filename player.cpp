#include "player.h"
#include "deck.h"
#include "hand.h"
#include "rand.h"
#include <iostream>
#include <cassert>
#define BET_MINIMUM 5
using namespace std;





static bool player_draw(const Card& dealer, const Hand& player){
    // EFFECTS: returns true if the player wishes to be dealt another
    // card, false otherwise.
    // this method can be applied to both simple player and counting one
    if (player.handValue().soft){ // when the player's hand is "soft"
        if (player.handValue().count <= 17){
            return true;
        }
        else if (player.handValue().count == 18){
            return !(dealer.spot == 0 || dealer.spot == 5 || dealer.spot == 6); // index of enum of "2,7,8"
        }else
            return false;
    }
    else{ // "hard" state
        if (player.handValue().count <= 11){
            return true;
        }
        else if (player.handValue().count == 12){
            return !(dealer.spot == 2 || dealer.spot == 3 || dealer.spot == 4);
        }
        else if (player.handValue().count >= 13 && player.handValue().count <= 16){
            return !(dealer.spot >=0 && dealer.spot <=4);
        }else
            return false;
    }
}


class simple_Player:public Player{
public:

    int bet(unsigned int bankroll, unsigned int minimum) override{
        if (bankroll < minimum){
            cout << "error in Simple_player bet()! bankroll too small!\n";
            assert(0);
        }
        return (int)minimum; // simple player, always bet the minimum
    }

    bool draw(Card dealer, const Hand &player) override{
        return player_draw(dealer,player);
    }

    void expose(Card c) override{} // in simple player, no need to implement this
    void shuffled() override{} // in simple player, no need to implement this

};

class counting_Player:public Player{
    int count;
public:
    explicit counting_Player(int count = 0){
        this->count = count;
    }

    void shuffled() override{
        // MODIFIES: count
        // EFFECTS: reset count to zero
        count = 0;
    }

    void expose(Card c) override{
        // MODIFIRS: count
        // EFFECTS: if spot is between ten or ace, count --
        if (c.spot >= TEN && c.spot <= ACE){
            count --;
            return;
        }
        else if (c.spot >= TWO && c.spot <= SIX){
            count ++;
            return;
        }
    }

    int bet(unsigned int bankroll, unsigned int minimum) override{
        // EFFECTS: if count is \ge 2 and bandroll \ge 2*5, bet the double amount 
        if (this->count >= 2 && bankroll >= 2*minimum){
            return (int)(2*minimum);
        }
        else{
            return (int)minimum;
        }
    }
    bool draw(Card dealer, const Hand &player) override{
        return player_draw(dealer,player);
    }
};

/// global variable
static simple_Player simple_Player;
static counting_Player counting_Player;
Player *get_Simple(){
    return &simple_Player;
}
Player* get_Counting(){
    return &counting_Player;
}
