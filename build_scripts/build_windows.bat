mkdir build-windows
set pthread_zip=pthreads-w32-2-9-1-release.zip
if exist %pthread_zip% (
    echo Found %pthread_zip%
) else (
    echo download ...
    tools\wget ftp://sourceware.org/pub/pthreads-win32/%pthread_zip%
)

if not exist %pthread_zip% (
    echo "pthread not found"
    echo "You can download manually from ftp://sourceware.org/pub/pthreads-win32/%pthread_zip%"
    exit
)
echo "extract ..."
tools\unzip  -o -q %pthread_zip% -d pthreads
:: vs solution
cd build-windows
cmake -G "Visual Studio 14 2015 Win64"  -DCMAKE_INSTALL_PREFIX=.. ..
cd ..