#include <Wt/WLabel.h>

#include "card_w.h"

namespace fb
{

	card_w::card_w(card card):
		WContainerWidget()
	{
		addStyleClass("card_w");

		card_label = addNew<WLabel>();
		set_card(card);
	}

	void card_w::set_card(card card)
	{
		switch(card) {
			case card::BONUS_300:
				card_label->setText("BONUS<br /><span class='f200'>300</span>");
				break;
			case card::BONUS_400:
				card_label->setText("BONUS<br /><span class='f200'>400</span>");
				break;
			case card::BONUS_500:
				card_label->setText("BONUS<br /><span class='f200'>500</span>");
				break;
			case card::DOUBLE_TROUBLE:
				card_label->setText("DOUBLE<br />TROUBLE");
				break;
			case card::FILL_1000:
				card_label->setText("<span class='f150'>FILL<br />1000</span>");
				break;
			case card::MUST_BUST:
				card_label->setText("<span class='f150'>MUST<br />BUST</span>");
				break;
			case card::NO_DICE:
				card_label->setText("<span class='f200'>NO<br />DICE</span>");
				break;
			case card::VENGEANCE:
				card_label->setText("<span class='f75'>VENGEANCE</span><br /><span class='f150'>2500</span>");
				break;
		}
	}

}
