//
// MainWindow.cpp for camelsoul in /home/post_l/Dropbox/Programmation/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Mon Nov 12 21:50:09 2012 ludovic post
// Last update Sun Mar 24 11:52:40 2013 ludovic post
//

#include "MainWindow.hpp"
#include "ChatWidget.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    _availableImg(":/images/available.png"),
    _awayImg(":/images/away.png"),
    _offlineImg(":/images/offline.png")
{
  _manageAccountDlg = new ManageAccountDialog(this);
  _client = new Client(this);
  createConnectActions();
  createTrayIcon();
  // --------------- Menu Bar -------------------
  QMenu		*buddiesMenu = menuBar()->addMenu("&Buddies");
  buddiesMenu->addAction(_addBuddyAction);
  buddiesMenu->addAction(_deleteBuddyAction);
  buddiesMenu->addSeparator();
  buddiesMenu->addAction(_quitAction);
  QMenu		*accountsMenu = menuBar()->addMenu("&Account");
  accountsMenu->addAction(_manageAccountAction);
  QMenu		*toolsMenu = menuBar()->addMenu("&Tools");
  (void)toolsMenu;
  QMenu		*helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(_aboutAction);
  // ------------ Central Widget -----------------
  _tabWidget = new QTabWidget;
  _tabWidget->setTabsClosable(true);
  _tabWidget->setMovable(true);
  connect(_tabWidget, SIGNAL(tabCloseRequested(int)),
	  this, SLOT(tabClose(int)));
  //------------- Main Tab Widget ---------------
  _mainTabWidget = new QWidget;
  QVBoxLayout	*mainLayout = new QVBoxLayout;
  _contactList = new QListWidget;
  _contactList->setSortingEnabled(true);
  connect(_contactList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
	  this, SLOT(contactSelected(QListWidgetItem*)));
  mainLayout->addWidget(_contactList);
  _currentStatus = new QComboBox;
  _currentStatus->addItem(_availableImg, "Available");
  _currentStatus->addItem(_awayImg, "Away");
  _currentStatus->addItem(_offlineImg, "Offline");
  connect(_currentStatus, SIGNAL(currentIndexChanged(int)),
	  _client, SLOT(changeStatus(int)));
  mainLayout->addWidget(_currentStatus);
  _mainTabWidget->setLayout(mainLayout);
  _tabWidget->addTab(_mainTabWidget, "&Buddy List");
  //----------------------------------------------
  setCentralWidget(_tabWidget);
  setIcon();
  _trayIcon->show();
  readSettings();
  setWindowTitle(tr("Camelsoul"));
}

void MainWindow::connected()
{
  QSettings	settings;

  qDebug() << "Connected";
  _currentStatus->setCurrentIndex(0);
  QString	contacts_str = settings.value("account/contacts").toString();
  if (!contacts_str.isEmpty())
    {
      QStringList	contact_list = contacts_str.split(',');;
      foreach (const QString &contact, contact_list)
	{
	  QListWidgetItem	*item = new QListWidgetItem;
	  item->setText(contact);
	  item->setIcon(_offlineImg);
	  _contactList->addItem(item);
	}
    }
}

void MainWindow::readMessage(QString message)
{
  qDebug() << "Serveur: " <<  message;
}

void MainWindow::disconnected()
{
  qDebug() << "Disconnected";
  _trayIcon->showMessage("Disconnected", "You've been disconnected !",
			QSystemTrayIcon::MessageIcon(2), 4000);
  _currentStatus->setCurrentIndex(2);
  _contactList->clear();
}

void MainWindow::handleError(QString error)
{
  qDebug() << "ERROR :" << error;
  _trayIcon->showMessage("Disconnected", "You've been disconnected !\n" + error
			+ "\nReconnection in 10 sec...",
			QSystemTrayIcon::MessageIcon(2), 4000);
  _currentStatus->setCurrentIndex(2);
  _contactList->clear();
  if (error != "User identification fail")
    QTimer::singleShot(10000, _client, SLOT(connectMe()));
}

void MainWindow::handleBuddyMessage(const QString &buddy, const QString &msg)
{
  qDebug() << buddy << ": " << msg;
  _trayIcon->showMessage(buddy, msg,
			QSystemTrayIcon::MessageIcon(1), 4000);
  for (int i = 0; i != _tabWidget->count(); ++i)
    if (_tabWidget->tabText(i) == buddy)
      {
	dynamic_cast<ChatWidget*>(_tabWidget->widget(i))->appendBuddyMsg(msg);
	return;
      }
  ChatWidget	*newTab = new ChatWidget(buddy, _me, _client);
  newTab->appendBuddyMsg(msg);
  _tabWidget->setCurrentIndex(_tabWidget->addTab(newTab, buddy));
}

void MainWindow::changeBuddyStatus(const QString &buddy, const QString &state)
{
  qDebug() << buddy << ": " << state;
  for (int i = 0; i < _contactList->count(); ++i)
    if (_contactList->item(i)->text() == buddy)
      {
	if (state == "actif" || state == NS_LOGIN)
	  _contactList->item(i)->setIcon(_availableImg);
	else if (state == "away")
	  _contactList->item(i)->setIcon(_awayImg);
	else
	  _contactList->item(i)->setIcon(_offlineImg);
	return;
      }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (_trayIcon->isVisible())
    {
      event->ignore();
      hide();
    }
  else
    {
      event->accept();
      aboutToQuit();
    }
}

void MainWindow::aboutToQuit()
{
  writeSettings();
  qApp->quit();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::Trigger
      || reason == QSystemTrayIcon::DoubleClick)
    changeMainWindowState();
}

void MainWindow::changeMainWindowState()
{
  setVisible(!isVisible());
}

void MainWindow::showMessage()
{
  // QSystemTrayIcon::MessageIcon icon =
  //   QSystemTrayIcon::MessageIcon(typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
  // _trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
  // 			durationSpinBox->value() * 1000);
}

void MainWindow::about()
{
  QMessageBox::about(this, "Camelsoul About",
		     "Camelsoul is a netsoul messaging client\n"	\
		     "Camel is funky\n"					\
		     "All camel reserved to post_l");
}

void MainWindow::addBuddy()
{
  QString	buddy = QInputDialog::getText(this, "Add Buddy",
					      "&User name:");

  if (!buddy.isEmpty())
    {
      QListWidgetItem	*item = new QListWidgetItem;
      QSettings		settings;
      QString		contacts = settings.value("account/contacts").toString();
      if (!contacts.isEmpty())
	contacts += ',';
      contacts += buddy;
      settings.setValue("account/contacts", contacts);
      item->setText(buddy);
      item->setIcon(_offlineImg);
      _contactList->addItem(item);
      _client->refreshBuddyWatch();
    }
}

void MainWindow::deleteBuddy()
{
  QListWidgetItem	*item = _contactList->currentItem();
  if (!item)
    return;
  QSettings		settings;
  QString		buddy = item->text();
  QString		contacts = settings.value("account/contacts").toString();

  settings.setValue("account/contacts", contacts.remove(QRegExp(buddy + ",?|," + buddy + '$')));
  delete _contactList->takeItem(_contactList->row(item));
  _client->refreshBuddyWatch();
}

void MainWindow::contactSelected(QListWidgetItem *item)
{
  QString	contact = item->text();
  int		tab_count = _tabWidget->count();

  for (int i = 0; i != tab_count; i++)
    if (_tabWidget->tabText(i) == contact)
      {
	_tabWidget->setCurrentIndex(i);
	return;
      }
  QWidget	*newTab = new ChatWidget(contact, _me, _client);
  _tabWidget->setCurrentIndex(_tabWidget->addTab(newTab, contact));
}

void MainWindow::tabClose(int index)
{
  QWidget	*widget = _tabWidget->widget(index);

  if (widget != _mainTabWidget)
    widget->deleteLater();
}

void MainWindow::setIcon()
{
  QIcon		icon(":/images/main_icon.png");

  _trayIcon->setIcon(icon);
  setWindowIcon(icon);
}

void MainWindow::createConnectActions()
{
  // Find icon for this action
  _showListAction = new QAction("Show Buddy &List", this);
  connect(_showListAction, SIGNAL(triggered()), this, SLOT(changeMainWindowState()));

  _quitAction = new QAction(QIcon::fromTheme("application-exit"),"&Quit", this);
  connect(_quitAction, SIGNAL(triggered()), this, SLOT(aboutToQuit()));

  _manageAccountAction = new QAction(QIcon::fromTheme("document-properties"), "&Manage Account",this);
  connect(_manageAccountAction, SIGNAL(triggered()), _manageAccountDlg, SLOT(show()));

  _aboutAction = new QAction(QIcon::fromTheme("help-about"),"&About", this);
  connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  _addBuddyAction = new QAction(QIcon::fromTheme("list-add"), "&Add Buddy", this);
  connect(_addBuddyAction, SIGNAL(triggered()), this, SLOT(addBuddy()));

  _deleteBuddyAction = new QAction(QIcon::fromTheme("list-remove"), "&Delete Buddy", this);
  connect(_deleteBuddyAction, SIGNAL(triggered()), this, SLOT(deleteBuddy()));
}

void MainWindow::createTrayIcon()
{
  _trayIconMenu = new QMenu(this);
  _trayIconMenu->addAction(_showListAction);
  _trayIconMenu->addAction(_manageAccountAction);
  _trayIconMenu->addSeparator();
  _trayIconMenu->addAction(_quitAction);

  _trayIcon = new QSystemTrayIcon(this);
  _trayIcon->setContextMenu(_trayIconMenu);
  _trayIcon->setToolTip(tr("Camelsoul"));
  connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	  this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::writeSettings()
{
  QSettings	settings;

  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.endGroup();
}

void MainWindow::readSettings()
{
  QSettings	settings;
  QString	contacts_str;

  settings.beginGroup("MainWindow");
  resize(settings.value("size", QSize(400, 300)).toSize());
  move(settings.value("pos", QPoint(200, 200)).toPoint());
  settings.endGroup();
  _me = settings.value("account/username").toString();
}
