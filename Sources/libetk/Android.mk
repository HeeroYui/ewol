LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := etk
LOCAL_STATIC_LIBRARIES := libzip

# load the common sources file of the platform
include $(LOCAL_PATH)/file.mk

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

ifeq ($(DEBUG),1)
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=3 \
                -Wall
else
LOCAL_CFLAGS := -D__PLATFORM__Android \
                -Wno-write-strings \
                -DETK_DEBUG_LEVEL=1 \
                -DMODE_RELEASE
endif



LOCAL_SRC_FILES := $(FILE_LIST)


# Ewol Test Software :
LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_STATIC_LIBRARY)


