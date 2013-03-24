//
// Client.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Thu Nov 15 21:23:45 2012 ludovic post
// Last update Sun Mar 24 16:25:11 2013 ludovic post
//

#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <QObject>
#include <QTcpSocket>

enum		e_cmd
  {
    NO_CMD = 0,
    NB_ASK_AUTH = 1,
    NB_USR_LOG = 2
  };

class Client : public QObject
{
  Q_OBJECT
public:
  Client(QObject *window);
  void			sendMessage(const QString &message);
  void			refreshBuddyWatch();
  static QString	urlEncode(const QString &str);
  static QString	urlDecode(const QString &str);
public slots:
  void		connectMe();
  void		changeStatus(int index);
private slots:
  void		readData();
  void		socketError(QAbstractSocket::SocketError socketError);
signals:
  void		readyMessage(QString message);
  void		readyBuddyMessage(const QString &buddy, const QString &msg);
  void		buddyStatus(const QString &buddy, const QString &status);
  void		connected();
  void		error(QString error);
private:
  void		salutCmd(const QStringList &stringList);
  void		logCmd(const QString &line);
  void		logEndCmd(const QString &line);
  void		getUsrCmd(const QString &line, const QString &info);
  void		getWatchCmd(const QStringList &info, const QStringList &cmd);
  void		getWhoCmd(const QStringList &info, const QStringList &cmd);
  void		getMessageCmd(const QStringList &info, const QStringList &cmd);

  QTcpSocket	*_socket;
  QString	_hash;
  QString	_host;
  QString	_port;
  e_cmd		_lastCmd;
};

#endif
