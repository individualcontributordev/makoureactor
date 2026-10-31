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
#include "DuckStation.h"
#include "Config.h"
#include <QtNetwork>

QByteArray DuckStation::sessionId;

QString DuckStation::url()
{
	return Config::value("DuckStationUrl").toString();
}

void DuckStation::setUrl(const QString &url)
{
	sessionId.clear();
	Config::setValue("DuckStationUrl", url.isEmpty() ? QVariant() : url);
}

bool DuckStation::post(const QJsonObject &request, QJsonObject &reply, QString &error)
{
	QNetworkRequest req{QUrl(url())};
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	req.setRawHeader("Accept", "application/json, text/event-stream");
	req.setTransferTimeout(30000);
	if (!sessionId.isEmpty()) {
		req.setRawHeader("MCP-Session-Id", sessionId);
	}

	QNetworkAccessManager manager;
	QNetworkReply *response = manager.post(req, QJsonDocument(request).toJson(QJsonDocument::Compact));
	QEventLoop loop;
	QObject::connect(response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec(QEventLoop::ExcludeUserInputEvents);

	const QByteArray body = response->readAll();
	const QByteArray newSession = response->rawHeader("MCP-Session-Id");
	const QNetworkReply::NetworkError netError = response->error();
	error = response->errorString();
	response->deleteLater();

	if (!newSession.isEmpty()) {
		sessionId = newSession;
	}
	if (netError != QNetworkReply::NoError) {
		return false;
	}
	reply = QJsonDocument::fromJson(body).object(); // notifications get an empty 202
	return true;
}

bool DuckStation::openSession(QString &server, QString &error)
{
	sessionId.clear();
	QJsonObject reply;
	if (!post({
	        {"jsonrpc", "2.0"}, {"id", 1}, {"method", "initialize"},
	        {"params", QJsonObject{
	            {"protocolVersion", "2025-03-26"},
	            {"capabilities", QJsonObject()},
	            {"clientInfo", QJsonObject{{"name", MAKOU_REACTOR_NAME}, {"version", MAKOU_REACTOR_VERSION}}}
	        }}
	    }, reply, error)) {
		return false;
	}
	const QJsonObject info = reply.value("result").toObject().value("serverInfo").toObject();
	if (info.isEmpty()) {
		error = QObject::tr("Not a DuckStation MCP server");
		return false;
	}
	server = info.value("name").toString() % ' ' % info.value("version").toString();

	QJsonObject ignored;
	return post({{"jsonrpc", "2.0"}, {"method", "notifications/initialized"}}, ignored, error);
}

bool DuckStation::connect(QString &server, QString &error)
{
	return openSession(server, error);
}

bool DuckStation::callTool(const QString &tool, const QJsonObject &arguments, QString &error)
{
	const QJsonObject request{
	    {"jsonrpc", "2.0"}, {"id", 2}, {"method", "tools/call"},
	    {"params", QJsonObject{{"name", tool}, {"arguments", arguments}}}
	};
	QJsonObject reply;
	bool ok = !sessionId.isEmpty() && post(request, reply, error);
	if (!ok) { // no session yet, or it died with DuckStation: reopen once
		QString server;
		if (!openSession(server, error) || !post(request, reply, error)) {
			return false;
		}
	}

	if (reply.contains("error")) {
		error = reply.value("error").toObject().value("message").toString();
		return false;
	}
	const QJsonObject result = reply.value("result").toObject();
	if (result.value("isError").toBool()) {
		error = result.value("content").toArray().at(0).toObject().value("text").toString();
		return false;
	}
	return true;
}

bool DuckStation::shutdown(QString &error)
{
	return callTool("shutdown_system", {{"save_resume_state", false}}, error);
}

bool DuckStation::boot(const QString &path, QString &error)
{
	return callTool("boot_game", {{"path", path}}, error);
}
