cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# Project setup
project (MirageRender CXX)
set (VERSION_R 0)
set (VERSION_B 1)
set (VERSION_A 0)
set (BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set (RES_DIR "${PROJECT_SOURCE_DIR}/res")

# Set recommended compilers for MinGW / *Nix
if (MINGW OR UNIX)

	MESSAGE (STATUS "C/C++ Compilers: MinGW or *Nix detected, defaulting to gcc.")

	set (CMAKE_C_COMPILER "gcc")
	set (CMAKE_CXX_COMPILER "gcc")

endif ()

# Sources, headers + resources
file (GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.h")
file (GLOB_RECURSE RESOURCES "${PROJECT_SOURCE_DIR}/res/*")

# SDL2 and Lua 5.3.x
set (SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/include")
set (LUA_INCLUDE_DIR ${LUA_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/include")
if (MSVC)
	set (SDL2_LIBRARY ${SDL2_LIBRARY} "${PROJECT_SOURCE_DIR}/lib/SDL2.lib" "${PROJECT_SOURCE_DIR}/lib/SDL2main.lib")
	set (LUA_LIBRARY ${LUA_LIBRARY} "${PROJECT_SOURCE_DIR}/lib/lua*.lib")
else ()
	set (SDL2_LIBRARY ${SDL2_LIBRARY} "${PROJECT_SOURCE_DIR}/lib/libSDL2.a" "${PROJECT_SOURCE_DIR}/lib/libSDL2.dll.a" "${PROJECT_SOURCE_DIR}/lib/libSDL2main.a")
	set (LUA_LIBRARY ${LUA_LIBRARY} "${PROJECT_SOURCE_DIR}/lib/liblua.a")
endif ()

# CMake modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules")

# Add executables
add_executable (MirageRender ${SOURCES})

# Compiler flags for each generator
if (MSVC)

	MESSAGE (STATUS "Compiler flags: MSVC Detected.")

	target_compile_options (MirageRender PUBLIC
		-std=c++11
	)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")

		MESSAGE (STATUS "Compiler flags: MSVC Release detected.")

		target_compile_options (MirageRender PUBLIC
			/O2
		)

	else ()

		MESSAGE (STATUS "Compiler flags: MSVC Debug detected.")

	endif ()

else ()

	MESSAGE (STATUS "Compiler flags: MinGW or *Nix detected.")

	target_compile_options (MirageRender PUBLIC
		-std=c++11
		-Wall
		-Wextra
		-Wno-unused-variable
		-Wno-unused-parameter
		-Wno-unused-function
		-m64
	)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")

		MESSAGE (STATUS "Compiler flags: MinGW or *Nix Release detected.")

		target_compile_options (MirageRender PUBLIC
			-Ofast
		)

	else ()

		MESSAGE (STATUS "Compiler flags: MinGW or *Nix Debug detected.")

	endif ()

endif ()

# Linker flags for each generator
if (MINGW)

	MESSAGE (STATUS "Linker flags: MinGW Detected.")

	target_link_libraries (MirageRender PUBLIC
		-lmingw32
	)

endif ()

if (MINGW OR UNIX)

	MESSAGE (STATUS "Linker flags: MinGW or *Nix Detected.")

	target_link_libraries (MirageRender PUBLIC
		-lstdc++
		-lm
		-static-libgcc
		-static-libstdc++
	)

	if (CMAKE_BUILD_TYPE STREQUAL "Release")

		MESSAGE (STATUS "Linker flags: MinGW or *Nix Release detected.")

	else ()

		MESSAGE (STATUS "Linker flags: MinGW or *Nix Debug detected.")

	endif()

endif ()

# Includes
target_include_directories (MirageRender PUBLIC "${PROJECT_SOURCE_DIR}/src/")

# Dependencies
find_package (SDL2 REQUIRED)
find_package (Lua 5.3 REQUIRED)

target_include_directories (MirageRender PUBLIC ${SDL2_INCLUDE_DIR})
target_include_directories (MirageRender PUBLIC ${LUA_INCLUDE_DIR})

target_link_libraries (MirageRender PUBLIC
	${SDL2_LIBRARY}
	${LUA_LIBRARY}
)

# Install project
install (TARGETS MirageRender DESTINATION ${BIN_DIR})
install (DIRECTORY ${RES_DIR} DESTINATION ${BIN_DIR})

# Configure package
set (CPACK_PACKAGE_NAME "MirageRender")
set (CPACK_PACKAGE_VERSION ${VERSION_R}.${VERSION_B}.${VERSION_A})
set (CPACK_MONOLITHIC_INSTALL 1)
include (CPack)