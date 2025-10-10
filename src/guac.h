#pragma once
#include <qlist.h>
#include <qstring.h>

namespace guac_utils
{
	inline QStringList decode(const QString& input)
	{
		QStringList sections;

		int pos = -1;

		while (true)
		{
			qsizetype len = input.indexOf('.', pos + 1);

			if (len == -1)
				break;

			int length = input.mid(pos + 1, len - pos - 1).toInt();

			pos = length + len + 1;

			// Check if message length is invalid
			if (pos > input.size())
				return {};

			sections.push_back(input.mid(len + 1, length));

			QChar sep = (pos < input.size()) ? input[pos] : '\0';

			if (sep == ',')
				continue;
			if (sep == ';')
				break;
			return {};
		}

		return sections;
	}

	inline QString encode(const QStringList& strings) {
		QString command;

		for (int i = 0; i < strings.size(); ++i) {
			const QString& current = strings[i];
			command += QString::number(current.length()) % '.' % current;
			command += (i < strings.size() - 1) ? ',' : ';';
		}

		return command;
	}
}
