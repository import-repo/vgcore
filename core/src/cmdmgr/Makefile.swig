ROOTDIR  =../../..
PKGNAME  =touchvg
PROJNAME =cmdmgr

INCLUDES += -I$(ROOTDIR)/core/include \
            -I$(ROOTDIR)/core/include/geom \
            -I$(ROOTDIR)/core/include/graph \
            -I$(ROOTDIR)/core/include/canvas \
            -I$(ROOTDIR)/core/include/gshape \
            -I$(ROOTDIR)/core/include/shape \
            -I$(ROOTDIR)/core/include/storage \
            -I$(ROOTDIR)/core/include/cmd \
            -I$(ROOTDIR)/core/include/cmdobserver \
            -I$(ROOTDIR)/core/include/cmdbase \
            -I$(ROOTDIR)/core/include/cmdmgr

SOURCES   =$(wildcard *.cpp) \
           $(wildcard ../geom/*.cpp) \
           $(wildcard ../graph/*.cpp) \
           $(wildcard ../gshape/*.cpp) \
           $(wildcard ../shape/*.cpp) \
                      ../cmdbase/mgcmddraw.cpp

include $(ROOTDIR)/core/mk/config.mk
include $(ROOTDIR)/core/mk/Makefile.swig
