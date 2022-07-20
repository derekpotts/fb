#ifndef PLAYER_H
#define PLAYER_H

#include <Wt/WSignal.h>
#include <functional>
#include <string>

using namespace std;

namespace fb
{

	class player
	{
		private:
			function<void(player*)> update_signal;
			string session_id;
			string name;
			uint score;
			uint risk;

		public:
			player(function<void(player*)>, string session_id, string name);
			player(const player&) = delete;

			string get_session_id();
			void set_session_id(string);

			string get_name();
			void set_name(string);

			uint get_score();
			void set_score(uint);
			void add_to_score(int);
			void slash_score();
			void clear_score();

			uint get_risk();
			void set_risk(uint);
			void add_to_risk(uint);
			void clear_risk();

			void lose_risk();
	};

}

#endif //PLAYER_H
