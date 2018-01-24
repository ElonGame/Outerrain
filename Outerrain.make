# GNU Make project makefile autogenerated by Premake
ifndef config
  config=debug64
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),debug64)
  OBJDIR     = obj/x64/debug
  TARGETDIR  = bin
  TARGET     = $(TARGETDIR)/Outerrain
  DEFINES   += -DDEBUG
  INCLUDES  += -I. -IOuterrain/Include -IDependencies/include -I/usr/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -m64 -mtune=native -march=native -std=c++11 -W -Wall -Wsign-compare -Wno-unused-parameter -Wno-unused-variable -flto -g
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -m64 -L/usr/lib64 -flto -g
  LIBS      += -lGLEW -lSDL2 -lSDL2_image -lGL
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release64)
  OBJDIR     = obj/x64/release
  TARGETDIR  = bin
  TARGET     = $(TARGETDIR)/Outerrain
  DEFINES   += 
  INCLUDES  += -I. -IOuterrain/Include -IDependencies/include -I/usr/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O3 -m64 -mtune=native -march=native -std=c++11 -W -Wall -Wsign-compare -Wno-unused-parameter -Wno-unused-variable -flto -fopenmp
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -m64 -L/usr/lib64 -flto -fopenmp
  LIBS      += -lGLEW -lSDL2 -lSDL2_image -lGL
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug32)
  OBJDIR     = obj/x32/debug
  TARGETDIR  = bin
  TARGET     = $(TARGETDIR)/Outerrain
  DEFINES   += -DDEBUG
  INCLUDES  += -I. -IOuterrain/Include -IDependencies/include -I/usr/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -m32 -mtune=native -march=native -std=c++11 -W -Wall -Wsign-compare -Wno-unused-parameter -Wno-unused-variable -flto -g
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -m32 -L/usr/lib32 -flto -g
  LIBS      += -lGLEW -lSDL2 -lSDL2_image -lGL
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release32)
  OBJDIR     = obj/x32/release
  TARGETDIR  = bin
  TARGET     = $(TARGETDIR)/Outerrain
  DEFINES   += 
  INCLUDES  += -I. -IOuterrain/Include -IDependencies/include -I/usr/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O3 -m32 -mtune=native -march=native -std=c++11 -W -Wall -Wsign-compare -Wno-unused-parameter -Wno-unused-variable -flto -fopenmp
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -m32 -L/usr/lib32 -flto -fopenmp
  LIBS      += -lGLEW -lSDL2 -lSDL2_image -lGL
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/graph.o \
	$(OBJDIR)/shader.o \
	$(OBJDIR)/imgui.o \
	$(OBJDIR)/texture.o \
	$(OBJDIR)/cameraOrbiter.o \
	$(OBJDIR)/app.o \
	$(OBJDIR)/mytime.o \
	$(OBJDIR)/window.o \
	$(OBJDIR)/app-callbacks.o \
	$(OBJDIR)/imgui_draw.o \
	$(OBJDIR)/vegetationObject.o \
	$(OBJDIR)/imgui_opengl.o \
	$(OBJDIR)/transform.o \
	$(OBJDIR)/mesh.o \
	$(OBJDIR)/image.o \
	$(OBJDIR)/gameobject.o \
	$(OBJDIR)/main.o \
	$(OBJDIR)/terrain.o \
	$(OBJDIR)/color.o \
	$(OBJDIR)/noise.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking Outerrain
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Outerrain
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	-$(SILENT) cp $< $(OBJDIR)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
endif

$(OBJDIR)/graph.o: Outerrain/Source/graph.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/shader.o: Outerrain/Source/shader.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/imgui.o: Outerrain/Source/imgui.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/texture.o: Outerrain/Source/texture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/cameraOrbiter.o: Outerrain/Source/cameraOrbiter.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/app.o: Outerrain/Source/app.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/mytime.o: Outerrain/Source/mytime.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/window.o: Outerrain/Source/window.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/app-callbacks.o: Outerrain/Source/app-callbacks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/imgui_draw.o: Outerrain/Source/imgui_draw.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/vegetationObject.o: Outerrain/Source/vegetationObject.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/imgui_opengl.o: Outerrain/Source/imgui_opengl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/transform.o: Outerrain/Source/transform.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/mesh.o: Outerrain/Source/mesh.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/image.o: Outerrain/Source/image.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/gameobject.o: Outerrain/Source/gameobject.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/main.o: Outerrain/Source/main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/terrain.o: Outerrain/Source/terrain.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/color.o: Outerrain/Source/color.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/noise.o: Outerrain/Source/noise.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
