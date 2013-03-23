//
// ManageAccountDialog.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Tue Nov 13 22:59:02 2012 ludovic post
// Last update Sat Mar 23 22:43:41 2013 ludovic post
//

#ifndef	__MANAGEACCOUNTDIALOG_HPP__
#define	__MANAGEACCOUNTDIALOG_HPP__

#include <QDialog>
#include <QLineEdit>
#include <QSettings>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

class ManageAccountDialog : public QDialog
{
  Q_OBJECT
public:
  ManageAccountDialog(QWidget *parent = 0)
    : QDialog(parent)
  {
    QSettings	settings;
    settings.beginGroup("account");
    QTabWidget	*onglets = new QTabWidget(this);
    QWidget	*basic = new QWidget;
    _usernameEdit = new QLineEdit(settings.value("username").toString());
    _passwordEdit = new QLineEdit(settings.value("password").toString());
    _passwordEdit->setEchoMode(QLineEdit::Password);
    QFormLayout	*basicBox = new QFormLayout;
    basicBox->addRow("&Username", _usernameEdit);
    basicBox->addRow("&Password", _passwordEdit);
    basic->setLayout(basicBox);

    QWidget	*advanced = new QWidget;
    _serverEdit = new QLineEdit(settings.value("server", "ns-server.epita.fr").toString());
    _portEdit = new QLineEdit(settings.value("port", "4242").toString());
    _locationEdit = new QLineEdit(settings.value("location", "maison").toString());
    _commentEdit = new QLineEdit(settings.value("comment", "Camelsoul").toString());
    QFormLayout	*advancedBox = new QFormLayout;
    advancedBox->addRow("&Server: ", _serverEdit);
    advancedBox->addRow("&Port: ", _portEdit);
    advancedBox->addRow("&Location: ", _locationEdit);
    advancedBox->addRow("&Comment: ", _commentEdit);
    advanced->setLayout(advancedBox);

    onglets->addTab(basic, "&Basic");
    onglets->addTab(advanced, "&Advanced");

    QPushButton	*closeBtn = new QPushButton("&Cancel");
    QPushButton	*saveBtn = new QPushButton("&Save");
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveConfig()));
    QHBoxLayout	*btnLayout = new QHBoxLayout;
    btnLayout->addWidget(closeBtn);
    btnLayout->addWidget(saveBtn);
    QVBoxLayout	*mainLayout= new QVBoxLayout;
    mainLayout->addWidget(onglets);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
    settings.endGroup();
  }
private slots:
  void		saveConfig()
  {
    QSettings	settings;

    settings.beginGroup("account");
    settings.setValue("username", _usernameEdit->text());
    settings.setValue("password", _passwordEdit->text());
    settings.setValue("server", _serverEdit->text());
    settings.setValue("port", _portEdit->text());
    settings.setValue("location", _locationEdit->text());
    settings.setValue("comment", _commentEdit->text());
    settings.endGroup();
    hide();
  }
private:
  QLineEdit	*_usernameEdit;
  QLineEdit	*_passwordEdit;
  QLineEdit	*_serverEdit;
  QLineEdit	*_portEdit;
  QLineEdit	*_locationEdit;
  QLineEdit	*_commentEdit;
};

#endif
