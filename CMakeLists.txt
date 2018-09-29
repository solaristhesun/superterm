cmake_minimum_required(VERSION 3.1.0)

project(superterm)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "~/qt/5.10.1/gcc_64/lib/cmake")

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Gui Widgets Network SerialPort REQUIRED)

# Populate a CMake variable with the sources
set(superterm_SRCS
	contrib/waitingspinnerwidget/waitingspinnerwidget.cpp
	contrib/singleapplication/singleapplication.cpp
    src/session/session.cpp
    src/session/serializableobject.cpp
    src/ipc/message.cpp
	src/ipc/messagecodec.cpp
	src/ui/widgets/clickablelabel.cpp
	src/ui/consoletabbar.cpp
	src/ui/consoleview/consoleview.cpp
	src/ui/consoleview/consolelineitemdelegate.cpp
	src/ui/statusbarframe.ui
	src/ui/logfilepanel.cpp
	src/ui/renametabframe.ui
	src/ui/consoletabfactory.cpp
	src/ui/connectionbar.cpp
	src/ui/portscombobox.cpp
	src/ui/consoletab.cpp
	src/ui/statusbarframe.cpp
	src/ui/mainwindow.ui
	src/ui/consoletab.ui
	src/ui/consoleview/consoleview.ui
	src/ui/portitemdelegate.cpp
	src/ui/portitemview.cpp
	src/ui/portitemview.ui
	src/ui/notificationbar.ui
	src/ui/notificationbar.cpp
	src/ui/highlightingsframe.cpp
	src/ui/renametabframe.cpp
	src/ui/connectionbar.ui
	src/ui/logfilepanel.ui
	src/ui/consoletabwidget.cpp
	src/ui/mainwindow.cpp
	src/ui/highlightingsframe.ui
	src/misc/defaultsettings.cpp
	src/misc/globals.cpp
	src/misc/softwareversion.cpp
	src/misc/updatechecker.cpp
	src/misc/uniqueidentifier.cpp
	src/main.cpp
	src/serial/portenumerator.cpp
	src/serial/portendpoint.cpp
	src/serial/serialportinfo.cpp
	src/serial/portapplication.cpp
	src/serial/portobserver.cpp
	src/models/consolelinebuffer.cpp
	src/models/consoleline.cpp
	superterm.qrc
	superterm.qss
)
include_directories(src contrib)

add_definitions(-DREVISION=250 -DQAPPLICATION_CLASS=QApplication)

# Tell CMake to create the helloworld executable
add_executable(superterm WIN32 ${superterm_SRCS})
# Use the Widgets module from Qt 5
target_link_libraries(superterm Qt5::Widgets)

qt5_use_modules(superterm Core SerialPort Network)
