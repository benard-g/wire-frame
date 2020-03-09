set VCPKG_GIT_URL=https://github.com/microsoft/vcpkg.git
set VCPKG_GIT_TAG=2020.01
set VCPKG_DIR=.vcpkg

if not exist %VCPKG_DIR% (
    git clone --branch %VCPKG_GIT_TAG% %VCPKG_GIT_URL% %VCPKG_DIR%
)

cd %VCPKG_DIR%

if not exist vcpkg.exe (
    bootstrap-vcpkg.bat
)

vcpkg.exe install sfml:x64-windows
