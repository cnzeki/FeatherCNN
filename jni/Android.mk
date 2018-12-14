            
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SRC_DIR :=..
LOCAL_SRC_FILES  += \
   $(SRC_DIR)/src/blob.cpp \
   $(SRC_DIR)/src/common.cpp \
   $(SRC_DIR)/src/layer.cpp \
   $(SRC_DIR)/src/layer_factory.cpp \
   $(SRC_DIR)/src/mempool.cpp \
   $(SRC_DIR)/src/net.cpp \
   $(SRC_DIR)/src/layers/batchnorm_layer.cpp \
   $(SRC_DIR)/src/layers/concat_layer.cpp \
   $(SRC_DIR)/src/layers/filter_layer.cpp \
   $(SRC_DIR)/src/layers/lrn_layer.cpp \
   $(SRC_DIR)/src/layers/prelu_layer.cpp \
   $(SRC_DIR)/src/layers/relu_layer.cpp \
   $(SRC_DIR)/src/layers/reshape_layer.cpp \
   $(SRC_DIR)/src/layers/scale_layer.cpp \
   $(SRC_DIR)/src/layers/slice_layer.cpp \
   $(SRC_DIR)/src/layers/softmax_layer.cpp \
   $(SRC_DIR)/src/arm/depthwise.cpp \
   $(SRC_DIR)/src/arm/generic_kernels.cpp \
   $(SRC_DIR)/src/arm/helper.cpp \
   $(SRC_DIR)/src/arm/sgemm.cpp \
   $(SRC_DIR)/src/arm/sgemm_legacy.cpp \
   $(SRC_DIR)/src/arm/sgemv.cpp \
   $(SRC_DIR)/src/arm/winograd_kernels.cpp \
   $(SRC_DIR)/src/arm/winograd_kernels_F63.cpp 

# Open OpenMP
LOCAL_CFLAGS := -DHAVE_OPENMP  
LOCAL_CXXFLAGS := -fopenmp
LOCAL_CFLAGS +=  -fopenmp
LOCAL_LDLIBS := -llog -fopenmp
# include dirs
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src \
                    $(LOCAL_PATH)/../src/arm \
                    $(LOCAL_PATH)/../src/flatbuffers \
                    $(LOCAL_PATH)/../src/general \
                    $(LOCAL_PATH)/../src/layers \
                    $(LOCAL_PATH)/../src/wrapper \
                    
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS += -O3 -std=c++11
#LOCAL_ARM_NEON := true 

LOCAL_MODULE     := dnn
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
#include $(BUILD_STATIC_LIBRARY)

