//
// ChatWidget.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Sun Dec  2 23:51:23 2012 ludovic post
// Last update Sat Mar  9 14:13:00 2013 ludovic post
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
      m_buddy(buddy),
      m_me(me),
      m_client(client)
  {
    QVBoxLayout	*mainLayout = new QVBoxLayout;
    m_conversation = new QTextEdit;
    m_conversation->setReadOnly(true);
    m_sendText = new QLineEdit;
    connect(m_sendText, SIGNAL(returnPressed()), this, SLOT(handleSendText()));
    mainLayout->addWidget(m_conversation);
    mainLayout->addWidget(m_sendText);
    /*
    mainLayout->setStretch(0, 4);
    mainLayout->setStretch(1, 1);
    */
    setLayout(mainLayout);
  }

  void		appendBuddyMsg(const QString &msg)
  {
    m_conversation->append(m_buddy + ": " + msg);
  }
private slots:
  void		handleSendText()
  {
    if (!m_sendText->text().isEmpty())
      {
	m_conversation->append(m_me + ": " + m_sendText->text());
	m_client->sendMessage(QString(NS_CMD_USR) + ' ' + NS_CMD_MSG_USR + ' '
			      + m_buddy + ' ' + NS_CMD_USR_MSG + ' '
			      + Client::url_encode(m_sendText->text()));
	m_sendText->clear();
      }
    m_sendText->setFocus();
  }
private:
  QTextEdit	*m_conversation;
  QLineEdit	*m_sendText;
  QString	m_buddy;
  QString	m_me;
  Client	*m_client;
};

#endif
