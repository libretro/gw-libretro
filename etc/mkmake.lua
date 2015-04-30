local template = [[
DEBUG = 0
LOG_PERFORMANCE = 1



CORE_DIR      = ..
INCDIRS       = -I${PLAT_INCDIR}
SOURCES_C     =
SOURCES_CXX   =
SOURCES_LUA_C =
LUA_FILES     =
PNG_FILES     =

include Makefile.common



CC  = ${CC}
CXX = ${CXX}
AS  = ${AS}



OBJEXT = .${EXT}.o
SOEXT  = .${EXT}.${SO}
TARGET = gw_libretro$(SOEXT)



PLATDEFS     = ${PLAT_DEFS}
PLATCFLAGS   = ${PLAT_CFLAGS}
PLATCXXFLAGS = ${PLAT_CXXFLAGS}
PLATLDFLAGS  = ${PLAT_LDFLAGS}



RETRODEFS     = -D__LIBRETRO__
RETROCFLAGS   =
RETROCXXFLAGS =
RETROLDFLAGS  =



COREDEFS     = -Drl_malloc=malloc -Drl_free=free -Dgwlua_malloc=malloc -Dgwlua_realloc=realloc -Dgwlua_free=free -Dgwrom_malloc=malloc -Dgwrom_free=free
CORECFLAGS   = -Wall --std=c99
CORECXXFLAGS = -Wall
CORELDFLAGS  =



DEFINES  = $(PLATDEFS) $(RETRODEFS) $(COREDEFS)
CFLAGS   = $(PLATCFLAGS) $(RETROCFLAGS) $(CORECFLAGS) $(DEFINES) $(INCDIRS)
CXXFLAGS = $(PLATCXXFLAGS) $(RETROCXXFLAGS) $(CORECXXFLAGS) $(DEFINES) $(INCDIRS)
LDFLAGS  = $(PLATLDFLAGS) $(RETROLDFLAGS) $(CORELDFLAGS) -shared



ifeq ($(DEBUG), 1)
	CFLAGS   += -O0 -g
	CXXFLAGS += -O0 -g
  LDFLAGS  += -g
else
	CFLAGS   += -O3 -DNDEBUG
	CXXFLAGS += -O3 -DNDEBUG
endif

ifeq ($(LOG_PERFORMANCE), 1)
	CFLAGS   += -DLOG_PERFORMANCE
	CXXFLAGS += -DLOG_PERFORMANCE
endif



HEADERS = $(LUA_FILES:.lua=.h) $(PNG_FILES:.png=.h)
OBJS    = $(SOURCES_C:.c=$(OBJEXT)) $(SOURCES_CXX:.cpp=$(OBJEXT)) $(SOURCES_LUA_C:.c=$(OBJEXT)) $(CORE_DIR)/android/missing$(OBJEXT)

%$(OBJEXT): %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFINES) $(INCDIRS)

%$(OBJEXT): %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES) $(INCDIRS)

%$(OBJEXT): %.S
	$(AS) -c -o $@ $< $(CFLAGS) $(DEFINES) $(INCDIRS)

%.h: %.lua
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.png
	retroluxury/etc/luai/luai retroluxury/etc/rlrle.lua $<
	xxd -i $(basename $<).rle | sed "s/unsigned/const unsigned/g" > $@
	rm -f $(basename $<).rle

all: $(TARGET)

$(TARGET): $(HEADERS) $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

$(CORE_DIR)/gwlua/functions.c: $(CORE_DIR)/gwlua/entries.c $(HEADERS)
	touch $@

$(CORE_DIR)/gwlua/entries.c: $(CORE_DIR)/gwlua/entries.gperf
	gperf -c -C -l -L C -t -7 -m 100 -I --output-file=$@ $<

clean-objs:
	rm -f $(OBJS)

clean: clean-objs
	rm -f $(TARGET)

dist-clean: clean
	rm -f $(HEADERS)
	rm -f $(CORE_DIR)/gwlua/entries.c

.PHONY: $(TARGET) clean-objs clean dist-clean
]]

local host = 'linux-x86'
--local host = 'windows-x86_64'

local platforms = {
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_arm64_v8a = {
    CC            = '$(NDK_ROOT)/toolchains/aarch64-linux-android-4.9/prebuilt/' .. host .. '/bin/aarch64-linux-android-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/aarch64-linux-android-4.9/prebuilt/' .. host .. '/bin/aarch64-linux-android-g++',
    AS            = '$(NDK_ROOT)/toolchains/aarch64-linux-android-4.9/prebuilt/' .. host .. '/bin/aarch64-linux-android-as',
    EXT           = 'android_arm64_v8a',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-21/arch-arm64/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-21/arch-arm64 -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_x86_64 = {
    CC            = '$(NDK_ROOT)/toolchains/x86_64-4.9/prebuilt/' .. host .. '/bin/x86_64-linux-android-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/x86_64-4.9/prebuilt/' .. host .. '/bin/x86_64-linux-android-g++',
    AS            = '$(NDK_ROOT)/toolchains/x86_64-4.9/prebuilt/' .. host .. '/bin/x86_64-linux-android-as',
    EXT           = 'android_x86_64',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-21/arch-x86_64/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-21/arch-x86_64 -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_mips64 = {
    CC            = '$(NDK_ROOT)/toolchains/mips64el-linux-android-4.9/prebuilt/' .. host .. '/bin/mips64el-linux-android-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/mips64el-linux-android-4.9/prebuilt/' .. host .. '/bin/mips64el-linux-android-g++',
    AS            = '$(NDK_ROOT)/toolchains/mips64el-linux-android-4.9/prebuilt/' .. host .. '/bin/mips64el-linux-android-as',
    EXT           = 'android_mips64',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-21/arch-mips64/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-fpic -fno-strict-aliasing -finline-functions -ffunction-sections -funwind-tables -fmessage-length=0 -fno-inline-functions-called-once -fgcse-after-reload -frerun-cse-after-loop -frename-registers -no-canonical-prefixes -fomit-frame-pointer -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-21/arch-mips64 -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_arm_v7a = {
    CC            = '$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.8/prebuilt/' .. host .. '/bin/arm-linux-androideabi-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.8/prebuilt/' .. host .. '/bin/arm-linux-androideabi-g++',
    AS            = '$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.8/prebuilt/' .. host .. '/bin/arm-linux-androideabi-as',
    EXT           = 'android_arm_v7a',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-3/arch-arm/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-3/arch-arm -lgcc -no-canonical-prefixes -march=armv7-a -Wl,--fix-cortex-a8 -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_arm_v5te = {
    CC            = '$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.8/prebuilt/' .. host .. '/bin/arm-linux-androideabi-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.8/prebuilt/' .. host .. '/bin/arm-linux-androideabi-g++',
    AS            = '$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.8/prebuilt/' .. host .. '/bin/arm-linux-androideabi-as',
    EXT           = 'android_arm_v5te',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-3/arch-arm/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -march=armv5te -mtune=xscale -msoft-float -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-3/arch-arm -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_x86 = {
    CC            = '$(NDK_ROOT)/toolchains/x86-4.8/prebuilt/' .. host .. '/bin/i686-linux-android-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/x86-4.8/prebuilt/' .. host .. '/bin/i686-linux-android-g++',
    AS            = '$(NDK_ROOT)/toolchains/x86-4.8/prebuilt/' .. host .. '/bin/i686-linux-android-as',
    EXT           = 'android_x86',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-9/arch-x86/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-ffunction-sections -funwind-tables -no-canonical-prefixes -fstack-protector -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-9/arch-x86 -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  android_mips = {
    CC            = '$(NDK_ROOT)/toolchains/mipsel-linux-android-4.8/prebuilt/' .. host .. '/bin/mipsel-linux-android-gcc',
    CXX           = '$(NDK_ROOT)/toolchains/mipsel-linux-android-4.8/prebuilt/' .. host .. '/bin/mipsel-linux-android-g++',
    AS            = '$(NDK_ROOT)/toolchains/mipsel-linux-android-4.8/prebuilt/' .. host .. '/bin/mipsel-linux-android-as',
    EXT           = 'android_mips',
    SO            = 'so',
    PLAT_INCDIR   = '$(NDK_ROOT)/platforms/android-9/arch-mips/usr/include',
    PLAT_DEFS     = '-DANDROID -DINLINE=inline -DHAVE_STDINT_H -DBSPF_UNIX -DHAVE_INTTYPES -DLSB_FIRST -Dl_getlocaledecpoint\\(\\)=\\(\\\'.\\\'\\)',
    PLAT_CFLAGS   = '-fpic -fno-strict-aliasing -finline-functions -ffunction-sections -funwind-tables -fmessage-length=0 -fno-inline-functions-called-once -fgcse-after-reload -frerun-cse-after-loop -frename-registers -no-canonical-prefixes -fomit-frame-pointer -funswitch-loops -finline-limit=300 -Wa,--noexecstack -Wformat -Werror=format-security',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-shared --sysroot=$(NDK_ROOT)/platforms/android-9/arch-mips -lgcc -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -lc -lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  linux_x86 = {
    CC            = 'gcc',
    CXX           = 'g++',
    AS            = 'as',
    EXT           = 'linux_x86',
    SO            = 'so',
    PLAT_INCDIR   = '',
    PLAT_DEFS     = '',
    PLAT_CFLAGS   = '-m32 -fpic -fstrict-aliasing',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  linux_x86_64 = {
    CC            = 'gcc',
    CXX           = 'g++',
    AS            = 'as',
    EXT           = 'linux_x86_64',
    SO            = 'so',
    PLAT_INCDIR   = '',
    PLAT_DEFS     = '',
    PLAT_CFLAGS   = '-m64 -fpic -fstrict-aliasing',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  windows_x86 = {
    CC            = '$(MINGW32)/bin/i686-w64-mingw32-gcc',
    CXX           = '$(MINGW32)/bin/i686-w64-mingw32-g++',
    AS            = '$(MINGW32)/bin/i686-w64-mingw32-as',
    EXT           = 'windows_x86',
    SO            = 'dll',
    PLAT_INCDIR   = '',
    PLAT_DEFS     = '',
    PLAT_CFLAGS   = '-fpic -fstrict-aliasing',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-lm'
  },
  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  windows_x86_64 = {
    CC            = '$(MINGW64)/bin/x86_64-w64-mingw32-gcc',
    CXX           = '$(MINGW64)/bin/x86_64-w64-mingw32-g++',
    AS            = '$(MINGW64)/bin/x86_64-w64-mingw32-as',
    EXT           = 'windows_x86_64',
    SO            = 'dll',
    PLAT_INCDIR   = '',
    PLAT_DEFS     = '',
    PLAT_CFLAGS   = '-fpic -fstrict-aliasing',
    PLAT_CXXFLAGS = '${PLAT_CFLAGS}',
    PLAT_LDFLAGS  = '-lm'
  },
}

for plat, defs in pairs( platforms ) do
  local templ = template
  local equal
  
  repeat
    equal = true
    
    for def, value in pairs( defs ) do
      local templ2 = templ:gsub( '%${' .. def .. '}', value )
      equal = equal and templ == templ2
      templ = templ2
    end
  until equal
  
  local file, err = io.open( 'Makefile.' .. plat, 'w' )
  if not file then error( err ) end
  
  file:write( templ )
  file:close()
end
