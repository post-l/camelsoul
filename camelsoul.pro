TARGET          = camelsoul

MOC_DIR         = src/
OBJECTS_DIR     = obj/

HEADERS         = src/MainWindow.hpp                    \
                  src/ChatWidget.hpp                    \
                  src/ManageAccountDialog.hpp           \
                  src/Client.hpp

SOURCES         = src/main.cpp                          \
                  src/MainWindow.cpp                    \
                  src/Client.cpp

fclean.commands = $(RM) $(TARGET)
fclean.depends  = clean

re.depends      = clean fclean all

.PHONY.depends  = clean fclean re all

RESOURCES       += camelsoul.qrc

QT              += network
CONFIG          += release

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Os

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

QMAKE_EXTRA_TARGETS += fclean re .PHONY

target.path     = /usr/bin

INSTALLS        += target
