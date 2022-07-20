#ifndef SERVER_H
#define SERVER_H

#include <Wt/WSignal.h>

#include "player.h"
#include "deck.h"

using namespace std;

namespace fb
{

	class server
	{
		private:
			list<shared_ptr<player>> players;
			player* player_cur = 0;
			deck deck;
			uint round;

			void apply_vengeance();
			void next_player();

			void player_update_slot(player*);

		public:
			server();
			server(const server&) = delete;

			// the old one (still used)
			Wt::Signal<player*> update;

			Wt::Signal<player*> player_update_signal;
			Wt::Signal<player*, uint> current_player_signal;
			Wt::Signal<player*> player_remove_signal;
			Wt::Signal<card> current_card;

			shared_ptr<player> register_player(string session_id, string name);
			void deregister_player(shared_ptr<player>);

			card get_cur_card();
			void add_score(int, bool);
			void next_card();
			void push_stuff();
			void clear_scores();

			list<shared_ptr<player>> get_players();
			player* get_current_player();
			uint get_round();
	};

}

#endif //SERVER_H
