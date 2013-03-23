//
// Client.cpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Thu Nov 15 21:23:31 2012 ludovic post
// Last update Sat Mar 23 22:41:29 2013 ludovic post
//

#include <QSettings>
#include <QStringList>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDebug>
#include "Client.hpp"
#include "netsoul_define.hpp"

Client::Client(QObject *window)
  : _last_cmd(NO_CMD)
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

QString Client::url_encode(const QString &str)
{
  QString	encoded;

  for (int n = 0; n < str.length(); ++n)
    {
      if (str[n] <= 47 || str[n] >= 123 || (str[n] >= 48 && str[n] <= 64) || (str[n] >= 91 && str[n] <= 96))
	{
	  ushort tmp = str[n].unicode();
	  if (str[n] > 255)
	    tmp = '?';
	  encoded += '%';
          encoded += QChar((tmp / 16 + ((tmp / 16 > 9) ? 'A' - 10 : '0')));
          encoded += QChar((tmp % 16 + ((tmp % 16 > 9) ? 'A' - 10 : '0')));
	}
      else
	encoded += str[n];
    }
  return (encoded);
}

QString Client::url_decode(const QString &str)
{
  QString	decoded;
  ushort	tmp;
  ushort	t;

  for (int n = 0; n < str.length(); ++n)
    {
      if (str[n] == '%')
	{
	  t = str[++n].unicode();
	  tmp = ((t <= '9') ? t - '0' : ((t <= 'F') ? t - 'A' : t - 'a') + 10) * 16;
	  t = str[++n].unicode();
	  tmp += ((t <= '9') ? t - '0' : ((t <= 'F') ? t - 'A' : t - 'a') + 10);
	  decoded += QChar(tmp);
	}
      else
	decoded += str[n];
    }
  return (decoded);
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
		_last_cmd = NO_CMD;
	      else if (_last_cmd == NB_ASK_AUTH)
		log_cmd(line);
	      else if (_last_cmd == NB_USR_LOG)
		log_end_cmd(line);
	      else if (stringList[0] == NS_SALUT)
		salut_cmd(stringList);
	      else if (stringList[0] == NS_CMD_USR)
		get_usr_cmd(line, stringList[1]);
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

void Client::salut_cmd(const QStringList &stringList)
{
  _hash = stringList[2];
  _host = stringList[3];
  _port = stringList[4];
  _last_cmd = NB_ASK_AUTH;
  sendMessage(NS_ASK_AUTH);
}

void Client::log_cmd(const QString &line)
{
  QCryptographicHash	md5sum(QCryptographicHash::Md5);
  QSettings		settings;

  if (line != NS_CMD_END)
    {
      _last_cmd = NO_CMD;
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
	      + ' ' + url_encode(settings.value("location").toString())
	      + ' ' + url_encode(settings.value("comment").toString()));
  _last_cmd = NB_USR_LOG;
  settings.endGroup();
}

void Client::log_end_cmd(const QString &line)
{
  _last_cmd = NO_CMD;
  if (line == NS_CMD_END)
    {
      sendMessage(QString(NS_STATE) + " actif");
      emit connected();
      refreshBuddyWatch();
    }
  else
    sendMessage(NS_CMD_WHO);
}

void Client::get_usr_cmd(const QString &line, const QString &info)
{
  int	pos = line.lastIndexOf(" | ");

  if (pos != -1)
    {
      QStringList	client_info = info.split(':');
      QStringList	client_cmd = line.mid(pos + 3).split(' ');
      if (client_cmd[0] == NS_CMD_USR_MSG)
	get_message_cmd(client_info, client_cmd);
      else if (client_cmd[0] == NS_STATE || client_cmd[0] == NS_LOGIN || client_cmd[0] == NS_LOGOUT)
	get_watch_cmd(client_info, client_cmd);
      else if (client_cmd[0] == NS_CMD_WHO && client_cmd[1] != "rep")
	get_who_cmd(client_info, client_cmd);
    }
}

void Client::get_watch_cmd(const QStringList &info, const QStringList &cmd)
{
  QString	buddy = info[3].left(info[3].lastIndexOf('@'));
  if (cmd[0] == NS_LOGOUT)
    sendMessage(QString(NS_CMD_USR) + ' ' + NS_CMD_WHO + ' ' + buddy);
  emit buddyStatus(buddy, cmd[1].left(cmd[1].lastIndexOf(':')));
}

void Client::get_who_cmd(const QStringList &info, const QStringList &cmd)
{
  (void)info;
  emit buddyStatus(cmd[2], cmd[11].left(cmd[11].lastIndexOf(':')));
}

void Client::get_message_cmd(const QStringList &info, const QStringList &cmd)
{
  emit readyBuddyMessage(info[3].split('@')[0], url_decode(cmd[1]));
}
