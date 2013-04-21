project(marshmallow_lua)

add_definitions(-DLUA_COMPAT_ALL)
if(UNIX)
	if(APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		add_definitions(-DLUA_USE_MACOSX)
		list(APPEND LUA_LIBS readline)
	elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		add_definitions(-DLUA_USE_LINUX)
		list(APPEND LUA_LIBS dl readline)
	elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
		add_definitions(-DLUA_USE_LINUX)
		list(APPEND LUA_LIBS readline)
	else()
		add_definitions(-DLUA_USE_POSIX)
		list(ADD LUA_LIBS dl readline)
	endif()
elseif(WIN32)
	add_definitions(-DLUA_WIN)
	if(BUILD_SHARED_LIBS)
		add_definitions(-DLUA_BUILD_AS_DLL)
	endif()
endif()

set(LUA_BASE ${CMAKE_CURRENT_SOURCE_DIR}/code)

include_directories(${LUA_INCLUDE_DIRS})
file(GLOB LUA_SRCS ${LUA_BASE}/src/*.c)
list(REMOVE_ITEM LUA_SRCS ${LUA_BASE}/src/lua.c)

set(LUA_LIBS ${M_LIBRARY})

add_library(${LUA_LIBRARY} ${LUA_SRCS})
target_link_libraries(${LUA_LIBRARY} ${LUA_LIBS})

install(TARGETS ${LUA_LIBRARY}
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

