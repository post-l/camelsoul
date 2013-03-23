HEADERS         = MainWindow.hpp                \
                  ChatWidget.hpp                \
                  ManageAccountDialog.hpp       \
                  Client.hpp

SOURCES         = main.cpp                      \
                  MainWindow.cpp                \
                  Client.cpp

fclean.commands = $(RM) $(TARGET)
fclean.depends = clean

re.depends = clean fclean all

.PHONY.depends = clean fclean re all

RESOURCES       = camelsoul.qrc

QT              += core gui xml svg network
CONFIG          += release

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Os

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

QMAKE_EXTRA_TARGETS += fclean re .PHONY
