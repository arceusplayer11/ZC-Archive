-include makefile.inc

# Libraries needed for statically linking allegro 4. Result of calling "allegro-config --static"
ALLEGRO_LIBS := -lalleg -lm -lpthread -lrt -lSM -lICE -lX11 -lXext -lXext -lXcursor -lXcursor -lXpm -lXxf86vm -lasound -ljack -lpthread -lXxf86dga -lSM -lICE -lX11 -lXext -ldl
# Libraries for zcsound.so
SOUND_LIBS = -lgme -lalogg -lalmp3 -laldmb -ldumb
# Image libraries.
IMAGE_LIBS = -ljpgal -lldpng -lpng -lz

CFLAGS := -std=c99 -fPIC -m32 -g
CXXFLAGS := -std=c++03 -fPIC -m32 -g -fpermissive -D USE_ENCRYPTION
LDFLAGS := -m32 -g

# Various directories
SRCDIR := src
OBJDIR := obj
BINDIR := bin
DEPDIR := .d

# Create parent directories.
$(shell mkdir -p $(DEPDIR) > /dev/null)
$(shell mkdir -p $(DEPDIR)/item > /dev/null)
$(shell mkdir -p $(DEPDIR)/parser > /dev/null)
$(shell mkdir -p $(DEPDIR)/sequence > /dev/null)
$(shell mkdir -p $(OBJDIR) > /dev/null)
$(shell mkdir -p $(OBJDIR)/parser > /dev/null)
$(shell mkdir -p $(OBJDIR)/sequence > /dev/null)
$(shell mkdir -p $(BINDIR) > /dev/null)

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
HEADERS := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := alogg almp3 allegro dumb gme lpng1212 loadpng lpng1212 jpgalleg-2.5 zlib123
INCLUDES := -I./src -I./include $(addprefix -I./include/,$(INCLUDES))

.PHONY: default veryclean clean all msg gp2x test done zc zq

default: all

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES)))

ifdef COMPILE_FOR_LINUX
  PLATEXT = -l
  ZCSOUND_ALLEG_LIB = -L./include/linux -L/usr/lib32 -lalleg -lm -lpthread -lrt -lSM -lICE -lX11 -lXext -lXext -lXcursor -lXcursor -lXpm -lXxf86vm -lasound -ljack -lpthread -lXxf86dga -lSM -lICE -lX11 -lXext -ldl
  ZC_ICON = $(OBJDIR)/zc_icon.o
  ZC_ICON_DEPS = $(SRCDIR)/zc_icon.c
  ZC_ICON_CMD = $(CXX) $(CXXFLAGS) $(INCLUDES) -c $(SRCDIR)/zc_icon.c -o $(OBJDIR)/zc_icon.o
  ZQ_ICON = $(OBJDIR)/zq_icon.o
  ZQ_ICON_DEPS = $(SRCDIR)/zq_icon.c
  ZQ_ICON_CMD = $(CC) $(OPTS) $(CFLAGS) $(INCLUDES) -m32 -c $(SRCDIR)/zq_icon.c -o $(OBJDIR)/zq_icon.o $(SFLAG)
  RV_ICON = $(OBJDIR)/rv_icon.o
  RV_ICON_DEPS = $(SRCDIR)/rv_icon.c
  RV_ICON_CMD = $(CC) $(OPTS) $(CFLAGS) $(INCLUDES) -m32 -c $(SRCDIR)/rv_icon.c -o $(OBJDIR)/rv_icon.o $(SFLAG)
  ZC_PLATFORM = Linux

	LDFLAGS := -Wl,--export-dynamic $(LDFLAGS) -L./$(BINDIR) -L./libs/linux -L/usr/lib32 -Wl,-rpath,.
	LDLIBS := $(ALLEGRO_LIBS) -lzcsound $(STDCXX_LIB)

	ZCSOUND_DLL := $(BINDIR)/libzcsound.so

	SINGLE_INSTANCE_O = single_instance_unix.o
endif

ZELDA_PREFIX = zelda
ZQUEST_PREFIX = zquest
ROMVIEW_PREFIX = romview

ZELDA_EXE = $(BINDIR)/$(ZELDA_PREFIX)$(PLATEXT)$(EXEEXT)
ZQUEST_EXE = $(BINDIR)/$(ZQUEST_PREFIX)$(PLATEXT)$(EXEEXT)
ROMVIEW_EXE = $(BINDIR)/$(ROMVIEW_PREFIX)$(PLATEXT)$(EXEEXT)

ZCSOUND_OBJECTS = $(OBJDIR)/zcmusic.o $(OBJDIR)/zcmusicd.o

SEQUENCES := gameOver.o ganonIntro.o getBigTriforce.o getTriforce.o potion.o sequence.o whistle.o
ZELDA_OBJECTS := aglogo.o colors.o debug.o decorations.o defdata.o editbox.o EditboxModel.o EditboxView.o encryptionEnabled.o ending.o enemyAttack.o ffc.o ffscript.o fontClass.o gamedata.o gui.o guys.o init.o inventory.o items.o jwin.o jwinfsel.o link.o load_gif.o maps.o matrix.o md5.o message.o messageManager.o messageRenderer.o messageStream.o midi.o pal.o particles.o qst.o refInfo.o room.o save_gif.o screenFreezeState.o screenWipe.o script_drawing.o sfxAllegro.o sfxClass.o sfxManager.o $(SINGLE_INSTANCE_O) sound.o sprite.o subscr.o tab_ctl.o tiles.o title.o weapons.o win32.o zc_custom.o zc_init.o zc_items.o zc_sprite.o zc_subscr.o zc_sys.o zelda.o zscriptversion.o zsys.o
ZELDA_OBJECTS := $(addprefix $(OBJDIR)/,$(ZELDA_OBJECTS)) $(addprefix $(OBJDIR)/sequence/,$(SEQUENCES)) $(ZC_ICON)

ZQUEST_OBJECTS = zquest.o colors.o defdata.o editbox.o EditboxModel.o EditboxView.o gamedata.o gui.o init.o items.o jwin.o jwinfsel.o load_gif.o md5.o midi.o particles.o qst.o questReport.o save_gif.o sprite.o subscr.o tab_ctl.o tiles.o win32.o zc_custom.o zq_class.o zq_cset.o zq_custom.o zq_doors.o zq_files.o zq_items.o zq_init.o zq_misc.o zq_sprite.o zq_strings.o zq_subscr.o zq_tiles.o zqscale.o zsys.o ffasm.o parser/AST.o parser/BuildVisitors.o parser/ByteCode.o parser/DataStructs.o parser/GlobalSymbols.o parser/lex.yy.o parser/ParseError.o parser/ScriptParser.o parser/SymbolVisitors.o parser/TypeChecker.o parser/UtilVisitors.o parser/y.tab.o
ZQUEST_OBJECTS := $(addprefix $(OBJDIR)/,$(ZQUEST_OBJECTS)) $(ZQ_ICON)

ROMVIEW_OBJECTS = editbox.o EditboxModel.o EditboxView.o gui.o jwin.o jwinfsel.o load_gif.o romview.o save_gif.o tab_ctl.o zqscale.o zsys.o
ROMVIEW_OBJECTS := $(addprefix $(OBJDIR)/,$(ROMVIEW_OBJECTS)) $(RV_ICON)

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
COMPILE.cpp = $(CXX) -o $@ $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

%.o : %.cpp
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cpp) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

msg:
	@echo Compiling Zelda Classic for $(ZC_PLATFORM)...
done:
	@echo Done!
clean:
	rm -rf $(OBJDIR)
veryclean: clean
	rm -rf $(DEPDIR) $(ZELDA_EXE) $(ZQUEST_EXE) $(ROMVIEW_EXE)

all: msg $(ZELDA_EXE) $(ZQUEST_EXE) $(ROMVIEW_EXE) done

sound: $(ZCSOUND_DLL)
$(ZCSOUND_DLL): $(ZCSOUND_OBJECTS)
	$(CXX) -shared -o $(ZCSOUND_DLL) $(ZCSOUND_OBJECTS) $(LDFLAGS) $(ALLEGRO_LIBS) $(SOUND_LIBS)

zc: $(ZELDA_EXE)
$(ZELDA_EXE): $(ZELDA_OBJECTS)
	$(CXX) -o $(ZELDA_EXE) $(ZELDA_OBJECTS) $(LDFLAGS) $(LDLIBS) $(IMAGE_LIBS) #-Wl,--verbose

$(ZQUEST_EXE): $(ZQUEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(ZQUEST_EXE) $(ZQUEST_OBJECTS) $(LDFLAGS) $(LIBDIR) $(IMAGE_LIBS) $(ZCSOUND_LIB) $(ALLEG_LIB) $(STDCXX_LIB) $(ZQ_ICON) $(SFLAG) $(WINFLAG)

$(ROMVIEW_EXE): $(ROMVIEW_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(ROMVIEW_EXE) $(ROMVIEW_OBJECTS) $(LDFLAGS) $(LIBDIR) $(IMAGE_LIBS) $(ALLEG_LIB) $(STDCXX_LIB) $(RV_ICON) $(SFLAG) $(WINFLAG)

$(ZC_ICON) : $(ZC_ICON_DEPS)
	$(ZC_ICON_CMD)

$(ZQ_ICON) : $(ZQ_ICON_DEPS)
	$(ZQ_ICON_CMD)

$(RV_ICON) : $(RV_ICON_DEPS)
	$(RV_ICON_CMD)
