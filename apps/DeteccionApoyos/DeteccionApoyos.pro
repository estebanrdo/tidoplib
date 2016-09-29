# ensure one "debug_and_release" in CONFIG, for clarity...
debug_and_release {
    CONFIG -= debug_and_release
    CONFIG += debug_and_release
}

# ensure one "debug" or "release" in CONFIG so they can be used as
# conditionals instead of writing "CONFIG(debug, debug|release)"...
CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug

}
CONFIG(release, debug|release) {
        CONFIG -= debug release
        CONFIG += release
}

DEFINES += __QT_PRJ__

QT += core
QT += xml
QT -= gui

CONFIG += c++11

TARGET = DeteccionApoyos
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../src/ \
               ../../../Libs/opencv/include/

SOURCES += \
    ../src/VideoStream.cpp \
    ../src/LineDetector.cpp \
    ../src/LSWMS.cpp \
    ../src/utils.cpp \
    DeteccionApoyos.cpp \
    ../src/Logger.cpp

HEADERS  += ../src/VideoStream.h \
    ../src/LineDetector.h \
    ../src/LSWMS.h \
    ../src/utils.h \
    ../src/Logger.h

debug{

                LIBS += -L$$PWD/../../../Libs/opencv/x64/vc12/bin
                LIBS += -L$$PWD/../../../Libs/opencv/x64/vc12/lib
                LIBS += -lopencv_core310d -lopencv_shape310d -lopencv_stitching310d -lopencv_superres310d -lopencv_videostab310d -lopencv_objdetect310d -lopencv_photo310d -lopencv_calib3d310d -lopencv_features2d310d -lopencv_flann310d -lopencv_highgui310d -lopencv_ml310d -lopencv_videoio310d -lopencv_imgcodecs310d -lopencv_video310d -lopencv_imgproc310d -lopencv_optflow310d

}else{
                LIBS += -L$$PWD/../../../Libs/opencv/x64/vc12/bin
                LIBS += -L$$PWD/../../../Libs/opencv/x64/vc12/lib
                LIBS += -lopencv_core310 -lopencv_shape310 -lopencv_stitching310 -lopencv_superres310 -lopencv_videostab310 -lopencv_objdetect310 -lopencv_photo310 -lopencv_calib3d310 -lopencv_features2d310 -lopencv_flann310 -lopencv_highgui310 -lopencv_ml310 -lopencv_videoio310 -lopencv_imgcodecs310 -lopencv_video310 -lopencv_imgproc310 -lopencv_optflow310 -lopencv_optflow310d
}
