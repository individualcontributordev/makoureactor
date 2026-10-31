/****************************************************************************
 ** Makou Reactor Final Fantasy VII Field Script Editor
 ** Copyright (C) 2009-2022 Arzel Jérôme <myst6re@gmail.com>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#pragma once

#include <QtCore>

// Blocking client for DuckStation's built-in MCP server (JSON-RPC over HTTP).
// Attached = a server URL is stored in the config.
class DuckStation
{
public:
	static QString url();
	static void setUrl(const QString &url);
	static bool attached() { return !url().isEmpty(); }
	// Opens a session; on success `server` is the server name and version
	static bool connect(QString &server, QString &error);
	// Releases the disc image file so it can be written
	static bool shutdown(QString &error);
	static bool boot(const QString &path, QString &error);
private:
	static bool post(const QJsonObject &request, QJsonObject &reply, QString &error);
	static bool openSession(QString &server, QString &error);
	static bool callTool(const QString &tool, const QJsonObject &arguments, QString &error);
	static QByteArray sessionId;
};
