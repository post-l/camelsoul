/*
** MainWindow.hpp for camelsoul in /home/post_l/Dropbox/Programmation/camelsoul
**
** Made by ludovic post
** Login   <post_l@epitech.net>
**
** Started on  Mon Nov 12 21:49:51 2012 ludovic post
// Last update Sat Mar 23 22:41:53 2013 ludovic post
*/

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

  QAction	*showListAction;
  QAction	*quitAction;
  QAction	*manageAccountAction;
  QAction	*aboutAction;
  QAction	*addBuddyAction;
  QAction	*deleteBuddyAction;

  QMenu			*trayIconMenu;
  QSystemTrayIcon	*trayIcon;

  QTabWidget		*_tabWidget;
  QWidget		*_mainTabWidget;

  QListWidget		*_contact_list;
  QComboBox		*_current_status;

  ManageAccountDialog	*_manageAccountDlg;
  Client		*_client;

  QIcon			_availableImg;
  QIcon			_awayImg;
  QIcon			_offlineImg;

  QString		_me;
};

#endif
