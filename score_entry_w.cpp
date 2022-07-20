#include <Wt/WCheckBox.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

#include "score_entry_w.h"

namespace fb
{

	score_entry_w::score_entry_w(server* server):
		WContainerWidget(),
		server_(server)
	{
		addStyleClass("score_entry_w");
		auto entry_label = addNew<WLabel>("Score:");
		addNew<WText>(" ");
		entry = addNew<WLineEdit>();
		entry->setStyleClass("entry");
		entry->setAutoComplete(false);
		entry->setAttributeValue("type", "number");
		entry_label->setBuddy(entry);

		action = addNew<WPushButton>("Submit");

		/* connect action button to submit score */
		action->clicked().connect(this, &score_entry_w::submit);
		entry->enterPressed().connect(this, &score_entry_w::submit);

		/* ******** */

		tac_checkbox = addNew<WContainerWidget>();
		tac_checkbox->addStyleClass("tac");
		tac_checkbox->hide();
		auto tac_label = tac_checkbox->addNew<WLabel>("Take Another Card?");
		tac_c = tac_checkbox->addNew<WCheckBox>();
		tac_label->setBuddy(tac_c);

		tac_pushbutton = addNew<WContainerWidget>();
		tac_pushbutton->addStyleClass("tac");
		tac_pushbutton->hide();
		auto tac_b = tac_pushbutton->addNew<WPushButton>("Skip This Card");
		tac_b->clicked().connect([server] {
			server->next_card();
		});

	}

	void score_entry_w::clear()
	{
		entry->setText("");
		tac_c->setChecked(false);
	}

	void score_entry_w::submit()
	{
		server_->add_score(stoi("0" + entry->text()), tac_c->isChecked());
		clear();
	}

	void score_entry_w::set_enabled(bool enabled)
	{
		this->removeStyleClass("nodice");
		tac_checkbox->hide();
		tac_pushbutton->hide();
		disable();

		if(!enabled) return;

		enable();
		entry->enable();
		entry->setFocus();

		switch(server_->get_cur_card())
		{
			case card::VENGEANCE:
				tac_pushbutton->show();
				break;
			case card::DOUBLE_TROUBLE:
			case card::MUST_BUST:
				break;
			case card::NO_DICE:
				entry->disable();
				this->addStyleClass("nodice");
				break;
			default:
				tac_checkbox->show();
				break;
		}
	}

}
