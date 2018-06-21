cmake_minimum_required(VERSION 3.1.0)

project(superterm)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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
	src/ui/consoletabbar.cpp
	src/ui/consoleview.cpp
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
	src/ui/consoleview.ui
	src/ui/highlightsframe.cpp
	src/ui/renametabframe.cpp
	src/ui/connectionbar.ui
	src/ui/logfilepanel.ui
	src/ui/consoletabwidget.cpp
	src/ui/mainwindow.cpp
	src/ui/highlightsframe.ui
	src/misc/defaultsettings.cpp
	src/misc/globals.cpp
	src/main.cpp
	src/serial/portenumerator.cpp
	src/serial/portendpoint.cpp
	src/serial/serialportinfo.cpp
	src/serial/portapplication.cpp
	src/serial/portobserver.cpp
	superterm.qrc
	superterm.qss
)
include_directories(src contrib)

add_definitions(-DREVISION=55 -DQAPPLICATION_CLASS=QApplication)

# Tell CMake to create the helloworld executable
add_executable(superterm WIN32 ${superterm_SRCS})
# Use the Widgets module from Qt 5
target_link_libraries(superterm Qt5::Widgets)

qt5_use_modules(superterm Core SerialPort Network)
