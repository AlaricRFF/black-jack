#include "deck.h"
#include <iostream>
#include <cassert>
using namespace std;

Deck::Deck() {
    int suit_type = 0; // suit_type divided by 13 is the enum value of Suit
    int spot_type = 0; // spot_type % 13 is the enum value of Spot
    // modifies deck
    for (int card_index = 0; card_index < DeckSize; ++card_index) {
        Card temp;
        temp.suit = (Suit) (suit_type / 13);
        temp.spot = (Spot) (spot_type % 13);
        deck[card_index] = temp;
        suit_type++;
        spot_type++;
    }
    // modifies next
    next = 0; // point to the first card in the deck[].
}

void Deck::reset() {
    int suit_type = 0; 
    int spot_type = 0; 
    // modifies deck
    for (int card_index = 0; card_index < DeckSize; ++card_index) {
        Card temp;
        temp.suit = (Suit) (suit_type / 13);
        temp.spot = (Spot) (spot_type % 13);
        deck[card_index] = temp;
        suit_type++;
        spot_type++;
    }
    // modifies next
    next = 0;
}

Card Deck::deal() {
    // check whether remaining card exist
    if (next >= DeckSize ) {// the last card has been dealt already
        DeckEmpty error;
        throw error;
    }
    next ++; // renew the next card's state
    return deck[next - 1]; // return the card dealt
}

void Deck::shuffle(int n) {
    next = 0; /// First, relocate the card pointer to the first card
    // boundary: either left or right is empty
    if (n == 0 || n == 52){
        return; // no need to modify the deck[], because it's an ineffective cut
    }
    Card* left = new Card[n];
    Card* right = new Card[DeckSize-n]; // dynamically locate the array
    for (int i = 0; i < DeckSize; ++i) {
        if (i<n){ // sort it to the left pile of card
            left[i] = deck[i];
        }
        else{
            right[i-n] = deck[i]; // sort to the right part
        }
    }
    int deck_index = -1; // to balance the first "++" operator
    int left_index = 0,right_index = 0;
    for (;(left_index < n && right_index < DeckSize-n); ++left_index,++right_index) {
        deck[++deck_index] = right[right_index];
        deck[++deck_index] = left[left_index];
    }
    if (left_index == n){ // the left has been consumed, use the right part
        for (int cur_Deck = deck_index + 1; cur_Deck < DeckSize; ++cur_Deck) {
            deck[cur_Deck] = right[right_index++];
        }
    }
    else{
        for (int cur_Deck = deck_index + 1; cur_Deck < DeckSize; ++cur_Deck) {
            deck[cur_Deck] = left[left_index++];
        }
    }
    delete [] left; // de-locate the array
    delete [] right;
}

int Deck::cardsLeft() {
    return DeckSize - next;
}

/*
/// DEBUG CODE
void Deck::showDeck_DEBUG() const {
    int i=1;
    for (const auto& e:deck){
        cout << i << ": "<<SuitNames[e.suit] << '\t' << SpotNames[e.spot] << endl;
        i++;
    }
    cout << "next= " << next << endl;
    cout << "-------------partition line-------------" << endl;
}

Deck::Deck(DEBUG message) {
    // only include spade into the deck
    for (int i = 0; i < 13; ++i) {
        Card temp{};
        temp.spot = (Spot) i;
        temp.suit = SPADES;
        deck[i] = temp;
    }
    next = 0;

}
*/
