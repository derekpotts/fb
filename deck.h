#ifndef DECK_H
#define DECK_H

#include <random>
#include <algorithm>
#include "card.h"

#include <list>

using namespace std;

namespace fb
{

	class deck
	{
		private:
			random_device rdev;

			vector<card> cards;
			vector<card>::iterator card_cur;

		public:
			deck();
			deck(const deck&) = delete;

			void shuffle();
			card card_cur_get();
			card draw();
	};

}

#endif //DECK_H
