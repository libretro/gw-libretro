LOCAL_PATH := $(call my-dir)

CORE_DIR := $(LOCAL_PATH)/../..

INCLUDES      :=
SOURCES_C     :=
SOURCES_LUA_C :=

include $(CORE_DIR)/build/Makefile.common
SOURCES_C := $(filter-out %/version.c, $(SOURCES_C))

DEFINES := -Drl_malloc=malloc -Drl_free=free -Dgwlua_malloc=malloc -Dgwlua_realloc=realloc -Dgwlua_free=free -Dgwrom_malloc=malloc -Dgwrom_free=free -DBZ_NO_STDIO
COREFLAGS := $(RETRODEFS) $(INCLUDES) $(DEFINES)

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
  COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)
LOCAL_MODULE    := retro
LOCAL_SRC_FILES := $(SOURCES_C) $(SOURCES_LUA_C)
LOCAL_CFLAGS    := $(COREFLAGS)
LOCAL_LDFLAGS   := -Wl,-version-script=$(CORE_DIR)/build/link.T
include $(BUILD_SHARED_LIBRARY)
