CONFIG += testcase
TARGET = tst_moduleqt47

QT += testlib
contains(QT_CONFIG,quick1): QT += quick1 gui
macx:CONFIG -= app_bundle

SOURCES += tst_moduleqt47.cpp

symbian: {
    importFiles.files = data
    importFiles.path = .
    DEPLOYMENT += importFiles
} else {
    DEFINES += SRCDIR=\\\"$$PWD\\\"
}

CONFIG += parallel_test
