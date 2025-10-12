
#include "cvm.h"

namespace cvm
{
	user::user(const QString& username, const rank& rank)
	{
		m_rank = rank;
		m_username = username;
	}

	vm::vm(const QString& id, const QString& display_name, const QString& thumbnail)
	{
		m_id = id;
		m_display_name = display_name;
		m_thumbnail = thumbnail; //TODO: Could possibly put call to a base64 to image converter function here instead.
	}
}
