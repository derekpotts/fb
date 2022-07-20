#include "player.h"

namespace fb
{

	player::player(function<void(player*)> update_signal,
				   string session_id, string name):
		update_signal(update_signal),
		session_id(session_id),
		name(name),
		score(0),
		risk(0)
	{
	}

	/* session_id */

	string player::get_session_id()
	{
		return session_id;
	}

	void player::set_session_id(string session_id)
	{
		this->session_id = session_id;
	}

	/* name */

	string player::get_name()
	{
		return name;
	}

	void player::set_name(string name)
	{
		this->name = name;
	}

	/* score */

	uint player::get_score()
	{
		return score;
	}

	void player::set_score(uint score)
	{
		this->score = score;
		update_signal(this);
	}

	void player::add_to_score(int points)
	{
		score += points;
		update_signal(this);
	}

	void player::slash_score()
	{
		score = (score<2500 ? 0 : score -= 2500);
		update_signal(this);
	}

	void player::clear_score()
	{
		score = 0;
		update_signal(this);
	}

	/* risk */

	uint player::get_risk()
	{
		return risk;
	}

	void player::set_risk(uint points)
	{
		risk = points;
		update_signal(this);
	}

	void player::add_to_risk(uint points)
	{
		risk += points;
		update_signal(this);
	}

	void player::clear_risk()
	{
		risk = 0;
		update_signal(this);
	}

	/* */

	void player::lose_risk()
	{
		score -= risk;
		risk = 0;
		update_signal(this);
	}

}
