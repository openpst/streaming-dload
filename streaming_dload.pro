#-------------------------------------------------
# QMake Build Script for: openpst/streaming-dload
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 5): error("At least Qt 5.0 is required")

QT       += core gui widgets
CONFIG   += C++11 debug_and_release build_all
TARGET   = streaming-dload
TEMPLATE = app

equals(BASE_DIR, ""):  BASE_DIR = $$PWD

include($$BASE_DIR/lib/gui-common/gui-common.pri)
include($$BASE_DIR/lib/gui-common/gui-common.pro)
include($$BASE_DIR/lib/libopenpst/libopenpst.pri)

INCLUDEPATH +=  $$BASE_DIR/include \
                $$BASE_DIR/lib/libopenpst/include \
                $$BASE_DIR/lib/libopenpst/lib/serial/include \
                $$BASE_DIR/lib/gui-common/include 

SOURCES += \
    $$BASE_DIR/src/streaming_dload_window.cpp \
    $$BASE_DIR/src/task/streaming_dload_read_task.cpp \
    $$BASE_DIR/src/task/streaming_dload_stream_write_task.cpp \
    $$BASE_DIR/src/main.cpp

HEADERS  += \
    $$BASE_DIR/include/streaming_dload_window.h \
    $$BASE_DIR/include/task/streaming_dload_read_task.h \
    $$BASE_DIR/include/task/streaming_dload_stream_write_task.h

FORMS  += $$BASE_DIR/resources/ui/streaming_dload_window.ui

RESOURCES = $$BASE_DIR/resources/streaming_dload.qrc
