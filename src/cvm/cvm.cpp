
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


		auto result = QByteArray::fromBase64Encoding(thumbnail.toUtf8());
		Q_ASSERT(result.decodingStatus == QByteArray::Base64DecodingStatus::Ok);

		m_thumbnail.loadFromData(result.decoded);
	}
}
