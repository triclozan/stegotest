#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T00:13:45
#
#-------------------------------------------------

QT       += core gui
QMAKE_MAC_SDK = macosx10.9

TARGET = stego
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/mainwindow.cpp \
    src/main.cpp \
    src/dct.cpp \
    src/ctransform.cpp \
    src/ctesttransform.cpp \
    src/ctestnoise.cpp \
    src/ctestmedian.cpp \
    src/ctestkoh.cpp \
    src/ctestjpeg.cpp \
    src/ctestdistortion.cpp \
    src/ctest.cpp \
    src/cparamhelper.cpp \
    src/cimgcompare.cpp \
    src/cimagehistory.cpp \
    src/calgpri.cpp \
    src/calgorithm.cpp \
    src/calglsb.cpp \
    src/calglangelaar.cpp \
    src/calgkutter.cpp \
    src/calgkohy.cpp \
    src/calgkoh.cpp \
    src/calgfridrich.cpp \
    src/calgfridrich2.cpp \
    src/calgcombine.cpp \
    src/calgbengam.cpp \
    src/util.cpp \
    src/RNG/wnchyppr.cpp \
    src/RNG/userintf.cpp \
    src/RNG/stoc3.cpp \
    src/RNG/stoc2.cpp \
    src/RNG/stoc1.cpp \
    src/RNG/mersenne.cpp \
    src/RNG/fnchyppr.cpp \
    src/RNG/erfres.cpp \
    src/ECC/rsccode.cpp \
    src/calgconvolution.cpp \
    src/calgpolynom.cpp \
    src/calglegendre.cpp \
    src/calgwalsh.cpp \
    src/calgmidcoeff.cpp \
    src/cintegrator.cpp

HEADERS  += \
    src/mainwindow.h \
    src/dct.h \
    src/ctransform.h \
    src/ctesttransform.h \
    src/ctestnoise.h \
    src/ctestmedian.h \
    src/ctestkoh.h \
    src/ctestjpeg.h \
    src/ctestdistortion.h \
    src/ctest.h \
    src/cparamhelper.h \
    src/cimgcompare.h \
    src/cimagehistory.h \
    src/calgpri.h \
    src/calgorithm.h \
    src/calglsb.h \
    src/calglangelaar.h \
    src/calgkutter.h \
    src/calgkohy.h \
    src/calgkoh.h \
    src/calgfridrich.h \
    src/calgfridrich2.h \
    src/calgcombine.h \
    src/calgbengam.h \
    src/FFT/fft.h \
    src/FFT/wavelet2d.h \
    src/FFT/fftw3.h \
    src/RNG/stocc.h \
    src/RNG/randomc.h \
    src/ECC/rsccode.h \
    src/calgconvolution.h \
    src/calgpolynom.h \
    src/calglegendre.h \
    src/calgwalsh.h \
    src/calgmidcoeff.h \
    src/cintegrator.h

INCLUDEPATH += $$PWD/../builds/stego

FORMS    += src/ui/mainwindow.ui

RC_FILE += resources/res.rc

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    TODO.txt \
    Optimal_parameter.txt \
    Description.txt\
    data/*

LIBS += /usr/local/lib/libfftw3.a
LIBS += $$PWD/lib/libwavelet2s.a
win32: LIBS += -L$$PWD/lib/ -llibfftw3-3
win32: LIBS += -L$$PWD/lib/ -llibwavelet2d
win32: PRE_TARGETDEPS += $$PWD/lib/libfftw3-3.lib
win32: PRE_TARGETDEPS += $$PWD/lib/libwavelet2d.dll.a
