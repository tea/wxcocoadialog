VERBOSE=0
DEBUG=0
E_ROOT=../../e

ifeq ($(VERBOSE),0)
	ECHO=echo
	SILENT=@
else
	ECHO=true
	SILENT=
endif

ifeq ($(DEBUG),0)
	CXXFLAGS = -O2
	LDFLAGS  =
	EXT_DIR  = $(E_ROOT)/external/out.release
	OUTDIR   = .objs.release
else
	ECORE    = ecored
	CXXFLAGS = -O0 -g
	LDFLAGS  = -g
	EXT_DIR  = $(E_ROOT)/external/out.debug
	OUTDIR   = .objs.debug
endif

EXE = CocoaDialog

WXINCLUDES = $(shell $(EXT_DIR)/bin/wx-config --cppflags)

GTKINCLUDES = $(shell pkg-config --cflags gtk+-2.0 cairo pango atk glib-2.0)

OURINCLUDES = -I$(EXT_DIR)/include

INCLUDES = $(WXINCLUDES) $(OURINCLUDES) $(GTKINCLUDES)

OURLIBPATHS = -L$(EXT_DIR)/lib
WXLIBS = $(shell $(EXT_DIR)/bin/wx-config --libs)
WEBKIT_LIBS = -lwxwebkit -lwebcore-wx -ljscore

LIBS = $(OURLIBPATHS) -lcurl $(WEBKIT_LIBS) $(WXLIBS)

# Exceptions which do not need to be compiled on linux
EXCEPT = IEHtmlWin.cpp \
         wxactivex.cpp

SRCS = $(filter-out $(EXCEPT), $(wildcard *.cpp))
OBJS = $(SRCS:%.cpp=$(OUTDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OUTDIR)/%.d)

EXTOBJS = $(shell $(EXT_DIR)/bin/wx-config --linkdeps) $(shell pkg-config --libs libxml-2.0 sqlite3 libxslt) $(shell icu-config --ldflags)

CXXFLAGS += -Wall -fno-strict-aliasing -DFEAT_BROWSER -MD

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	@$(ECHO) "[LD] $@"
	$(SILENT)$(CXX) $(LDFLAGS) -o $(EXE) $(OBJS) $(LIBS) $(EXTOBJS)

$(OUTDIR)/%.o: %.cpp
	@-mkdir -p $(OUTDIR)
	@$(ECHO) "[CC] $<"
	$(SILENT)$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXE) $(DEPS)

-include $(DEPS)

