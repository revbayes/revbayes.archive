set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj")
set(Boost_INCLUDE_DIR "/usr/${TOOLCHAIN_PREFIX}/sys-root/mingw/include/")
set(Boost_LIBRARY_DIRS "/usr/${TOOLCHAIN_PREFIX}/sys-root/mingw/lib/")
set(ENV{PKG_CONFIG_PATH} "/usr/${TOOLCHAIN_PREFIX}/sys-root/mingw/lib/pkgconfig")

# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH "/usr/${TOOLCHAIN_PREFIX}")

# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)