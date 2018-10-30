set rootdir=%cd%
::{ protobuf begin
set version=3.6.1
echo "protobuf v%version%"
set protobuf_file=protobuf-cpp-%version%.zip
if exist %protobuf_file% (
    echo Found %protobuf_file%
) else (
    echo download ...
    wget https://github.com/protocolbuffers/protobuf/releases/download/v%version%/protobuf-cpp-%version%.zip
)

if not exist %protobuf_file% (
    echo "protobuf source file not found"
    echo "You can download manually from https://github.com/protocolbuffers/protobuf/releases/"
    exit
)
echo "extract ..."
unzip -o -q protobuf-cpp-%version%.zip

cd protobuf-%version%

mkdir install

set gitbin="C:\Program Files\Git\cmd"

set cmakebin="C:\Program Files\CMake3.8\bin"

set PATH=%gitbin%;%cmakebin%;%PATH%

cd cmake

mkdir build & cd build

mkdir release & cd release

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=../../../../install ../..
 
nmake & nmake install

cd ..

:: vs solution
mkdir sln & cd sln
cmake -G "Visual Studio 14 2015 Win64"  -DCMAKE_INSTALL_PREFIX=../../../../install ../..
cd ..

echo "done protobuf"
:: protobuf end }

cd %rootdir%
:: compile caffe proto
echo "compile caffe.proto"
::install\bin\protoc --cpp_out=dllexport_decl=LIBPROTOBUF_EXPORT:. ./caffe.proto
install\bin\protoc --cpp_out=. ./caffe.proto
echo "building tool"
mkdir build & cd build
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.. ..
nmake
cd ..

:: vs solution
mkdir sln & cd sln
cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.. ..
cd ..

cd %rootdir%
echo "done !"







