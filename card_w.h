#ifndef CARD_W_H
#define CARD_W_H

#include <Wt/WContainerWidget.h>

#include "card.h"

using namespace Wt;

namespace fb
{

	class card_w: public WContainerWidget
	{
		private:
			WLabel* card_label;

		public:
			card_w(card);
			void set_card(card);
	};

}

#endif //CARD_W_H
