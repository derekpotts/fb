#include "server.h"

namespace fb
{

	server::server():
		round(1)
	{
	}

	card server::get_cur_card()
	{
		return deck.card_cur_get();
	}

	shared_ptr<player> server::register_player(string session_id,
											   string name)
	{
		for (auto player: players)
			if (player->get_name() == name) {
				push_stuff();
				player->set_session_id(session_id);
				return player;
			}

		auto player_ = make_shared<player>(
			bind(&server::player_update_slot, this, placeholders::_1),
			session_id, name);
		players.push_back(player_);
		player_update_slot(player_.get());

		if(!player_cur) {
			current_player_signal.emit(player_cur = player_.get(), round = 1);
			next_card();
		}

		push_stuff();
		return player_;
	}

	void server::player_update_slot(player* player)
	{
		player_update_signal.emit(player);
	}

	list<shared_ptr<player>> server::get_players()
	{
		return this->players;
	}

	player* server::get_current_player()
	{
		return player_cur;
	}

	uint server::get_round()
	{
		return round;
	}

	void server::deregister_player(shared_ptr<player> player)
	{
		for(auto it = players.begin(); it != players.end(); it++) {
			if(it->get() == player.get()) {
				//if this is the cur player, set the next player as cur
				if(player_cur == player.get())
					next_player();
				//if this is still the cur player, there is no next player
				if(player_cur == player.get())
					current_player_signal.emit(player_cur = 0, round);
				players.erase(it);
				player_remove_signal.emit(it->get());
				push_stuff();
				return;
			}
		}
	}

	void server::apply_vengeance()
	{
		uint high = 0;
		for(auto p: players)
			if(p->get_score() > high)
				high = p->get_score();
		for(auto p: players)
			if(p->get_score() == high)
				p->slash_score();

		push_stuff();
	}

	void server::next_player()
	{
		if(player_cur)
			player_cur->clear_risk();

		for(auto it=players.begin(); it != players.end(); it++) {
			if(it->get() == player_cur) {
				auto next = ++it;
				if(next == players.end()) {
					round++;
					current_player_signal.emit(player_cur = players.front().get(), round);
				} else {
					current_player_signal.emit(player_cur = next->get(), round);
				}
				break;
			}
		}

		next_card();
	}

	void server::add_score(int score, bool take_another)
	{
		if(get_cur_card() == card::VENGEANCE && score > 0)
			apply_vengeance();

		if(get_cur_card() == card::MUST_BUST)
			player_cur->clear_risk();

		if(score)
			player_cur->add_to_score(score);
		else
			player_cur->lose_risk();

		switch(get_cur_card())
		{
			case card::VENGEANCE:
			case card::DOUBLE_TROUBLE:
				if(score)
				{
					player_cur->clear_risk();
					next_card();
				}
				else
					next_player();
				break;
			case card::MUST_BUST:
				next_player();
				break;
			default:
				if(score && take_another) {
					player_cur->add_to_risk(score);
					next_card();
				} else {
					next_player();
				}
				break;
		}
	}

	void server::next_card()
	{
		current_card.emit(deck.draw());

		if(player_cur && get_cur_card() == card::MUST_BUST)
			player_cur->clear_risk();

		if(player_cur && get_cur_card() == card::NO_DICE)
			player_cur->lose_risk();

		push_stuff();
	}

	void server::push_stuff()
	{
		update.emit(player_cur);
	}

	void server::clear_scores()
	{
		for (auto p : players)
		{
			p->clear_score();
			p->clear_risk();
		}

		current_player_signal.emit(
			player_cur = players.front().get(),
			round = 1
			);
		deck.shuffle();
		current_card.emit(deck.draw());

		push_stuff();
	}

}
