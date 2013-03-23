//
// ManageAccountDialog.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Tue Nov 13 22:59:02 2012 ludovic post
// Last update Tue Dec  4 21:31:35 2012 ludovic post
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
    usernameEdit = new QLineEdit(settings.value("username").toString());
    passwordEdit = new QLineEdit(settings.value("password").toString());
    passwordEdit->setEchoMode(QLineEdit::Password);
    QFormLayout	*basicBox = new QFormLayout;
    basicBox->addRow("&Username", usernameEdit);
    basicBox->addRow("&Password", passwordEdit);
    basic->setLayout(basicBox);

    QWidget	*advanced = new QWidget;
    serverEdit = new QLineEdit(settings.value("server", "ns-server.epita.fr").toString());
    portEdit = new QLineEdit(settings.value("port", "4242").toString());
    locationEdit = new QLineEdit(settings.value("location", "maison").toString());
    commentEdit = new QLineEdit(settings.value("comment", "Camelsoul").toString());
    QFormLayout	*advancedBox = new QFormLayout;
    advancedBox->addRow("&Server: ", serverEdit);
    advancedBox->addRow("&Port: ", portEdit);
    advancedBox->addRow("&Location: ", locationEdit);
    advancedBox->addRow("&Comment: ", commentEdit);
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
    settings.setValue("username", usernameEdit->text());
    settings.setValue("password", passwordEdit->text());
    settings.setValue("server", serverEdit->text());
    settings.setValue("port", portEdit->text());
    settings.setValue("location", locationEdit->text());
    settings.setValue("comment", commentEdit->text());
    settings.endGroup();
    hide();
  }
private:
  QLineEdit	*usernameEdit;
  QLineEdit	*passwordEdit;
  QLineEdit	*serverEdit;
  QLineEdit	*portEdit;
  QLineEdit	*locationEdit;
  QLineEdit	*commentEdit;
};

#endif
