#include <filesystem>
#include <iostream>
#include <fstream>

#include <Wt/WApplication.h>
#include <Wt/WCheckBox.h>
#include <Wt/WCompositeWidget.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WGlobal.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WProgressBar.h>
#include <Wt/WPushButton.h>
#include <Wt/WServer.h>
#include <Wt/WTableView.h>

#include <Wt/WFlags.h>

#include "lib.h"
#include "fb.h"
#include "card_w.h"
#include "scoreboard_w.h"

namespace fb
{

	fb::fb(const WEnvironment& env, server* server):
		WApplication(env),
		server_(server)
	{
		enableUpdates();
		useStyleSheet(get_style_css_url(env.server()->docRoot()));
		setTitle("Fill Or Bust!");

		/* ******** */

		auto cookied_name = env.getCookie("name");
		if(cookied_name)
			player_ = server_->register_player(sessionId(), *cookied_name);
		else
			get_name(root(), false);

		/* ******** */

		server->update.connect(this, &fb::update);
		server->push_stuff();

		/* ******** */

		auto head = root()->addNew<WContainerWidget>();
		head->addStyleClass("menu");

		auto m_ng = head->addNew<WPushButton>("New Game");
		m_ng->clicked().connect([this] {
			this->server_->clear_scores();
		});

		auto m_cn = head->addNew<WPushButton>("Change Name");
		m_cn->clicked().connect([this] {
			get_name(root(), true);
		});

		vmsize = head->addNew<WText>();

		/* ******** */

		root()->addNew<WText>("Round ");
		round_display = root()->addNew<WText>(to_string(server->get_round()));

		/* ******** */

		scoreboard = root()->addNew<scoreboard_w>(server->get_players(),
												  server->get_current_player()
												 );
		server->player_update_signal.connect(this, &fb::player_update);
		server->current_player_signal.connect(this, &fb::current_player);
		server->player_remove_signal.connect(this, &fb::player_remove);

		/* ******** */

		card_display = root()->addNew<card_w>(server->get_cur_card());
		server->current_card.connect(this, &fb::update_card);

		/* ******** */

		score_entry_widget = root()->addNew<score_entry_w>(server);
	}

	void fb::unload()
	{
	}

	void fb::finalize()
	{
		if(player_ && player_->get_session_id() == sessionId())
			server_->deregister_player(player_);
	}

	string fb::get_style_css_url(string path)
	{
		auto mtime = filesystem::last_write_time(path + "/style.css");
		auto mtimesys = chrono::file_clock::to_sys(mtime);
		auto d = mtimesys.time_since_epoch();
		auto ds = chrono::duration_cast<chrono::seconds>(d);

		return "style.css?v=" + to_string(ds.count());
	}

	void fb::update(player* cur)
	{
		environment().server()->post(this->sessionId(), [this, cur] {
			vmsize->setText("vmsize:" + to_string(vmsize_get()));
			score_entry_widget->set_enabled(
				(cur && player_) ? player_.get() == cur : false);
			this->triggerUpdate();
		});
	}

	void fb::player_update(player* player)
	{
		environment().server()->post(this->sessionId(), [this, player] {
			scoreboard->update_player(player);
			this->triggerUpdate();
		});
	}
	void fb::current_player(player* player, uint round)
	{
		environment().server()->post(this->sessionId(), [this, player, round] {
			round_display->setText(to_string(round));
			scoreboard->update_current_player(player);
			this->triggerUpdate();
		});
	}
	void fb::player_remove(player* player)
	{
		environment().server()->post(this->sessionId(), [this, player] {
			scoreboard->remove_player(player);
			this->triggerUpdate();
		});
	}
	void fb::update_card(card card)
	{
		environment().server()->post(this->sessionId(), [this, card] {
			card_display->set_card(card);
			this->triggerUpdate();
		});
	}

	void fb::get_name(WObject* owner, bool allow_cancel)
	{
		auto dialog = owner->addChild(make_unique<WDialog>());

		auto label = dialog->contents()->addNew<WLabel>("Who are you?");
		auto edit = dialog->contents()->addNew<WLineEdit>();
		edit->setMaxLength(8);
		edit->setInputMask("Nnnnnnnn");
		label->setBuddy(edit);

		auto save = dialog->footer()->addNew<WPushButton>("Save");
		save->setDefault(true);
		save->clicked().connect(dialog, &WDialog::accept);

		if(allow_cancel)
		{
			auto cancel = dialog->footer()->addNew<WPushButton>("Cancel");
			cancel->clicked().connect(dialog, &WDialog::reject);
			dialog->rejectWhenEscapePressed();
		}

		dialog->finished().connect([=] {
			if(dialog->result() == DialogCode::Accepted)
			{
				if(edit->text() == "")
				{
					dialog->show();
					return;
				}

				if(player_)
				{
					player_->set_name(edit->text().toUTF8());
					server_->push_stuff();
				}
				else
				{
					player_ = server_->register_player(sessionId(),
													   edit->text().toUTF8());
				}

				setCookie("name", edit->text().toUTF8(), INT_MAX);
			}
		});

		dialog->show();
	}

}
