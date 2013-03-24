//
// ChatWidget.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Sun Dec  2 23:51:23 2012 ludovic post
// Last update Sun Mar 24 12:24:08 2013 ludovic post
//

#ifndef	__CHATWIDGET_HPP__
#define	__CHATWIDGET_HPP__

#include <QtGui>
#include "netsoul_define.hpp"
#include "Client.hpp"

class ChatWidget : public QWidget
{
  Q_OBJECT
public:
  ChatWidget(const QString &buddy, const QString &me, Client *client, QWidget *parent = 0)
    : QWidget(parent),
      _buddy(buddy),
      _me(me),
      _client(client)
  {
    QVBoxLayout	*mainLayout = new QVBoxLayout;
    _conversation = new QTextEdit;
    _conversation->setReadOnly(true);
    _sendText = new QLineEdit;
    connect(_sendText, SIGNAL(returnPressed()), this, SLOT(handleSendText()));
    mainLayout->addWidget(_conversation);
    mainLayout->addWidget(_sendText);
    // mainLayout->setStretch(0, 4);
    // mainLayout->setStretch(1, 1);
    setLayout(mainLayout);
  }

  void		appendBuddyMsg(const QString &msg)
  {
    QTime time = QTime::currentTime();
    _conversation->append("<b><font color=\"#D00000\">(" + time.toString() + ") "
			  + _buddy + ": </font></b>" + msg);
  }
private slots:
  void		handleSendText()
  {
    if (!_sendText->text().isEmpty())
      {
	QTime time = QTime::currentTime();
	_conversation->append("<b><font color=\"#3333CC\">(" + time.toString() + ") " +
			      _me + ": </font></b>" + _sendText->text());
	_client->sendMessage(QString(NS_CMD_USR) + ' ' + NS_CMD_MSG_USR + ' '
			      + _buddy + ' ' + NS_CMD_USR_MSG + ' '
			      + Client::url_encode(_sendText->text()));
	_sendText->clear();
      }
    _sendText->setFocus();
  }
private:
  QTextEdit	*_conversation;
  QLineEdit	*_sendText;
  QString	_buddy;
  QString	_me;
  Client	*_client;
};

#endif
