################################################################
# Configuration

-include config/os
-include config/arch
-include config/allegro_version
-include config/debug_symbols

# Maybe add autodetect in here later.
ifndef OS
  OS := Linux
endif
ifndef ARCH
  ARCH := i686
endif
ifndef ALLEGRO_VERSION
  ALLEGRO_VERSION := 4.2.modded
endif
ifndef DEBUG_SYMBOLS
  DEBUG_SYMBOLS := ON
endif

################################################################

CXX_FLAGS := -std=c++03 -fPIC -fpermissive -D USE_ENCRYPTION
LINKING_FLAGS := -Lbin

ifeq ($(DEBUG_SYMBOLS),ON)
	CXX_FLAGS := $(CXX_FLAGS) -g
	LINKING_FLAGS := $(LINKING_FLAGS) -g
else
	CXX_FLAGS := $(CXX_FLAGS) -s
	LINKING_FLAGS := $(LINKING_FLAGS) -s
endif

################################################################
# File Lists

SOURCE_FILES := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)

EXECUTABLE_FILES := zelda zquest romview
EXECUTABLE_FILES := $(addprefix bin/,$(EXECUTABLE_FILES))

ZELDA_DEPENDENCIES := aglogo colors debug decorations defdata editbox EditboxModel EditboxView ending ffscript gamedata gui guys init items jwin jwinfsel link load_gif maps matrix md5 midi pal particles qst save_gif script_drawing single_instance sprite subscr tab_ctl tiles title weapons win32 zc_custom zc_init zc_items zc_sprite zc_subscr zc_sys zelda zscriptversion zsys zc_icon
ZELDA_DEPENDENCIES := $(addprefix obj/,$(ZELDA_DEPENDENCIES))
ZELDA_DEPENDENCIES := $(addsuffix .o,$(ZELDA_DEPENDENCIES))

ZQUEST_DEPENDENCIES := zquest colors defdata editbox EditboxModel EditboxView gamedata gui init items jwin jwinfsel load_gif md5 midi particles qst questReport save_gif sprite subscr tab_ctl tiles win32 zc_custom zq_class zq_cset zq_custom zq_doors zq_files zq_items zq_init zq_misc zq_rules zq_sprite zq_strings zq_subscr zq_tiles zqscale zsys ffasm parser/AST parser/BuildVisitors parser/ByteCode parser/DataStructs parser/GlobalSymbols parser/lex.yy parser/ParseError parser/ScriptParser parser/SymbolVisitors parser/TypeChecker parser/UtilVisitors parser/y.tab zq_icon
ZQUEST_DEPENDENCIES := $(addprefix obj/,$(ZQUEST_DEPENDENCIES))
ZQUEST_DEPENDENCIES := $(addsuffix .o,$(ZQUEST_DEPENDENCIES))

ROMVIEW_DEPENDENCIES := editbox EditboxModel EditboxView gui jwin jwinfsel load_gif romview save_gif tab_ctl zqscale zsys rv_icon
ROMVIEW_DEPENDENCIES := $(addprefix obj/,$(ROMVIEW_DEPENDENCIES))
ROMVIEW_DEPENDENCIES := $(addsuffix .o,$(ROMVIEW_DEPENDENCIES))

################################################################
# Headers

INCLUDE_DIRS := alogg almp3 dumb gme lpng1212 loadpng lpng1212 jpgalleg-2.5 zlib123
INCLUDE_DIRS := -I./src -I./include $(addprefix -I./include/,$(INCLUDE_DIRS))

ifeq ($(ALLEGRO_VERSION),4.2.modded)
  INCLUDE_DIRS := -Iinclude/allegro-4.2 $(INCLUDE_DIRS)
else ifeq ($(ALLEGRO_VERSION),4.4)
  INCLUDE_DIRS := -Iinclude/allegro-4.4 $(INCLUDE_DIRS)
endif

################################################################
# Libraries

# Statically linking allegro 4. Result of calling "allegro-config --static"
ALLEGRO_LIBS := -lalleg -lm -lpthread -lrt -lSM -lICE -lX11 -lXext -lXext -lXcursor -lXcursor -lXpm -lXxf86vm -lasound -ljack -lpthread -lXxf86dga -lSM -lICE -lX11 -lXext -ldl

# Sound libraries.
SOUND_LIBS := -lgme -lalmp3 -laldmb -ldumb -lalogg

ifeq ($(ALLEGRO_VERSION),4.4)
  SOUND_LIBS := $(SOUND_LIBS) -lalleg-alsadigi -lalleg-alsamidi -lalleg-jack -lalleg-dga2
endif

# Image libraries.
IMAGE_LIBS := -lldpng -lpng -lz

################################################################
# OS Specific Rules

ifeq ($(OS),Linux)
  ICON_DEPENDENCIES = src/%.c
  ICON_CREATION = $(CXX) $(OUTPUT_OPTION) -c $(CXX_FLAGS) $(INCLUDE_FLAGS) $^
  SOUND_LIBRARY := zc_sound.so
  SOUND_FLAGS :=
  # Append -l to executable names.
  EXECUTABLE_SUFFIX := -l
  ZELDA_DEPENDENCIES := $(subst single_instance,single_instance_unix,$(ZELDA_DEPENDENCIES))
  ifeq ($(ALLEGRO_VERSION),4.2.modded)
    LINKING_FLAGS := $(LINKING_FLAGS) -Llibs/linux/4.2
    IMAGE_LIBS := -ljpgal $(IMAGE_LIBS)
  else ifeq ($(ALLEGRO_VERSION),4.4)
    LINKING_FLAGS := $(LINKING_FLAGS) -Llibs/linux/4.4
    IMAGE_LIBS := -ljpgalleg $(IMAGE_LIBS)
    SHARED_LIBRARIES := $(wildcard libs/linux/4.4/*.so)
    ifeq ($(DEBUG_SYMBOLS),ON)
      ALLEGRO_LIBS := $(subst -lalleg,-lalleg-debug,$(ALLEGRO_LIBS))
    endif
  endif
  LINKING_FLAGS := $(LINKING_FLAGS) -Llibs/linux -L/usr/lib32 -L/usr/lib -Wl,-rpath,.
endif

EXECUTABLE_FILES := $(addsuffix $(EXECUTABLE_SUFFIX), $(EXECUTABLE_FILES))

################################################################
# Allegro Compat

ifneq ($(ALLEGRO_VERSION),4.2.modded)
  $(info Using Compat)
  CXX_FLAGS := $(CXX_FLAGS) -D USE_ALLEG_COMPAT
	ZELDA_DEPENDENCIES := $(ZELDA_DEPENDENCIES) obj/alleg_compat.o
	ZQUEST_DEPENDENCIES := $(ZQUEST_DEPENDENCIES) obj/alleg_compat.o
	ROMVIEW_DEPENDENCIES := $(ROMVIEW_DEPENDENCIES) obj/alleg_compat.o
endif

################################################################
################################################################
################################################################

default: all

################################################################
# Automatic Dependency Generation

# Don't freak out if a generated dependency file doesn't exist.
.d/%.d: ;

# Don't treat the dependency files as intermediate files.
.PRECIOUS: .d/%.d

# Load all the created dependency files.
-include $(patsubst %,.d/%.d,$(basename $(SOURCE_FILES)))

################################################################
# Configuration Interaction

lin linux :
	@mkdir -p config
	echo OS := Linux > config/os

32 32bit i686 :
	@mkdir -p config
	echo ARCH := i686 > config/arch

allegro-4.2.modded modded :
	@mkdir -p config
	echo ALLEGRO_VERSION := 4.2.modded > config/allegro_version

allegro-4.2 4.2 :
	@mkdir -p config
	echo ALLEGRO_VERSION := 4.2 > config/allegro_version

allegro-4.4 4.4 :
	@mkdir -p config
	echo ALLEGRO_VERSION := 4.4 > config/allegro_version

debug-on :
	@mkdir -p config
	echo DEBUG_SYMBOLS := ON > config/debug_symbols

debug-off :
	@mkdir -p config
	echo DEBUG_SYMBOLS := OFF > config/debug_symbols

show-config :
	@echo OS = $(OS)
	@echo ARCH = $(ARCH)
	@echo ALLEGRO_VERSION = $(ALLEGRO_VERSION)
	@echo DEBUG_SYMBOLS = $(DEBUG_SYMBOLS)

################################################################
# Object File Compilation
obj/%.o : src/%.cpp .d/%.d
# Make sure needed folders are present.
	@mkdir -p obj/$(*D)
	@mkdir -p .d/obj/$(*D)
# Compile the object file. Automatically detect needed dependencies
# and generate a tempory makefile so we don't need to do it again.
	$(CXX) $(OUTPUT_OPTION) -c $< $(DEP_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) -MT $@ -MMD -MP -MF .d/obj/$*.Td
# Rename the .Td file to .d,
	@mv -f .d/obj/$*.Td .d/obj/$*.d

################################################################
# Icons
obj/zc_icon.o obj/zq_icon.o obj/rv_icon.o : obj/%.o : $(ICON_DEPENDENCIES)
# Make sure needed folders are present.
	@mkdir -p obj
	$(ICON_CREATION)

################################################################
# Sound Library
bin/$(SOUND_LIBRARY) : obj/zcmusic.o obj/zcmusicd.o
# Make sure needed folders are present.
	@mkdir -p bin
# Compile the library.
	$(CXX) $(OUTPUT_OPTION) -shared obj/zcmusic.o obj/zcmusicd.o -Wl,-soname,$(SOUND_LIBRARY) $(LINKING_FLAGS) $(SOUND_FLAGS) $(ALLEGRO_LIBS) $(SOUND_LIBS)

################################################################
# Shared Libraries

$(addprefix bin/,$(SHARED_LIBRARIES)) : bin/%.so :
	cp $*.so bin/$(@F)

################################################################
# Zelda
bin/zelda$(EXECUTABLE_SUFFIX) : bin/$(SOUND_LIBRARY) $(ZELDA_DEPENDENCIES)
  # Make sure needed folders are present.
	@mkdir -p bin/
	@mkdir -p .d/bin/
# Compile the program.  Automatically detect needed dependencies and
# generate a tempory makefile so we don't need to do it again.
	$(CXX) $(OUTPUT_OPTION) bin/$(SOUND_LIBRARY) $(ZELDA_DEPENDENCIES) $(LINKING_FLAGS) $(STDCXX_LIB) $(ALLEGRO_LIBS) $(IMAGE_LIBS)

################################################################
# ZQuest
bin/zquest$(EXECUTABLE_SUFFIX) : bin/$(SOUND_LIBRARY) $(ZQUEST_DEPENDENCIES)
# Make sure needed folders are present.
	@mkdir -p bin/
	@mkdir -p .d/bin/
# Compile the program.  Automatically detect needed dependencies and
# generate a tempory makefile so we don't need to do it again.
	$(CXX) $(OUTPUT_OPTION) bin/$(SOUND_LIBRARY) $(ZQUEST_DEPENDENCIES) $(LINKING_FLAGS) $(STDCXX_LIB) $(ALLEGRO_LIBS) $(IMAGE_LIBS)

################################################################
# Romview
bin/romview$(EXECUTABLE_SUFFIX) : bin/$(SOUND_LIBRARY) $(ROMVIEW_DEPENDENCIES)
# Make sure needed folders are present.
	@mkdir -p bin/
	@mkdir -p .d/bin/
# Compile the program.  Automatically detect needed dependencies and
# generate a tempory makefile so we don't need to do it again.
	$(CXX) $(OUTPUT_OPTION) $(ROMVIEW_DEPENDENCIES) $(LINKING_FLAGS) $(STDCXX_LIB) $(ALLEGRO_LIBS) $(IMAGE_LIBS)

################################################################
# Various Other Rules

.PHONY : default all clean veryclean zc zelda zq zquest rv romview sound zcsound lin linux 32 32bit i686 allegro-4.2.modded modded allegro-4.2 4.2 allegro-4.4 4.4 debug-on debug-off show-config

all : $(EXECUTABLE_FILES) $(addprefix bin/,$(SHARED_LIBRARIES))

clean :
	-rm -rf obj
veryclean : clean
	-rm -rf .d
	-rm $(EXECUTABLE_FILES)
zc zelda : $(word 1, $(EXECUTABLE_FILES))
zq zquest : $(word 2, $(EXECUTABLE_FILES))
rv romview :  $(word 3, $(EXECUTABLE_FILES))
sound zcsound :	bin/$(SOUND_LIBRARY)
