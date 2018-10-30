call ndk-build

mkdir feather
mkdir feather\include
mkdir feather\armeabi-v7a

copy /y  src\*.h                 feather\include
copy /y  libs\armeabi-v7a\*.so   feather\armeabi-v7a
del  feather\include\feather_simple_generated.h
cmd