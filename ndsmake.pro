include(../RibiLibraries/ConsoleApplication.pri)
include(../RibiLibraries/Boost.pri)
include(../RibiLibraries/GeneralConsole.pri)

include(../RibiClasses/CppQrcFile/CppQrcFile.pri)
include(../RibiClasses/CppQtCreatorProFile/CppQtCreatorProFile.pri)

SOURCES += \
    main.cpp \
    ndsmake.cpp \
    ndsmakecreateholymakefile.cpp \
    ndsmakecreatemakefile.cpp

HEADERS += \
    ndsmake.h \
    ndsmakecreateholymakefile.h \
    ndsmakecreatemakefile.h
