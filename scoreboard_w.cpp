#include <Wt/WTable.h>
#include <Wt/WText.h>
#include <Wt/WProgressBar.h>
#include <memory>

#include "scoreboard_w.h"

using namespace Wt;

namespace fb
{

	scoreboard_player_w::scoreboard_player_w():
		WTableRow()
	{
	}

	void scoreboard_player_w::name_set(string name)
	{
		auto e = elementAt(0);
		e->clear();
		e->addNew<WText>(name);
	}

	/* **** */

	scoreboard_table_w::scoreboard_table_w():
		WTable()
	{
	}

	scoreboard_player_w* scoreboard_table_w::player_add()
	{
		cout << "player_add\n";
		auto x = new scoreboard_player_w;
		WTable::insertRow(this->rowCount(), unique_ptr<WTableRow>(x));
		return x;
	}

	/* **** */

	scoreboard_w::scoreboard_w(list<shared_ptr<player>> players,
							   player* current_player):
		WContainerWidget()
	{
		//table = addNew<WTable>();
		table = addNew<scoreboard_table_w>();
		table->addStyleClass("scoreboard");

		for(auto player: players) {
			this->players.push_back(player.get());
		}

		uint high_score = 0;
		for(auto player: this->players)
			if(player->get_score() > high_score)
				high_score = player->get_score();

		for(size_t i=0; i<(this->players.size()); i++)
		{
			//auto sbp = table->rowAt(i);
			//auto sbp = std::make_unique<WTableRow>();
			//auto sbp = table->insertRow(i, std::make_unique<WTableRow>());
			auto sbp = table->player_add();

			//table->elementAt(i,0)->addNew<WText>(this->players[i]->get_name());
			//sbp->elementAt(0)->addNew<WText>(this->players[i]->get_name());
			sbp->name_set(this->players[i]->get_name());

			auto score = to_string(this->players[i]->get_score());
			if(this->players[i]->get_risk())
				score += " (" + to_string(this->players[i]->get_risk()) + " at risk)";
			//table->elementAt(i,1)->addNew<WText>(score);
			sbp->elementAt(1)->addNew<WText>(score);

			//auto pbarc = table->elementAt(i,2);
			auto pbarc = sbp->elementAt(2);
			pbarc->addStyleClass("progressbar");
			if(this->players[i]->get_score() == high_score)
				pbarc->addStyleClass("highscore");
			auto pbar = pbarc->addNew<WProgressBar>();
			pbar->setObjectName("pbar");
			pbar->setMaximum(10000);
			pbar->setValue(this->players[i]->get_score());

			//table->elementAt(i,3)->addStyleClass("finishline");
			sbp->elementAt(3)->addStyleClass("finishline");

			if(this->players[i]->get_score() >= 10000)
				//table->elementAt(i,4)->addNew<WText>("&#127942;");
				sbp->elementAt(4)->addNew<WText>("&#127942;");
		}

		for(size_t i=0; i<(this->players.size()); i++) {
			if(this->players[i] == current_player)
				table->rowAt(i)->addStyleClass("current");
		}
	}

	void scoreboard_w::update_player(player* player)
	{
		auto found = false;

		for(size_t i=0; i<players.size(); i++) {
			if(players[i] == player) {
				found = true;

				table->elementAt(i,1)->clear();
				auto score = to_string(players[i]->get_score());
				if(players[i]->get_risk())
					score += " (" + to_string(players[i]->get_risk()) + " at risk)";
				table->elementAt(i,1)->addNew<WText>(score);

				auto pbar = (WProgressBar*)(table->elementAt(i,2)->find("pbar"));
				pbar->setValue(players[i]->get_score());

				if(player->get_score() >= 10000) {
					table->elementAt(i,4)->clear();
					table->elementAt(i,4)->addNew<WText>("&#127942;");
				}
			}
		}

		if(!found) {
			players.push_back(player);
			auto i = players.size() - 1;
			table->elementAt(i,0)->addNew<WText>(this->players[i]->get_name());

			auto score = to_string(this->players[i]->get_score());
			if(this->players[i]->get_risk())
				score += " (" + to_string(this->players[i]->get_risk()) + " at risk)";
			table->elementAt(i,1)->addNew<WText>(score);

			auto pbarc = table->elementAt(i,2);
			pbarc->addStyleClass("progressbar");
			auto pbar = pbarc->addNew<WProgressBar>();
			pbar->setObjectName("pbar");
			pbar->setMaximum(10000);
			pbar->setValue(players[i]->get_score());

			table->elementAt(i,3)->addStyleClass("finishline");

			if(player->get_score() >= 10000)
				table->elementAt(i,4)->addNew<WText>("&#127942;");
		}

		uint high_score = 0;
		for(auto player: players)
			if(player->get_score() > high_score)
				high_score = player->get_score();
		for(size_t i=0; i<players.size(); i++) {
			auto pbar = (WProgressBar*)(table->elementAt(i,2)->find("pbar"));
			if(players[i]->get_score() == high_score)
				pbar->addStyleClass("highscore");
			else
				pbar->removeStyleClass("highscore");
		}
	}

	void scoreboard_w::update_current_player(player* current_player)
	{
		for(size_t i=0; i<(players.size()); i++) {
			if(players[i] == current_player)
				table->rowAt(i)->addStyleClass("current");
			else
				table->rowAt(i)->removeStyleClass("current");
		}
	}

	void scoreboard_w::remove_player(player* player)
	{
		size_t i = 0;
		for(auto it = players.begin(); it != players.end(); i++, it++) {
			if(players[i] == player)
			{
				players.erase(it);
				table->removeRow(i);
				return;
			}
		}
	}

}
