//
// Client.cpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Thu Nov 15 21:23:31 2012 ludovic post
// Last update Sun Mar 24 16:24:34 2013 ludovic post
//

#include <QSettings>
#include <QStringList>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDebug>
#include <QByteArray>
#include "Client.hpp"
#include "netsoul_define.hpp"

Client::Client(QObject *window)
  : _lastCmd(NO_CMD)
{
  _socket = new QTcpSocket(this);
  connect(_socket, SIGNAL(readyRead()), this, SLOT(readData()));
  connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
  connect(_socket, SIGNAL(disconnected()), window, SLOT(disconnected()));
  connect(this, SIGNAL(readyMessage(QString)), window, SLOT(readMessage(QString)));
  connect(this, SIGNAL(readyBuddyMessage(const QString&, const QString&)),
	  window, SLOT(handleBuddyMessage(const QString&, const QString&)));
  connect(this, SIGNAL(buddyStatus(const QString&, const QString&)),
	  window, SLOT(changeBuddyStatus(const QString&, const QString&)));
  connect(this, SIGNAL(connected()), window, SLOT(connected()));
  connect(this, SIGNAL(error(QString)), window, SLOT(handleError(QString)));
  connectMe();
}

void Client::sendMessage(const QString &message)
{
  QTextStream	socketInputStream(_socket);

  socketInputStream << message << endl;
  qDebug() << "Client: " << message;
}

void Client::refreshBuddyWatch()
{
  if (_socket->state() == QTcpSocket::ConnectedState)
    {
      QString	contacts = QSettings().value("account/contacts").toString();
      if (!contacts.isEmpty())
	sendMessage(QString(NS_CMD_USR) + ' ' + NS_CMD_WATCH + " {" + contacts + "}\n"
		    + NS_CMD_USR  + ' ' + NS_CMD_WHO + " {" + contacts + '}');
    }
}

QString Client::urlEncode(const QString &str)
{
  return (str.toLatin1().toPercentEncoding().data());
}

QString Client::urlDecode(const QString &str)
{
  return (QByteArray::fromPercentEncoding(str.toLatin1()).data());
}

void		Client::connectMe()
{
  QSettings	settings;

  _socket->abort();
  _socket->connectToHost(settings.value("account/server").toString(),
			  settings.value("account/port").toInt());
}

void		Client::changeStatus(int index)
{
  if (_socket->state() != QTcpSocket::ConnectedState && (!index || index == 1))
    connectMe();
  else if (!index)
    sendMessage(QString(NS_CMD_USR) + ' ' + NS_STATE + " actif");
  else if (index == 1)
    sendMessage(QString(NS_CMD_USR) + ' ' + NS_STATE + " away");
  else
      _socket->abort();
}

void Client::readData()
{
  QString	line;

  while (_socket->canReadLine())
    {
      line = _socket->readLine();
      if (!line.isEmpty())
	{
	  line.resize(line.length() - 1);
	  QStringList	stringList = line.split(' ');
	  if (!stringList.isEmpty())
	    {
	      if (stringList[0] == NS_PING)
		sendMessage(line);
	      else if (line == NS_NO_SUCH_CMD)
		_lastCmd = NO_CMD;
	      else if (_lastCmd == NB_ASK_AUTH)
		logCmd(line);
	      else if (_lastCmd == NB_USR_LOG)
		logEndCmd(line);
	      else if (stringList[0] == NS_SALUT)
		salutCmd(stringList);
	      else if (stringList[0] == NS_CMD_USR)
		getUsrCmd(line, stringList[1]);
	      else if (line == NS_LOGIN_FAIL)
		{
		  _socket->abort();
		  emit error("User identification fail");
		}
	      emit readyMessage(line);
	    }
	}
    }
}

void Client::socketError(QAbstractSocket::SocketError socketError)
{
  (void)socketError;
  emit error(_socket->errorString());
}

void Client::salutCmd(const QStringList &stringList)
{
  _hash = stringList[2];
  _host = stringList[3];
  _port = stringList[4];
  _lastCmd = NB_ASK_AUTH;
  sendMessage(NS_ASK_AUTH);
}

void Client::logCmd(const QString &line)
{
  QCryptographicHash	md5sum(QCryptographicHash::Md5);
  QSettings		settings;

  if (line != NS_CMD_END)
    {
      _lastCmd = NO_CMD;
      return;
    }
  settings.beginGroup("account");
  md5sum.addData(_hash.toAscii());
  md5sum.addData("-");
  md5sum.addData(_host.toAscii());
  md5sum.addData("/");
  md5sum.addData(_port.toAscii() + settings.value("password").toString().toAscii());
  sendMessage(QString(NS_USR_LOG)
	      + ' ' + settings.value("username").toString()
	      + ' ' + md5sum.result().toHex()
	      + ' ' + urlEncode(settings.value("location").toString())
	      + ' ' + urlEncode(settings.value("comment").toString()));
  _lastCmd = NB_USR_LOG;
  settings.endGroup();
}

void Client::logEndCmd(const QString &line)
{
  _lastCmd = NO_CMD;
  if (line == NS_CMD_END)
    {
      sendMessage(QString(NS_STATE) + " actif");
      emit connected();
      refreshBuddyWatch();
    }
  else
    sendMessage(NS_CMD_WHO);
}

void Client::getUsrCmd(const QString &line, const QString &info)
{
  int	pos = line.lastIndexOf(" | ");

  if (pos != -1)
    {
      QStringList	client_info = info.split(':');
      QStringList	client_cmd = line.mid(pos + 3).split(' ');
      if (client_cmd[0] == NS_CMD_USR_MSG)
	getMessageCmd(client_info, client_cmd);
      else if (client_cmd[0] == NS_STATE || client_cmd[0] == NS_LOGIN || client_cmd[0] == NS_LOGOUT)
	getWatchCmd(client_info, client_cmd);
      else if (client_cmd[0] == NS_CMD_WHO && client_cmd[1] != "rep")
	getWhoCmd(client_info, client_cmd);
    }
}

void Client::getWatchCmd(const QStringList &info, const QStringList &cmd)
{
  QString	buddy = info[3].left(info[3].lastIndexOf('@'));
  if (cmd[0] == NS_LOGOUT)
    sendMessage(QString(NS_CMD_USR) + ' ' + NS_CMD_WHO + ' ' + buddy);
  emit buddyStatus(buddy, cmd[1].left(cmd[1].lastIndexOf(':')));
}

void Client::getWhoCmd(const QStringList &info, const QStringList &cmd)
{
  (void)info;
  emit buddyStatus(cmd[2], cmd[11].left(cmd[11].lastIndexOf(':')));
}

void Client::getMessageCmd(const QStringList &info, const QStringList &cmd)
{
  emit readyBuddyMessage(info[3].split('@')[0], urlDecode(cmd[1]));
}
