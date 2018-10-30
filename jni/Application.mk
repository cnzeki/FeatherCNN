# The ARMv7 is significanly faster due to the use of the hardware FPU
NDK_TOOLCHAIN_VERSION := 4.9
APP_ABI := armeabi-v7a
APP_PLATFORM := android-9
APP_STL := gnustl_static
APP_MODULES := dnn
#APP_STL := gnustl_shared
APP_CFLAGS := -O3 -mfpu=neon-vfpv4  -march=armv7-a -mtune=cortex-a8 -ftree-vectorize -mfloat-abi=softfp  -ffast-math 
#APP_CFLAGS := -O3 -mfpu=vfpv3 -mfloat-abi=softfp  -ffast-math -fpic 
APP_CPPFLAGS += -fexceptions -frtti
APP_OPTIM := release
APP_CFLAGS += -Wno-error=format-security


