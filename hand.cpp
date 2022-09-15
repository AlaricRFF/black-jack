#include "hand.h"
#include <cassert>
Hand::Hand() {
    curValue.count = 0; // no spots added
    curValue.soft = false; // no ace obtained
}

void Hand::discardAll() {
    this->curValue.count = 0;
    this->curValue.soft = false;
}

void Hand::addCard(Card c) {
    switch (c.spot) {
        case ACE:{
            if ( this->curValue.soft || this->curValue.count + 11 > 21){ // if choosing "soft" will bust, then choose hard
                this->curValue.count ++;
            }
            else{ // always choose soft count of ace if it won't bust
                this->curValue.count += 11;
                this->curValue.soft = true;
            }
            break;
        }
        case JACK:
        case QUEEN:
        case KING:{
            curValue.count += 10;
            break;
        }
        default:{ // from two to ten
            curValue.count += (c.spot + 2);
            break;
        }
    }
    if (curValue.soft && curValue.count > 21){ // if a hand with soft ace has busted after a card is added, then subtract 10 to make a hard count
        curValue.count -= 10;
        curValue.soft = false;
    }
}

HandValue Hand::handValue() const {
    return curValue;
}