/*
** MainWindow.hpp for camelsoul in /home/post_l/Dropbox/Programmation/camelsoul
**
** Made by ludovic post
** Login   <post_l@epitech.net>
**
** Started on  Mon Nov 12 21:49:51 2012 ludovic post
// Last update Sun Mar 10 09:29:40 2013 ludovic post
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

  QTabWidget		*m_tabWidget;
  QWidget		*m_mainTabWidget;

  QListWidget		*m_contact_list;
  QComboBox		*m_current_status;

  ManageAccountDialog	*m_manageAccountDlg;
  Client		*m_client;

  QIcon			_availableImg;
  QIcon			_awayImg;
  QIcon			_offlineImg;

  QString		m_me;
};

#endif
