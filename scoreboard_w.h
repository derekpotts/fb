#ifndef SCOREBOARD_W_H
#define SCOREBOARD_W_H

#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>
#include <Wt/WTableRow.h>

#include "player.h"

using namespace std;
using namespace Wt;

namespace fb
{

	class scoreboard_player_w: public WTableRow
	{
		public:
			scoreboard_player_w();
			void name_set(string);
	};

	class scoreboard_table_w: public WTable
	{
		public:
			scoreboard_table_w();
			
			scoreboard_player_w* player_add();
			//WTableRow* insertRow(int row, unique_ptr<WTableRow> = nullptr);
	};

	class scoreboard_w: public WContainerWidget
	{
		private:
			vector<player*> players;
			//WTable* table;
			scoreboard_table_w* table;

		public:
			scoreboard_w(list<shared_ptr<player>>, player* current_player);
			void update_player(player*);
			void update_current_player(player*);
			void remove_player(player*);
	};

}

#endif //SCOREBOARD_W_H
