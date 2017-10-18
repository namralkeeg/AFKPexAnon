TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

############################################
## Library Source Dependencies: Boost, keeg
## Boost: http://www.boost.org
## keeg: https://github.com/namralkeeg/keeg
############################################

INCLUDEPATH += $$(BOOST_ROOT) $$(KEEG_ROOT)/src src

SOURCES += \
    src/main.cpp \
    src/afk/fileformats/pex/pexheader.cpp \
    src/afk/fileformats/pex/pexbase.cpp \
    src/afk/fileformats/pex/pexskyrim.cpp \
    src/afk/fileformats/pex/pexskyrimse.cpp \
    src/afk/fileformats/pex/pexfallout4.cpp \
    src/afk/fileformats/pex/pexfactory.cpp \
    src/afk/afkpexanon.cpp

HEADERS += \
    src/version.hpp \
    src/afk/fileformats/pex/gameid.hpp \
    src/afk/fileformats/pex/pexheader.hpp \
    src/afk/fileformats/pex/pexbase.hpp \
    src/afk/fileformats/pex/pexskyrim.hpp \
    src/afk/fileformats/pex/pexskyrimse.hpp \
    src/afk/fileformats/pex/pexfallout4.hpp \
    src/afk/fileformats/pex/pexfactory.hpp \
    src/afk/afkpexanon.hpp

###############################
## Version Info
###############################

win32:VERSION = 1.1.0 # major.minor.patch
else:VERSION  = 1.1.0 # major.minor.patch

# Debug/Release options
CONFIG(debug, debug|release) {
        # Debug Options
    win32-g++ {
       LIBS += "-L$$(BOOST_LIBRARYDIR_MINGW)"
       LIBS += -lboost_program_options-mgw53-mt-d-1_65_1 -lboost_filesystem-mgw53-mt-d-1_65_1 -lboost_system-mgw53-mt-d-1_65_1
    }
} else {
        # Release Options
    win32-g++ {
       LIBS += "-L$$(BOOST_LIBRARYDIR_MINGW)"
       LIBS += -lboost_program_options-mgw53-mt-1_65_1 -lboost_filesystem-mgw53-mt-1_65_1 -lboost_system-mgw53-mt-1_65_1
    }
}

###############################
## COMPILER SCOPES
###############################

*msvc* {
        LIBS += "-L$$(BOOST_LIBRARYDIR)"

        # So VCProj Filters do not flatten headers/source
        CONFIG -= flat

        # COMPILER FLAGS
        #  Optimization flags
        QMAKE_CXXFLAGS_RELEASE -= /O2
        QMAKE_CXXFLAGS_RELEASE *= /O2 /Ot /Ox /GL

        #  Multithreaded compiling for Visual Studio
        QMAKE_CXXFLAGS += -MP

        # Linker flags
        QMAKE_LFLAGS_RELEASE += /LTCG
}

*-g++ {
        # COMPILER FLAGS

        #  Optimization flags
        QMAKE_CXXFLAGS_DEBUG -= -O0 -g
        QMAKE_CXXFLAGS_DEBUG *= -Og -g3
        QMAKE_CXXFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE *= -O3 -mfpmath=sse

        #  Extension flags
        QMAKE_CXXFLAGS_RELEASE += -msse2 -msse

        # Linker flags
        QMAKE_LFLAGS_RELEASE += -static
}
