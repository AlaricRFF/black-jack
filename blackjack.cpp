#include <iostream>
#include "deck.h"
#include "player.h"
#include "rand.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

#define BET_MINIMUM 5
using namespace std;
enum signal {
    endRound,
    stand
};

bool isNatural_21(const Hand &playerHand, const int &wager, int &bankroll) {
    // MODIFIES: bankroll, cout
    // EFFECTS: if dealt with natural 21, add 1.5 wager to bankroll, print to cout and return true
    /// This function can ONLY be called when the player has just dealt exactly TWO cards!
    if (playerHand.handValue().count == 21 && playerHand.handValue().soft) {
        cout << "Player dealt natural 21\n";
        bankroll += (int) (wager * 3 / 2);
        return true;
    }
    return false;
}
bool isBusted(const string& Name, const Hand& thisHand, const int& wager, int& bankroll){
    // MODIFIES: bankroll, cout
    // EFFECTS: if player busts, deduct bankroll with wager, print to cout and return true;
    //          if dealer busts, add bankroll with wager
    if (thisHand.handValue().count > 21){
        cout << Name <<" busts\n";
        if (Name == "Player"){ // player busts
            bankroll -= wager;
        }
        else if(Name == "Dealer"){
            bankroll += wager;
        }else{
            cout << "incorrect name in isBusted()!\n";
            assert(0);
        }
        return true;
    }
    return false;
}

void deck_shuffle(Deck *co_Deck) {
    // MODIFIES: cout, co_deck
    // EFFECTS: shuffle the deck SEVEN times and print the message to cout
    cout << "Shuffling the deck\n";
    for (int i = 0; i < 7; ++i) {
        int cut = get_cut();
        cout << "cut at " << cut << endl;
        co_Deck->shuffle(cut);
    }
}

void deal_card(const string &Name, Hand *thisHand, Deck *co_deck, Player *player) {
    // REQUIRES: Not dealing the hole_card!!! Name can either be "Player" and "Dealer"
    // MODIFIES: cout, thisHand
    // EFFECTS: deal one card, add this card to thisHand, print message to cout.
    /// Expose() to the player
    try{
    Card c = co_deck->deal();
    thisHand->addCard(c);
    if (Name != "Player" && Name != "Dealer") {
        cout << "Wrong name in deal_card()!" << endl;
        assert(0);
    }
    cout << Name << " dealt " << SpotNames[c.spot] << " of " << SuitNames[c.suit] << endl;
    player->expose(c); // expose to the player
    }
    catch (DeckEmpty err){
        cout << "empty deck!" << endl;
        assert(0);
    }
}


int main(int argc, char **argv) {
    // convert input arguments
    int bankroll = atoi(argv[1]), hand = atoi(argv[2]);
    Player *player; // decide whether simple player or counting
    if (strcmp(argv[3], "simple") == 0) {
        player = get_Simple();
    } else {
        player = get_Counting();
    }
    Deck co_Deck;
    Hand playerHand, dealerHand;
    /// Start round, shuffle
    deck_shuffle(&co_Deck);
    player->shuffled();
    /*                 HAND             STARTS                */
    int thisHand = 1;
    for (; thisHand <= hand; ++thisHand) {
        /// Check whether bankroll is enough
        if (bankroll < BET_MINIMUM){ // not enough bankroll, abort the for loop
            break;
        }
        /// Initialize playerHand and dealerHand
        playerHand.discardAll();
        dealerHand.discardAll();
        /// Announce thisHand
        cout << "Hand " << thisHand << " bankroll " << bankroll << endl;
        /// shuffle, if card < 20
        if (co_Deck.cardsLeft() < 20) {
            deck_shuffle(&co_Deck);
            player->shuffled(); // inform player
        }
        /// Player bet
        int wager = player->bet(bankroll, BET_MINIMUM);
        cout << "Player bets " << wager << endl;
        /// Deal four cards, remember EXPOSE()!
        deal_card("Player", &playerHand, &co_Deck, player); // 1st. player deals
        Card dealer_card_exposed = co_Deck.deal();                // 2nd. dealer deals
        cout << "Dealer dealt " << SpotNames[dealer_card_exposed.spot] << " of " << SuitNames[dealer_card_exposed.suit] << endl; // print to cout
        dealerHand.addCard(dealer_card_exposed);                  // 2nd. dealer deals
        player->expose(dealer_card_exposed);                      // expose to player
        deal_card("Player", &playerHand, &co_Deck, player); // 3rd. player deals
        Card hole_card = co_Deck.deal();
        dealerHand.addCard(hole_card); // 4th. hole card deal, not expose
        if (isNatural_21(playerHand, wager, bankroll)) continue; /// END THIS ROUND, immediately after the player has reached 21.
        // cout << "hole card: " << SpotNames[hole_card.spot] << '\t' << SuitNames[hole_card.suit] << endl; /// DEBUG
        /// Player draw until stand or bust
        try {
            while (player->draw(dealer_card_exposed, playerHand)) { // inside loop, player always draws -> bust or natural_21
                deal_card("Player", &playerHand, &co_Deck, player);
            }
            /// Announce player's total
            cout << "Player's total is " << playerHand.handValue().count << endl;
            if (isBusted("Player",playerHand,wager,bankroll)){ // player busts
                enum signal sig = endRound;
                throw sig;
            }
            /// Player Stands, dealer's round
            /// Announce hole card and expose
            cout << "Dealer's hole card is " << SpotNames[hole_card.spot] << " of " << SuitNames[hole_card.suit] << endl;
            player->expose(hole_card); // expose to player
            /// Dealer hits until reaches 17
            while ( dealerHand.handValue().count < 17){
                deal_card("Dealer", &dealerHand, &co_Deck, player);
            }
            cout << "Dealer's total is " << dealerHand.handValue().count << endl;
            if (isBusted("Dealer", dealerHand, wager, bankroll)){ /// dealer busts
                continue; /// END ROUND continue the for loop
            }
            /// No one busts, compare the hand total
            else{
                if (playerHand.handValue().count > dealerHand.handValue().count){
                    cout << "Player wins\n";
                    bankroll += wager;
                    continue; /// END ROUND
                }
                else if (playerHand.handValue().count < dealerHand.handValue().count){
                    cout << "Dealer wins\n";
                    bankroll -= wager;
                    continue; /// END ROUND
                }else{
                    cout << "Push\n";
                    continue; // END ROUND
                }
            }
        }
        catch(enum signal sig){
            if (sig == endRound){
                continue;
            }
        }
    }
    /// Simulation ends, cout the final message
    cout << "Player has " << bankroll << " after " << (thisHand ==1 ? 0:thisHand-1) << " hands\n";
    return 0;
}
