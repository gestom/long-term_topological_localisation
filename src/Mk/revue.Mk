VPATH= ./:./Mk/:../Mk/
UNAME_CMD=uname

OPSYS=$(shell uname)

-include ../Mk/localh.mk
-include Mk/localh.mk

ifeq ($(OPSYS),FreeBSD)
SDL_CONFIG_CMD=sdl11-config
XML_CONFIG_CMD=xml2-config

ifeq ($(origin QT_HOME), undefined)
QT_HOME=/usr/X11R6
endif

QT_INCLUDE=-I$(QT_HOME)/include
QT_LIBRARY=-L$(QT_HOME)/lib
QT_MOC=$(QT_HOME)/bin/moc
QT_UIC=$(QT_HOME)/bin/uic
QT_LIBS=-lqt-mt
SDL_NET_LIB=-lSDL_net 
OPENGL_HOME=/usr/X11R6/include
OPENGL_LIBRARY=-L/usr/X11R6/lib
OPENGL_LIBS=-l/usr/lib/libGL.so -lGLU
endif

ifeq ($(OPSYS),Linux)
SDL_CONFIG_CMD=sdl-config
XML_CONFIG_CMD=xml2-config
ifeq ($(origin QT_HOME), undefined)
QT_HOME=/usr/X11R6/qt/3
endif
GSL_HOME=/usr/local/include/gsl
GSL_LIBS=-lgsl -lgslcblas
OPENGL_HOME=/usr/
OPENGL_LIBRARY=-L$(OPENGL_HOME)/lib
OPENGL_LIBS=-lGL -lGLU
SDL_NET_LIB=-lSDL_net 
RAW1394_LIBS=-lraw1394
DC1394_LIBS=-ldc1394
OPENGL_LIBRARY=-L$(OPENGL_HOME)/lib
OPENGL_LIBS=-lGL -lGLU
OPENCV_LIBRARY=-L/usr/local/lib
OPENCV_LIBS=-lhighgui -lcxcore
endif

ifeq ($(OPSYS),CYGWIN_NT-5.1)
SDL_CONFIG_CMD=sdl-config
XML_CONFIG_CMD=xml2-config
QT_HOME=/opt/qt/3.3
QT_INCLUDE=-I$(QT_HOME)/include
QT_LIBRARY=-L$(QT_HOME)/lib
QT_MOC=$(QT_HOME)/bin/moc
QT_UIC=$(QT_HOME)/bin/uic
KDE_HOME=/opt/kde3
KDE_INCLUDE=-I$(KDE_HOME)/include
KDE_LIB=-L$(KDE_HOME)/lib
QT_LIBS=-lqt
OPENGL_HOME=/usr/local
OPENGL_LIBRARY=-L$(OPENGL_HOME)/lib
OPENGL_LIBS=-lGL -lGLU
SDL_NET_LIB=-lSDL_net 
endif
#CXXINCLUDE+=`sdl11-config --cflags`
#CXXINCLUDE+=$(shell $(SDL_CONFIG_CMD) --cflags)
SDL_INCLUDE+=$(shell $(SDL_CONFIG_CMD) --cflags)
SDL_LIBS+=$(shell $(SDL_CONFIG_CMD) --libs) -lSDL_ttf
XML_INCLUDE+=$(shell $(XML_CONFIG_CMD) --cflags)
XML_LIBS+=$(shell $(XML_CONFIG_CMD) --libs)
#SDL_LIBS+=$(SDL_NET_LIB)

-include ../Mk/local.mk
-include Mk/local.mk

BCPP_CMD=bcpp
BCPP_FLAGS=-i 3 -s -bcl -f 1 -ylcnc
