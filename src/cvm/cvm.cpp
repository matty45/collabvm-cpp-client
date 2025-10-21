
#include "cvm.h"

namespace cvm
{
	user::user(const QString& username, const rank& rank, const QUrl& server)
	{
		m_rank = rank;
		m_username = username;
		m_server = server;
	}

	vm::vm(const QString& id, const QString& display_name, const QString& thumbnail, const QUrl& server)
	{
		m_id = id;
		m_display_name = display_name;

		// Convert base64 to a bytearray to be loaded into the qpixmap.
		auto result = QByteArray::fromBase64Encoding(thumbnail.toUtf8());
		Q_ASSERT(result.decodingStatus == QByteArray::Base64DecodingStatus::Ok);

		m_thumbnail.loadFromData(result.decoded);

		m_server = server;

	}
}
