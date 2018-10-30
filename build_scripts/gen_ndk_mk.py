#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import sys

def is_cpp_file(path):
    img_exts = ['.c','.cc','.cpp','.cxx']
    _, ext = os.path.splitext(path)
    ext = ext.lower()
    return ext in img_exts or ext == ''
        
def makedirs(path):
    dir,fname = os.path.split(path)
    if not os.path.exists(dir):
        try:
            os.makedirs(dir)
        except:
            pass
            
            
def file_walker(dir, visitor):
    '''
        Recursive walk through a dir
    '''
    filenames = os.listdir(dir)
    for filename in filenames:
        fullpath = os.path.join(dir,filename)
        if os.path.isdir(fullpath):
            file_walker(fullpath, visitor)
        elif os.path.isfile(fullpath):
            visitor.process(fullpath)
            
header=r'''            
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SRC_DIR :=..
'''

ending=r'''
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
                    $(LOCAL_PATH)/../src/layers 
                    
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS += -O3 -std=c++11
#LOCAL_ARM_NEON := true 

LOCAL_MODULE     := dnn
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)
#include $(BUILD_STATIC_LIBRARY)

'''

class Visitor:
    def __init__(self):
        self.files = []
    
    def process(self, path):
        if is_cpp_file(path):
            path = path.replace('\\', '/')
            self.files.append(path)
        
    def get_files(self):
        return self.files

# build for arm        
arm_subdirs = ['src', 'src/flatbuffers', 'src/layers', 'src/arm']

# scan c/c++ files 
'''   
visitor = Visitor()
file_walker('src', visitor)
files = visitor.get_files() 
'''
cpp_files = []     
for subdir in arm_subdirs:
    filenames = os.listdir(subdir)
    for filename in filenames:
        fullpath = os.path.join(subdir, filename)
        if os.path.isfile(fullpath):
            if is_cpp_file(fullpath):
                path = fullpath.replace('\\', '/')
                path = path.strip()
                cpp_files.append(path)
            
# write Android.mk
with open('jni/Android.mk', 'w') as f:
    f.write(header)
    f.write('LOCAL_SRC_FILES  += \\\n')
    nfiles = len(cpp_files)
    for i in range(nfiles):
        fp = cpp_files[i]
        if i != nfiles - 1:
            f.write('   $(SRC_DIR)/%s \\\n' % (fp))
        else:
            f.write('   $(SRC_DIR)/%s\n' % (fp))
    f.write(ending)