
# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.19)
# Define the project and its version
project(treeViewTest VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH ~/Qt/6.4.2/gcc_64)

set(QT_QMAKE_EXECUTABLE ~/Qt/6.4.2/gcc_64/bin/qmake)
set(QT_QMAKE_EXECUTABLE ~/Qt/6.4.2/gcc_64/bin/qmake)
set(Qt6CoreTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6CoreTools)
set(Qt6CoreTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6CoreTools)
set(Qt6Core_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Core)
set(Qt6Core_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Core)
set(Qt6DBusTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6DBusTools)
set(Qt6DBusTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6DBusTools)
set(Qt6DBus_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6DBus)
set(Qt6DBus_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6DBus)
set(Qt6GuiTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6GuiTools)
set(Qt6GuiTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6GuiTools)
set(Qt6Gui_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Gui)
set(Qt6Gui_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Gui)
set(Qt6Multimedia_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Multimedia)
set(Qt6Multimedia_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Multimedia)
set(Qt6Network_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Network)
set(Qt6Network_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Network)
set(Qt6OpenGL_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6OpenGL)
set(Qt6OpenGL_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6OpenGL)
set(Qt6QmlIntegration_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6QmlIntegration)
set(Qt6QmlIntegration_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6QmlIntegration)
set(Qt6QmlModels_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6QmlModels)
set(Qt6QmlModels_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6QmlModels)
set(Qt6QmlTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6QmlTools)
set(Qt6QmlTools_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6QmlTools)
set(Qt6Qml_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Qml)
set(Qt6Qml_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Qml)
set(Qt6Quick_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Quick)
set(Qt6Quick_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6Quick)
set(Qt6_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6)
set(Qt6_DIR  ~/Qt/6.4.2/gcc_64/lib/cmake/Qt6)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Quick Qml Pdf QuickWidgets Multimedia MultimediaWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Define your project's source files
set(SOURCES
        main.cpp
        TreeModel.cpp
        TreeNode.cpp
        TreeView.cpp
        Dialog.cpp
        EditableTreeModel.cpp
        EditableTreeItem.cpp
)

# Define your project's header files
set(HEADERS
        TreeModel.cpp
        TreeNode.hpp
        TreeView.hpp
        Dialog.hpp
        EditableTreeModel.hpp
        EditableTreeItem.hpp
)

# Define your project's forms
set(FORMS
)

# Define your project's resources
#set(RESOURCES
#        displax.qrc
#)

#add_compile_options(-Wall -Wextra -Wno-sign-compare)

qt_add_executable(treeViewTest ${SOURCES})

# Include Qt include directories in the include path
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/$$PWD
)

# Link against the required Qt libraries
target_link_libraries(treeViewTest PRIVATE
        Qt6::Gui
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Quick
        Qt6::QuickWidgets
        Qt6::Qml
        Qt6::Pdf
)

# Set up installation rules if needed
install(TARGETS treeViewTest
        RUNTIME DESTINATION /opt/$${TARGET}/bin
)

# Add any additional configurations specific to your project
# ...

set(resource_files
)

qt_add_resources(treeViewTest "qml"
        PREFIX
        "/qml"
        FILES
        ${resource_files}
)
