//
// Client.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Thu Nov 15 21:23:45 2012 ludovic post
// Last update Sat Mar 23 22:41:38 2013 ludovic post
//

#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
  Q_OBJECT
public:
  Client(QObject *window);
  void			sendMessage(const QString &message);
  void			refreshBuddyWatch();
  static QString	url_encode(const QString &str);
  static QString	url_decode(const QString &str);
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
  void		salut_cmd(const QStringList &stringList);
  void		log_cmd(const QString &line);
  void		log_end_cmd(const QString &line);
  void		get_usr_cmd(const QString &line, const QString &info);
  void		get_watch_cmd(const QStringList &info, const QStringList &cmd);
  void		get_who_cmd(const QStringList &info, const QStringList &cmd);
  void		get_message_cmd(const QStringList &info, const QStringList &cmd);

  QTcpSocket	*_socket;
  QString	_hash;
  QString	_host;
  QString	_port;
  int		_last_cmd;
};

#endif
