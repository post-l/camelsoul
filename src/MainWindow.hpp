//
// MainWindow.hpp for camelsoul in /home/post_l/git/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Sun Mar 24 11:29:46 2013 ludovic post
// Last update Sun Mar 24 11:36:36 2013 ludovic post
//

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui>
#include "ManageAccountDialog.hpp"
#include "Client.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
public slots:
  void		connected();
  void		readMessage(QString message);
  void		disconnected();
  void		handleError(QString error);
  void		handleBuddyMessage(const QString &buddy, const QString &msg);
  void		changeBuddyStatus(const QString &buddy, const QString &state);
protected:
  virtual void	closeEvent(QCloseEvent *event);
private slots:
  void		aboutToQuit();
  void		iconActivated(QSystemTrayIcon::ActivationReason reason);
  void		changeMainWindowState();
  void		showMessage();
  void		about();
  void		addBuddy();
  void		deleteBuddy();
  void		contactSelected(QListWidgetItem *item);
  void		tabClose(int index);
private:
  void		setIcon();
  void		createConnectActions();
  void		createTrayIcon();
  void		writeSettings();
  void		readSettings();

  QAction	*_showListAction;
  QAction	*_quitAction;
  QAction	*_manageAccountAction;
  QAction	*_aboutAction;
  QAction	*_addBuddyAction;
  QAction	*_deleteBuddyAction;

  QMenu			*_trayIconMenu;
  QSystemTrayIcon	*_trayIcon;

  QTabWidget		*_tabWidget;
  QWidget		*_mainTabWidget;

  QListWidget		*_contactList;
  QComboBox		*_currentStatus;

  ManageAccountDialog	*_manageAccountDlg;
  Client		*_client;

  QIcon			_availableImg;
  QIcon			_awayImg;
  QIcon			_offlineImg;

  QString		_me;
};

#endif
