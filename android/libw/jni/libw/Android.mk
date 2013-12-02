LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libw

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../libsigcpp/libsigc++-2.2.10 \
$(LOCAL_PATH)/../../../../../libpng/src \
$(LOCAL_PATH)/../../../../../eigen/eigen-3.2 \
$(LOCAL_PATH)/../../../../include \
$(LOCAL_PATH)/../../../../src

# Application source files:
SOURCE := ../../../../src

LOCAL_SRC_FILES := \
$(SOURCE)/base/Log.cpp \
$(SOURCE)/base/Timer.cpp  \
$(SOURCE)/base/Mutex.cpp \
$(SOURCE)/base/Referenced.cpp \
$(SOURCE)/base/Thread.cpp \
$(SOURCE)/base/FileHandle.cpp \
$(SOURCE)/base/ResourceManager.cpp \
$(SOURCE)/base/ResourceManagerPrivate.cpp \
\
$(SOURCE)/audio/AudioResource.cpp \
$(SOURCE)/audio/AudioEnginePrivate-Android.cpp \
$(SOURCE)/audio/AudioEngine.cpp \
$(SOURCE)/audio/AudioAssetPrivate.cpp \
$(SOURCE)/audio/AudioAsset.cpp \
$(SOURCE)/audio/TrackerSample.cpp \
$(SOURCE)/audio/TrackerProducerThread.cpp \
$(SOURCE)/audio/Tracker.cpp \
\
$(SOURCE)/graphics/Window.cpp \
$(SOURCE)/graphics/WindowPrivate-Android.cpp \
$(SOURCE)/graphics/ShaderProgramAsset.cpp \
$(SOURCE)/graphics/ShaderProgramAssetPrivate-GLES2.cpp \
$(SOURCE)/graphics/TextureAsset.cpp \
$(SOURCE)/graphics/TextureAssetPrivate-GLES2.cpp \
$(SOURCE)/graphics/MeshAsset.cpp \
$(SOURCE)/graphics/MeshAssetPrivate-GLES2.cpp \
$(SOURCE)/graphics/Renderer.cpp \
$(SOURCE)/graphics/RendererPrivate-GLES2.cpp \
\
$(SOURCE)/scene/Component.cpp \
$(SOURCE)/scene/NodePrivate.cpp \
$(SOURCE)/scene/Node.cpp

# Dependencies
LOCAL_SHARED_LIBRARIES := libsigcpp libpng
LOCAL_STATIC_LIBRARY   := libgnustl
LOCAL_LDLIBS           := -llog -lz -lOpenSLES -landroid -lGLESv2

include $(BUILD_SHARED_LIBRARY)
