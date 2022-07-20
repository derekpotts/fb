#include "deck.h"

namespace fb
{

	deck::deck()
	{
		for(int i=0; i<12; i++) cards.push_back(card::BONUS_300);
		for(int i=0; i<10; i++) cards.push_back(card::BONUS_400);
		for(int i=0; i<8; i++) cards.push_back(card::BONUS_500);
		for(int i=0; i<8; i++) cards.push_back(card::NO_DICE);
		for(int i=0; i<6; i++) cards.push_back(card::FILL_1000);
		for(int i=0; i<4; i++) cards.push_back(card::MUST_BUST);
		for(int i=0; i<4; i++) cards.push_back(card::VENGEANCE);
		for(int i=0; i<2; i++) cards.push_back(card::DOUBLE_TROUBLE);

		card_cur = cards.end();
	}

	void deck::shuffle()
	{
		std::shuffle(cards.begin(), cards.end(), rdev);
		card_cur = cards.begin();
	}

	card deck::card_cur_get()
	{
		return *card_cur;
	}

	card deck::draw()
	{
		if(card_cur != cards.end())
			card_cur++;

		if(card_cur == cards.end())
			shuffle();

		return card_cur_get();
	}

}
