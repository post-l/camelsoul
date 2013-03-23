//
// main.cpp for camelsoul in /home/post_l/Dropbox/Programmation/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Mon Nov 12 21:27:48 2012 ludovic post
// Last update Tue Nov 13 22:04:19 2012 ludovic post
//

#include <QtGui>
#include <cstdlib>
#include "MainWindow.hpp"

int		main(int ac, char *av[])
{
  Q_INIT_RESOURCE(camelsoul);
  QCoreApplication::setOrganizationName("42Soft");
  QCoreApplication::setOrganizationDomain("42soft.com");
  QCoreApplication::setApplicationName("Camelsoul");
  QApplication::setQuitOnLastWindowClosed(false);
  QApplication	app(ac, av);
  if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
      QMessageBox::critical(0, QObject::tr("Systray"),
			    QObject::tr("I couldn't detect any system tray "
					"on this system."));
      return (EXIT_FAILURE);
    }
  MainWindow	window;
  window.show();
  return (app.exec());
}
