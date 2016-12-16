default: all

# Build Options
-include makefile.inc

CXX_FLAGS := -std=c++03 -fPIC -m32 -g -fpermissive -D USE_ENCRYPTION
LINKING_FLAGS := -m32 -g

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

INCLUDE_DIRS := alogg almp3 allegro dumb gme lpng1212 loadpng lpng1212 jpgalleg-2.5 zlib123
INCLUDE_DIRS := -I./src -I./include $(addprefix -I./include/,$(INCLUDE_DIRS))

################################################################
# Libraries

# Statically linking allegro 4. Result of calling "allegro-config --static"
ALLEGRO_LIBS := -lalleg -lm -lpthread -lrt -lSM -lICE -lX11 -lXext -lXext -lXcursor -lXcursor -lXpm -lXxf86vm -lasound -ljack -lpthread -lXxf86dga -lSM -lICE -lX11 -lXext -ldl

# Sound libraries.
SOUND_LIBS = -lgme -lalogg -lalmp3 -laldmb -ldumb

# Image libraries.
IMAGE_LIBS = -ljpgal -lldpng -lpng -lz

################################################################
# Automatic Dependency Generation

# Don't freak out if a generated dependency file doesn't exist.
.d/%.d: ;

# Don't treat the dependency files as intermediate files.
.PRECIOUS: .d/%.d

# Load all the created dependency files.
-include $(patsubst %,.d/%.d,$(basename $(SOURCE_FILES)))

################################################################
# OS Specific Rules

ifdef COMPILE_FOR_LINUX
	ICON_DEPENDENCIES = src/%.c
	ICON_CREATION = $(CXX) $(OUTPUT_OPTION) -c $(CXX_FLAGS) $(INCLUDE_FLAGS) $^
	SOUND_LIBRARY := zcsound.so
	LINKING_FLAGS := $(LINKING_FLAGS) -Llibs/linux -L/usr/lib -Wl,-rpath,.
	SOUND_FLAGS := -Wl,-soname,zcsound.so
# Append -l to executable names.
	EXECUTABLE_SUFFIX := -l
	ZELDA_DEPENDENCIES := $(subst single_instance,single_instance_unix,$(ZELDA_DEPENDENCIES))
endif

EXECUTABLE_FILES := $(addsuffix $(EXECUTABLE_SUFFIX), $(EXECUTABLE_FILES))

################################################################
# Object File Compilation
obj/%.o : src/%.cpp .d/%.d
# Make sure needed folders are present.
	@mkdir -p obj/$(*D)
	@mkdir -p .d/obj/$(*D)
# Compile the object file. Automatically detect needed dependencies
# and generate a tempory makefile so we don't need to do it again.
	$(CXX) $(OUTPUT_OPTION) -c $< $(DEP_FLAGS) $(CXX_FLAGS) $(INCLUDE_DIRS) -MT $@ -MMD -MP -MF .d/obj/$(*D).Td
# Rename the .Td file to .d,
	@mv -f .d/obj/$(*D).Td .d/obj/$(*D).d

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

.PHONY: default all clean veryclean zc zelda zq zquest rv romview sound zcsound

all: $(EXECUTABLE_FILES)

clean:
	rm -rf obj
veryclean: clean
	rm -rf .d bin
zc zelda : $(word 1, $(EXECUTABLE_FILES))
zq zquest : $(word 2, $(EXECUTABLE_FILES))
rv romview :  $(word 3, $(EXECUTABLE_FILES))
sound zcsound : bin/$(SOUND_LIBRARY)
