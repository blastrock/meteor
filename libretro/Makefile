DEBUG = 0
FRONTEND_SUPPORTS_RGB565=1

ifneq ($(EMSCRIPTEN),)
   platform = emscripten
endif

ifeq ($(platform),)
platform = unix
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   platform = osx
   arch = intel
ifeq ($(shell uname -p),powerpc)
   arch = ppc
endif
else ifneq ($(findstring win,$(shell uname -a)),)
   platform = win
endif
endif

LIBRETRO_DIR := .
CORE_DIR := ..

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
EXE_EXT = .exe
   system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   system_platform = osx
   arch = intel
ifeq ($(shell uname -p),powerpc)
   arch = ppc
endif
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   system_platform = win
endif

TARGET_NAME := meteor
GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	CXXFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

ifeq ($(platform), unix)
   TARGET := $(TARGET_NAME)_libretro.so
   fpic := -fPIC
   SHARED := -shared -Wl,-version-script=$(LIBRETRO_DIR)/link.T -Wl,-no-undefined
   ENDIANNESS_DEFINES = -DLSB_FIRST
else ifeq ($(platform), osx)
   TARGET := $(TARGET_NAME)_libretro.dylib
   fpic := -fPIC
ifeq ($(arch),ppc)
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES := -D__POWERPC__ -D__ppc__
else
   ENDIANNESS_DEFINES = -DLSB_FIRST
	NO_MEMMEM = 1
endif
   OSXVER = `sw_vers -productVersion | cut -d. -f 2`
   OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
   #fpic += -mmacosx-version-min=10.1
   SHARED := -dynamiclib

# iOS
else ifneq (,$(findstring ios,$(platform)))
   TARGET := $(TARGET_NAME)_libretro_ios.dylib
   fpic := -fPIC
   SHARED := -dynamiclib

ifeq ($(IOSSDK),)
   IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
endif
ifeq ($(platform),ios-arm64)
   CC = cc -arch arm64 -isysroot $(IOSSDK)
   CXX = c++ -arch arm64 -isysroot $(IOSSDK)
else
   CC = cc -arch armv7 -isysroot $(IOSSDK)
   CXX = c++ -arch armv7 -isysroot $(IOSSDK)
endif
   CFLAGS += -DIOS
ifeq ($(platform),$(filter $(platform),ios9 ios-arm64))
   CC += -miphoneos-version-min=8.0
   CXX += -miphoneos-version-min=8.0
   CFLAGS += -miphoneos-version-min=8.0
else
   CC += -miphoneos-version-min=5.0
   CXX += -miphoneos-version-min=5.0
   CFLAGS += -miphoneos-version-min=5.0
endif
   ENDIANNESS_DEFINES = -DLSB_FIRST

else ifeq ($(platform), tvos-arm64)
   TARGET := $(TARGET_NAME)_libretro_tvos.dylib
   fpic := -fPIC
   SHARED := -dynamiclib
   CFLAGS := -DIOS
   ENDIANNESS_DEFINES = -DLSB_FIRST

ifeq ($(IOSSDK),)
   IOSSDK := $(shell xcodebuild -version -sdk appletvos Path)
endif

else ifeq ($(platform), theos_ios)
DEPLOYMENT_IOSVERSION = 5.0
TARGET = iphone:latest:$(DEPLOYMENT_IOSVERSION)
ARCHS = armv7 armv7s
TARGET_IPHONEOS_DEPLOYMENT_VERSION=$(DEPLOYMENT_IOSVERSION)
THEOS_BUILD_DIR := objs
include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = $(TARGET_NAME)_libretro_ios
   ENDIANNESS_DEFINES = -DLSB_FIRST

else ifeq ($(platform), qnx)
   TARGET := $(TARGET_NAME)_libretro_qnx.so
   fpic := -fPIC
   SHARED := -lcpp -lm -shared -Wl,-version-script=$(LIBRETRO_DIR)/link.T -Wl,-no-undefined
	CC = qcc -Vgcc_ntoarmv7le
	CXX = QCC -Vgcc_ntoarmv7le_cpp
	AR = QCC -Vgcc_ntoarmv7le
   ENDIANNESS_DEFINES = -DLSB_FIRST
	PLATFORM_DEFINES := -D__BLACKBERRY_QNX__ -marm -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=softfp
else ifeq ($(platform), ps3)
   TARGET := $(TARGET_NAME)_libretro_ps3.a
   CC = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
   CXX = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
   AR = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-ar.exe
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES := -D__CELLOS_LV2__ -D__POWERPC__ -D__ppc__ -DUSE_CACHE_PREFETCH -DBRANCHLESS_GBA_GFX
   STATIC_LINKING = 1
else ifeq ($(platform), sncps3)
   TARGET := $(TARGET_NAME)_libretro_ps3.a
   CC = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
   CXX = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
   AR = $(CELL_SDK)/host-win32/sn/bin/ps3snarl.exe
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES := -D__CELLOS_LV2__ -D__POWERPC__ -D__ppc__ -DUSE_CACHE_PREFETCH -DBRANCHLESS_GBA_GFX
   STATIC_LINKING = 1

# PSP
else ifeq ($(platform), psp1)
	TARGET := $(TARGET_NAME)_libretro_psp1.a
	CC = psp-gcc$(EXE_EXT)
	CXX = psp-g++$(EXE_EXT)
	AR = psp-ar$(EXE_EXT)
	ENDIANNESS_DEFINES = -DLSB_FIRST
	PLATFORM_DEFINES := -DBRANCHLESS_GBA_GFX -DPSP 
	CFLAGS += -G0
	CXXFLAGS += -G0
   STATIC_LINKING = 1

# Vita
else ifeq ($(platform), vita)
	TARGET := $(TARGET_NAME)_libretro_vita.a
	CC = arm-vita-eabi-gcc$(EXE_EXT)
	CXX = arm-vita-eabi-g++$(EXE_EXT)
	AR = arm-vita-eabi-ar$(EXE_EXT)
	ENDIANNESS_DEFINES = -DLSB_FIRST
	PLATFORM_DEFINES := -DBRANCHLESS_GBA_GFX -DVITA
   STATIC_LINKING = 1
else ifeq ($(platform), psl1ght)
   TARGET := $(TARGET_NAME)_libretro_psl1ght.a
   CC = $(PS3DEV)/ppu/bin/ppu-gcc$(EXE_EXT)
   CXX = $(PS3DEV)/ppu/bin/ppu-g++$(EXE_EXT)
   AR = $(PS3DEV)/ppu/bin/ppu-ar$(EXE_EXT)
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES := -D__CELLOS_LV2__ -D__POWERPC__ -D__ppc__ -DUSE_CACHE_PREFETCH -DBRANCHLESS_GBA_GFX
   STATIC_LINKING = 1
else ifeq ($(platform), xenon)
   TARGET := $(TARGET_NAME)_libretro_xenon360.a
   CC = xenon-gcc$(EXE_EXT)
   CXX = xenon-g++$(EXE_EXT)
   AR = xenon-ar$(EXE_EXT)
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES := -D__LIBXENON__ -D__POWERPC__ -D__ppc__
   STATIC_LINKING = 1
else ifeq ($(platform), ngc)
   TARGET := $(TARGET_NAME)_libretro_ngc.a
   CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
   CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
   AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES += -DGEKKO -DHW_DOL -mrvl -mcpu=750 -meabi -mhard-float -D__ppc__
   STATIC_LINKING = 1
else ifeq ($(platform), wii)
   TARGET := $(TARGET_NAME)_libretro_wii.a
   CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
   CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
   AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
   ENDIANNESS_DEFINES = -DBLARGG_BIG_ENDIAN=1 -DWORDS_BIGENDIAN
   PLATFORM_DEFINES += -DGEKKO -DHW_RVL -mrvl -mcpu=750 -meabi -mhard-float -D__ppc__
   STATIC_LINKING = 1
else ifneq (,$(findstring armv,$(platform)))
   TARGET := $(TARGET_NAME)_libretro.so
   SHARED := -shared -Wl,--no-undefined
   ENDIANNESS_DEFINES = -DLSB_FIRST
   fpic := -fPIC
ifneq (,$(findstring cortexa8,$(platform)))
   PLATFORM_DEFINES += -marm -mcpu=cortex-a8
else ifneq (,$(findstring cortexa9,$(platform)))
   PLATFORM_DEFINES += -marm -mcpu=cortex-a9
endif
   PLATFORM_DEFINES += -marm
ifneq (,$(findstring neon,$(platform)))
   PLATFORM_DEFINES += -mfpu=neon
   HAVE_NEON = 1
endif
ifneq (,$(findstring softfloat,$(platform)))
   PLATFORM_DEFINES += -mfloat-abi=softfp
else ifneq (,$(findstring hardfloat,$(platform)))
   PLATFORM_DEFINES += -mfloat-abi=hard
endif
   PLATFORM_DEFINES += -DARM
else ifeq ($(platform), emscripten)
   TARGET := $(TARGET_NAME)_libretro_emscripten.bc
	ENDIANNESS_DEFINES = -DLSB_FIRST
else
   TARGET := $(TARGET_NAME)_libretro.dll
   CC ?= gcc
   CXX ?= g++
   SHARED := -shared -static-libgcc -static-libstdc++ -Wl,-no-undefined -Wl,-version-script=$(LIBRETRO_DIR)/link.T
   ENDIANNESS_DEFINES = -DLSB_FIRST
	NO_MEMMEM = 1
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
	CXXFLAGS += -O0 -g
else ifeq ($(platform), emscripten)
	CFLAGS += -O2 -DNDEBUG
	CXXFLAGS += -O2 -DNDEBUG
else
	CFLAGS += -O3 -DNDEBUG -fomit-frame-pointer -flto
	CXXFLAGS += -O3 -DNDEBUG -fomit-frame-pointer -flto
	LDFLAGS += -flto
endif

include Makefile.common

CXXFLAGS += $(INCFLAGS)

OBJECTS := $(SOURCES_CXX:.cpp=.o)

DEFINES += -D__LIBRETRO__ $(ENDIANNESS_DEFINES) $(PLATFORM_DEFINES) -DINLINE=inline

CODE_DEFINES =

CFLAGS += $(CODE_DEFINES) $(fpic) $(DEFINES)
CXXFLAGS += $(fpic) $(DEFINES) -std=c++17

ifeq ($(FRONTEND_SUPPORTS_RGB565), 1)
CFLAGS += -DFRONTEND_SUPPORTS_RGB565
CXXFLAGS += -DFRONTEND_SUPPORTS_RGB565
endif

ifeq ($(NO_MEMMEM), 1)
CFLAGS += -DNO_MEMMEM
CXXFLAGS += -DNO_MEMMEM
endif

LIBS :=

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCFLAGS)

ifeq ($(platform), theos_ios)
COMMON_FLAGS := -DIOS $(COMMON_DEFINES) $(INCFLAGS) -I$(THEOS_INCLUDE_PATH) -Wno-error
$(LIBRARY_NAME)_CFLAGS += $(CFLAGS) $(COMMON_FLAGS)
$(LIBRARY_NAME)_CXXFLAGS += $(CXXFLAGS) $(COMMON_FLAGS)
${LIBRARY_NAME}_FILES = $(SOURCES_CXX) $(SOURCES_C)
include $(THEOS_MAKE_PATH)/library.mk
else
all: $(TARGET)
$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(CXX) -o $@ $(SHARED) $(OBJECTS) $(LDFLAGS) $(LIBS)
endif


clean-objs:
	rm -f $(OBJECTS)

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

.PHONY: clean clean-objs
endif
