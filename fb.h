#ifndef FB_H
#define FB_H

#include <Wt/WApplication.h>
#include <Wt/WCompositeWidget.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WGlobal.h>
#include <Wt/WStandardItemModel.h>

#include "server.h"
#include "player.h"
#include "scoreboard_w.h"
#include "card_w.h"
#include "score_entry_w.h"

using namespace std;
using namespace Wt;

namespace fb
{

	class fb: public WApplication
	{
		private:
			void unload();
			void finalize();

			server* server_ = 0;
			shared_ptr<player> player_;

			void get_name(WObject*, bool);
			static string get_style_css_url(string);

			WText* vmsize;

			WText* round_display;
			scoreboard_w* scoreboard;
			card_w* card_display;

			score_entry_w* score_entry_widget = 0;

		public:
			fb(const WEnvironment&, server*);

			void update(player*);
			void player_update(player*);
			void current_player(player*, uint);
			void player_remove(player*);
			void update_card(card);
	};

}

#endif //FB_H
