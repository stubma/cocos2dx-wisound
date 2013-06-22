# macros to include all files with same extension

define all-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "$(2)" -and -not -name ".*") \
 )
endef

define all-cpp-files-under
$(call all-files-under,$(1),*.cpp)
endef

# module

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dx-wisound
LOCAL_CFLAGS += -DBACKEND_MEDIAPLAYER
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) \
	$(LOCAL_PATH)/src/backends/mediaplayer
LOCAL_SRC_FILES := src/SimpleAudioEngine.cpp \
	$(call all-cpp-files-under,src/backends/mediaplayer)	
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos2dx-common

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx-common)
