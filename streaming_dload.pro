#-------------------------------------------------
# QMake Build Script for: openpst/streaming-dload
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 5): error("At least Qt 5.0 is required")

QT += core gui widgets

CONFIG += C++11

TARGET = streaming-dload

TEMPLATE = app

equals(BASE_DIR, ""):		BASE_DIR 		= $$PWD
equals(LIBOPENPST_DIR, ""):	LIBOPENPST_DIR 	= $$PWD/lib/libopenpst
equals(GUICOMMON_DIR, ""):  GUICOMMON_DIR 	= $$PWD/lib/gui-common
equals(BUILD_DIR, ""):   	BUILD_DIR 		= $$PWD/build

INCLUDEPATH += \
	$$BASE_DIR/include \
	$$LIBOPENPST_DIR/include \
	$$LIBOPENPST_DIR/lib/serial/include \
	$$GUICOMMON_DIR/include

DEPENDPATH += $$BASE_DIR/

VPATH += $$BASE_DIR/

SOURCES += \
    $$BASE_DIR/src/streaming_dload_window.cpp \
    $$BASE_DIR/src/task/streaming_dload_read_task.cpp \
    $$BASE_DIR/src/task/streaming_dload_stream_write_task.cpp \
    $$BASE_DIR/src/main.cpp

HEADERS  += \
    $$BASE_DIR/src/streaming_dload_window.h \
    $$BASE_DIR/src/task/streaming_dload_read_task.h \
    $$BASE_DIR/src/task/streaming_dload_stream_write_task.h


FORMS  += $$BASE_DIR/resources/ui/streaming_dload_window.ui

RESOURCES = $$BASE_DIR/resources/streaming_dload.qrc

###
## Include gui-common .pro
###

include($$GUICOMMON_DIR/gui-common.pro)

###
## Make libopenpst and link against it
###

QMAKE_EXTRA_TARGETS += libopenpst
PRE_TARGETDEPS 		+= libopenpst
libopenpst.commands = cd $$LIBOPENPST_DIR && make qmake

LIBS += -L$$LIBOPENPST_DIR/build -lopenpst