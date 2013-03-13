#################################################################################
# build libmpg123
#################################################################################

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mpg123
LOCAL_CFLAGS += -DNO_32BIT -DNO_REAL -DREAL_IS_FIXED
ifeq ($(TARGET_ARCH),x86)
	LOCAL_CFLAGS += -DOPT_I386
else
	LOCAL_ARM_MODE := arm
	LOCAL_CFLAGS += -DOPT_ARM
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)
LOCAL_SRC_FILES := compat.c \
	dct64.c \
	dct64_i386.c \
	equalizer.c \
	format.c \
	frame.c \
	icy.c \
	icy2utf8.c \
	id3.c \
	index.c \
	layer1.c \
	layer2.c \
	layer3.c \
	libmpg123.c \
	ntom.c \
	optimize.c \
	parse.c \
	readers.c \
	stringbuf.c \
	synth.c \
	synth_8bit.c \
	synth_arm.S \
	tabinit.c

include $(BUILD_STATIC_LIBRARY)