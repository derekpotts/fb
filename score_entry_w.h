#ifndef SCORE_ENTRY_W_H
#define SCORE_ENTRY_W_H

#include <Wt/WContainerWidget.h>

#include "server.h"

using namespace Wt;

namespace fb
{

	class score_entry_w: public WContainerWidget
	{
		private:
			server* server_;
			WLineEdit* entry = 0;
			WPushButton* action = 0;

			WContainerWidget* tac_checkbox = 0;
			WContainerWidget* tac_pushbutton = 0;
			WCheckBox* tac_c = 0;

			void clear();
			void submit();

		public:
			score_entry_w(server*);
			void set_enabled(bool);
	};

}

#endif //SCORE_ENTRY_W_H
