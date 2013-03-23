//
// MainWindow.cpp for camelsoul in /home/post_l/Dropbox/Programmation/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Mon Nov 12 21:50:09 2012 ludovic post
// Last update Wed Feb 20 12:00:50 2013 ludovic post
//

#include "MainWindow.hpp"
#include "ChatWidget.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    _availableImg(":/images/available.png"),
    _awayImg(":/images/away.png"),
    _offlineImg(":/images/offline.png")
{
  m_manageAccountDlg = new ManageAccountDialog(this);
  m_client = new Client(this);
  createConnectActions();
  createTrayIcon();
  // --------------- Menu Bar -------------------
  QMenu		*buddiesMenu = menuBar()->addMenu("&Buddies");
  buddiesMenu->addAction(addBuddyAction);
  buddiesMenu->addAction(deleteBuddyAction);
  buddiesMenu->addSeparator();
  buddiesMenu->addAction(quitAction);
  QMenu		*accountsMenu = menuBar()->addMenu("&Account");
  accountsMenu->addAction(manageAccountAction);
  QMenu		*toolsMenu = menuBar()->addMenu("&Tools");
  (void)toolsMenu;
  QMenu		*helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  // ------------ Central Widget -----------------
  m_tabWidget = new QTabWidget;
  m_tabWidget->setTabsClosable(true);
  m_tabWidget->setMovable(true);
  connect(m_tabWidget, SIGNAL(tabCloseRequested(int)),
	  this, SLOT(tabClose(int)));
  //------------- Main Tab Widget ---------------
  m_mainTabWidget = new QWidget;
  QVBoxLayout	*mainLayout = new QVBoxLayout;
  m_contact_list = new QListWidget;
  m_contact_list->setSortingEnabled(true);
  connect(m_contact_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
	  this, SLOT(contactSelected(QListWidgetItem*)));
  mainLayout->addWidget(m_contact_list);
  m_current_status = new QComboBox;
  m_current_status->addItem(_availableImg, "Available");
  m_current_status->addItem(_awayImg, "Away");
  m_current_status->addItem(_offlineImg, "Offline");
  connect(m_current_status, SIGNAL(currentIndexChanged(int)),
	  m_client, SLOT(changeStatus(int)));
  mainLayout->addWidget(m_current_status);
  m_mainTabWidget->setLayout(mainLayout);
  m_tabWidget->addTab(m_mainTabWidget, "&Buddy List");
  //----------------------------------------------
  setCentralWidget(m_tabWidget);
  setIcon();
  trayIcon->show();
  readSettings();
  setWindowTitle(tr("Camelsoul"));
}

void MainWindow::connected()
{
  QSettings	settings;

  qDebug() << "Connected";
  m_current_status->setCurrentIndex(0);
  QString	contacts_str = settings.value("account/contacts").toString();
  if (!contacts_str.isEmpty())
    {
      QStringList	contact_list = contacts_str.split(',');;
      foreach (const QString &contact, contact_list)
	{
	  QListWidgetItem	*item = new QListWidgetItem;
	  item->setText(contact);
	  item->setIcon(_offlineImg);
	  m_contact_list->addItem(item);
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
  trayIcon->showMessage("Disconnected", "You've been disconnected !",
			QSystemTrayIcon::MessageIcon(2), 4000);
  m_current_status->setCurrentIndex(2);
  m_contact_list->clear();
}

void MainWindow::handleError(QString error)
{
  qDebug() << "ERROR :" << error;
  trayIcon->showMessage("Disconnected", "You've been disconnected !\n" + error
			+ "\nReconnection in 10 sec...",
			QSystemTrayIcon::MessageIcon(2), 4000);
  m_current_status->setCurrentIndex(2);
  m_contact_list->clear();
  if (error != "User identification fail")
    QTimer::singleShot(10000, m_client, SLOT(connectMe()));
}

void MainWindow::handleBuddyMessage(const QString &buddy, const QString &msg)
{
  qDebug() << buddy << ": " << msg;
  trayIcon->showMessage(buddy, msg,
			QSystemTrayIcon::MessageIcon(1), 4000);
  for (int i = 0; i != m_tabWidget->count(); ++i)
    if (m_tabWidget->tabText(i) == buddy)
      {
	dynamic_cast<ChatWidget*>(m_tabWidget->widget(i))->appendBuddyMsg(msg);
	return;
      }
  ChatWidget	*newTab = new ChatWidget(buddy, m_me, m_client);
  newTab->appendBuddyMsg(msg);
  m_tabWidget->setCurrentIndex(m_tabWidget->addTab(newTab, buddy));
}

void MainWindow::changeBuddyStatus(const QString &buddy, const QString &state)
{
  qDebug() << buddy << ": " << state;
  for (int i = 0; i < m_contact_list->count(); ++i)
    if (m_contact_list->item(i)->text() == buddy)
      {
	if (state == "actif" || state == NS_LOGIN)
	  m_contact_list->item(i)->setIcon(_availableImg);
	else if (state == "away")
	  m_contact_list->item(i)->setIcon(_awayImg);
	else
	  m_contact_list->item(i)->setIcon(_offlineImg);
	return;
      }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (trayIcon->isVisible())
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
  /*
  QSystemTrayIcon::MessageIcon icon =
    QSystemTrayIcon::MessageIcon(typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
  trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
			durationSpinBox->value() * 1000);
  */
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("Camelsoul About"),
		     tr("Camelsoul is a netsoul messaging client\n"	\
			"Camel is funky\n"				\
			"All camel reserved to post_l"));
}

void MainWindow::addBuddy()
{
  QString	buddy = QInputDialog::getText(this, tr("Add Buddy"),
					      tr("&User name:"));

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
      m_contact_list->addItem(item);
      m_client->refreshBuddyWatch();
    }
}

void MainWindow::deleteBuddy()
{
  QListWidgetItem	*item = m_contact_list->currentItem();
  QSettings		settings;
  QString		buddy = item->text();
  QString		contacts = settings.value("account/contacts").toString();

  settings.setValue("account/contacts", contacts.remove(QRegExp(buddy + ",?|," + buddy + '$')));
  delete m_contact_list->takeItem(m_contact_list->row(item));
  m_client->refreshBuddyWatch();
}

void MainWindow::contactSelected(QListWidgetItem *item)
{
  QString	contact = item->text();
  int		tab_count = m_tabWidget->count();

  for (int i = 0; i != tab_count; i++)
    if (m_tabWidget->tabText(i) == contact)
      {
	m_tabWidget->setCurrentIndex(i);
	return;
      }
  QWidget	*newTab = new ChatWidget(contact, m_me, m_client);
  m_tabWidget->setCurrentIndex(m_tabWidget->addTab(newTab, contact));
}

void MainWindow::tabClose(int index)
{
  QWidget	*widget = m_tabWidget->widget(index);

  if (widget != m_mainTabWidget)
    widget->deleteLater();
}

void MainWindow::setIcon()
{
  QIcon		icon(":/images/main_icon.png");

  trayIcon->setIcon(icon);
  setWindowIcon(icon);
}

void MainWindow::createConnectActions()
{

  showListAction = new QAction(tr("Show Buddy &List"), this);
  connect(showListAction, SIGNAL(triggered()), this, SLOT(changeMainWindowState()));

  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(aboutToQuit()));

  manageAccountAction = new QAction(tr("&Manage Account"),this);
  connect(manageAccountAction, SIGNAL(triggered()), m_manageAccountDlg, SLOT(show()));

  aboutAction = new QAction(tr("&About"), this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  addBuddyAction = new QAction(tr("&Add Buddy"), this);
  connect(addBuddyAction, SIGNAL(triggered()), this, SLOT(addBuddy()));

  deleteBuddyAction = new QAction(tr("&Delete Buddy"), this);
  connect(deleteBuddyAction, SIGNAL(triggered()), this, SLOT(deleteBuddy()));
}

void MainWindow::createTrayIcon()
{
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(showListAction);
  trayIconMenu->addAction(manageAccountAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
  trayIcon->setToolTip(tr("Camelsoul"));
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
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
  m_me = settings.value("account/username").toString();
}
