LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := uecho
LOCAL_SRC_FILES := ../client.c ../utils.c ../main.c
include $(BUILD_EXECUTABLE)