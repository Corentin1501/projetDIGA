cmake_minimum_required(VERSION 3.5)

project(Taquin_Impossible)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# si traductions : (paquet "qttools5-dev" pour LinguistTools)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
# sinon :
#find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Si traductions :
set(TS_FILES Taquin_french.ts)
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

add_executable(${PROJECT_NAME}
    "main.cpp"
    # ajouter vos fichier sources et en-tÃªte
    # ...
    mainwindow.cpp
    mainwindow.h

    gamewidget.cpp
    gamewidget.h

    dialognewgame.cpp
    dialognewgame.h
    dialognewgame.ui

# si ressources :
#    tp1.qrc

# si traductions :
    ${QM_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
