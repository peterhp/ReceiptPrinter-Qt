#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T15:18:24
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ReceiptPrinter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    basic/Goods.cpp \
    basic/ItemCart.cpp \
    basic/ItemOffer.cpp \
    basic/OfferBuy3Free1.cpp \
    util/Parser.cpp \
    module/ReceiptPrinter.cpp \
    test/Test.cpp \
    test/TestCartOffer.cpp \
    test/TestModule.cpp \
    test/TestParser.cpp

HEADERS += \
    basic/Goods.h \
    basic/ItemCart.h \
    basic/ItemOffer.h \
    basic/OfferBuy3Free1.h \
    util/Parser.h \
    module/ReceiptPrinter.h \
    test/Test.h

RESOURCES += \
    res.qrc
