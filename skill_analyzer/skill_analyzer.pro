QT       += core gui widgets
CONFIG   += c++17
TARGET    = skill_analyzer
TEMPLATE  = app

SOURCES  += \
    main.cpp \
    Graph.cpp \
    DataLoader.cpp \
    MainWindow.cpp

HEADERS  += \
    Node.h \
    Graph.h \
    DataLoader.h \
    MainWindow.h
