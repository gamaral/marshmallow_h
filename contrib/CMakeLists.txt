###############################################################################
# TinyXML2 (REQUIRED)
option(MARSHMALLOW_CONTRIB_TINYXML2 "Use bundled TinyXML2 library" ON)
if(MARSHMALLOW_CONTRIB_TINYXML2)
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/code/tinyxml2.h)
		message(FATAL_ERROR "\n** tinyxml2 submodule is unavailable **\n"
		                    "Please run the following command: \n"
		                    "  git submodule update --init contrib/tinyxml2"
		)
	endif()

	unset(TINYXML2_LIBRARY CACHE)
	unset(TINYXML2_INCLUDE_DIR CACHE)

	set(TINYXML2_LIBRARY marshmallow_tinyxml2)
	set(TINYXML2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/code)
	set(TINYXML2_FOUND TRUE)

	add_subdirectory(tinyxml2)

	set(TINYXML2_LIBRARY ${TINYXML2_LIBRARY} PARENT_SCOPE)
	set(TINYXML2_INCLUDE_DIR ${TINYXML2_INCLUDE_DIR} PARENT_SCOPE)
else()
	find_package(TinyXML2 REQUIRED)
endif()

message(STATUS "TinyXML2 library:\n"
               "	${TINYXML2_LIBRARY}"
)
set(TINYXML2_FOUND ${TINYXML2_FOUND} PARENT_SCOPE)

###############################################################################
# zlib Compression Library (REQUIRED)
option(MARSHMALLOW_CONTRIB_ZLIB "Use bundled zlib compression library" ON)
if(MARSHMALLOW_CONTRIB_ZLIB)
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/zlib/code/CMakeLists.txt)
		message(FATAL_ERROR "\n** zlib submodule is unavailable **\n"
		                    "Please run the following command: \n"
		                    "  git submodule update --init contrib/zlib"
		)
	endif()

	unset(ZLIB_INCLUDE_DIR CACHE)
	unset(ZLIB_LIBRARY CACHE)

	set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib/code
	                     ${CMAKE_CURRENT_BINARY_DIR}/zlib/code
	)
	set(ZLIB_LIBRARY marshmallow_zlib)
	set(ZLIB_FOUND TRUE)

	add_subdirectory(zlib)

	set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIR} PARENT_SCOPE)
	set(ZLIB_LIBRARY ${ZLIB_LIBRARY} PARENT_SCOPE)
else()
	find_package(ZLIB REQUIRED)
endif()

message(STATUS "zlib compression library:\n"
               "	${ZLIB_LIBRARY}"
)
set(ZLIB_FOUND ${ZLIB_FOUND} PARENT_SCOPE)

###############################################################################
# libpng (REQUIRED)
option(MARSHMALLOW_CONTRIB_LIBPNG "Use bundled libpng library" ON)
if(MARSHMALLOW_CONTRIB_LIBPNG)
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libpng/code/CMakeLists.txt)
		message(FATAL_ERROR "\n** libpng submodule is unavailable **\n"
		                    "Please run the following command: \n"
		                    "  git submodule update --init contrib/libpng"
		)
	endif()

	unset(PNG_DEFINITIONS CACHE)
	unset(PNG_PNG_INCLUDE_DIR CACHE)
	unset(PNG_LIBRARY CACHE)

	if(BUILD_SHARED_LIBS)
		set(PNG_DEFINITIONS)
	else()
		set(PNG_DEFINITIONS "-DPNG_STATIC")
	endif()
	set(PNG_PNG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libpng/code
	                        ${CMAKE_CURRENT_BINARY_DIR}/libpng/code
	)
	set(PNG_LIBRARY marshmallow_libpng)
	set(PNG_FOUND TRUE)

	add_subdirectory(libpng)

	set(PNG_DEFINITIONS ${PNG_DEFINITIONS} PARENT_SCOPE)
	set(PNG_PNG_INCLUDE_DIR ${PNG_PNG_INCLUDE_DIR} PARENT_SCOPE)
	set(PNG_LIBRARY ${PNG_LIBRARY} PARENT_SCOPE)
else()
	find_package(PNG REQUIRED)
endif()

message(STATUS "libpng library:\n"
               "	${PNG_LIBRARY}"
)
set(PNG_FOUND ${PNG_FOUND} PARENT_SCOPE)

###############################################################################
# libogg (REQUIRED)
option(MARSHMALLOW_CONTRIB_LIBOGG "Use bundled Ogg container format library" ON)
if(MARSHMALLOW_CONTRIB_LIBOGG)
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libogg/code/src)
		message(FATAL_ERROR "\n** libogg submodule is unavailable **\n"
		                    "Please run the following command: \n"
		                    "  git submodule update --init contrib/libogg"
		)
	endif()

	unset(OGG_INCLUDE_DIR CACHE)
	unset(OGG_LIBRARY CACHE)

	set(OGG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/libogg/code/include
	                    ${PROJECT_BINARY_DIR}/contrib/libogg/code/include)
	set(OGG_LIBRARY marshmallow_libogg)
	set(OGG_FOUND TRUE)

	add_subdirectory(libogg)

	set(OGG_INCLUDE_DIR ${OGG_INCLUDE_DIR} PARENT_SCOPE)
	set(OGG_LIBRARY ${OGG_LIBRARY} PARENT_SCOPE)
else()
	find_package(Ogg REQUIRED)
endif()

message(STATUS "Ogg container format library:\n"
               "	${OGG_LIBRARY}"
)
set(OGG_FOUND ${OGG_FOUND} PARENT_SCOPE)

###############################################################################
# libvorbis (REQURED)
option(MARSHMALLOW_CONTRIB_LIBVORBIS "Use bundled Vorbis audio compression library" ON)
if(MARSHMALLOW_CONTRIB_LIBVORBIS)
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/code/lib)
		message(FATAL_ERROR "\n** libvorbis submodule is unavailable **\n"
		                    "Please run the following command: \n"
		                    "  git submodule update --init contrib/libvorbis"
		)
	endif()

	unset(VORBIS_FILE_INCLUDE_DIR CACHE)
	unset(VORBIS_FILE_LIBRARY CACHE)
	unset(VORBIS_INCLUDE_DIRS CACHE)
	unset(VORBIS_LIBRARIES CACHE)
	unset(VORBIS_VORBIS_INCLUDE_DIR CACHE)
	unset(VORBIS_VORBIS_LIBRARY CACHE)

	set(VORBIS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/code/include)
	set(VORBIS_VORBIS_INCLUDE_DIR ${VORBIS_INCLUDE_DIRS})
	set(VORBIS_FILE_INCLUDE_DIR ${VORBIS_INCLUDE_DIRS})

	set(VORBIS_VORBIS_LIBRARY marshmallow_libvorbis)
	set(VORBIS_FILE_LIBRARY marshmallow_libvorbisfile)
	set(VORBIS_LIBRARIES ${VORBIS_VORBIS_LIBRARY}
	                     ${VORBIS_FILE_LIBRARY}
	)

	set(VORBIS_VORBIS_FOUND TRUE)
	set(VORBIS_FILE_FOUND TRUE)

	add_subdirectory(libvorbis)

	set(VORBIS_FILE_INCLUDE_DIR ${VORBIS_FILE_INCLUDE_DIR} PARENT_SCOPE)
	set(VORBIS_FILE_LIBRARY ${VORBIS_FILE_LIBRARY} PARENT_SCOPE)
	set(VORBIS_INCLUDE_DIRS ${VORBIS_INCLUDE_DIRS} PARENT_SCOPE)
	set(VORBIS_LIBRARIES ${VORBIS_LIBRARIES} PARENT_SCOPE)
	set(VORBIS_VORBIS_INCLUDE_DIR ${VORBIS_VORBIS_INCLUDE_DIR} PARENT_SCOPE)
	set(VORBIS_VORBIS_LIBRARY ${VORBIS_VORBIS_LIBRARY} PARENT_SCOPE)
else()
	find_package(Vorbis REQUIRED)
endif()

message(STATUS "Vorbis audio compression library:\n"
               "	${VORBIS_VORBIS_LIBRARY}\n"
               "	${VORBIS_FILE_LIBRARY}"
)
set(VORBIS_VORBIS_FOUND ${VORBIS_VORBIS_FOUND} PARENT_SCOPE)
set(VORBIS_FILE_FOUND ${VORBIS_FILE_FOUND} PARENT_SCOPE)

###############################################################################
# Box2D (OPTIONAL)
option(MARSHMALLOW_WITH_BOX2D "Use Box2D Physics Engine" ON)
if(MARSHMALLOW_WITH_BOX2D)
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/box2d/code/Box2D)
		message(FATAL_ERROR "\n** box2d submodule is unavailable **\n"
		                    "Please run the following command: \n"
		                    "  git submodule update --init contrib/box2d"
		)
	endif()

	unset(BOX2D_INCLUDE_DIR CACHE)
	set(BOX2D_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/box2d/code/Box2D)
	unset(BOX2D_LIBRARY CACHE)
	set(BOX2D_LIBRARY marshmallow_box2d)
	set(BOX2D_FOUND TRUE)

	add_subdirectory(box2d)

	set(BOX2D_INCLUDE_DIR ${BOX2D_INCLUDE_DIR} PARENT_SCOPE)
	set(BOX2D_LIBRARY ${BOX2D_LIBRARY} PARENT_SCOPE)
	set(BOX2D_FOUND ${BOX2D_FOUND} PARENT_SCOPE)

	message(STATUS "Box2D physics engine:\n"
		       "	${BOX2D_LIBRARY}"
	)
endif()

###############################################################################
# SQLite3 (OPTIONAL)
option(MARSHMALLOW_WITH_SQLITE "Enable SQLite based features" OFF)
if(MARSHMALLOW_WITH_SQLITE)
	option(MARSHMALLOW_CONTRIB_SQLITE "Use bundled SQLite3 library" ON)
	if(MARSHMALLOW_CONTRIB_SQLITE)
		if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3/code/sqlite3.h)
			message(FATAL_ERROR "\n** sqlite3 submodule is unavailable **\n"
			                    "Please run the following command: \n"
			                    "  git submodule update --init contrib/sqlite3"
			)
		endif()

		unset(SQLITE_INCLUDE_DIR CACHE)
		set(SQLITE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3/code)
		unset(SQLITE_LIBRARY CACHE)
		set(SQLITE_LIBRARY marshmallow_sqlite3)
		set(SQLITE_FOUND TRUE)

		add_subdirectory(sqlite3)

		set(SQLITE_INCLUDE_DIR ${SQLITE_INCLUDE_DIR} PARENT_SCOPE)
		set(SQLITE_LIBRARY ${SQLITE_LIBRARY} PARENT_SCOPE)
	else()
		find_package(SQLite3 REQUIRED)
	endif()

	message(STATUS "SQLite3 library:\n"
		       "	${SQLITE_LIBRARY}"
	)
	set(SQLITE_FOUND ${SQLITE_FOUND} PARENT_SCOPE)
endif()

###############################################################################
# Lua Programming Language (OPTIONAL)
option(MARSHMALLOW_WITH_LUA "Use Lua Programming Language" OFF)
if(MARSHMALLOW_WITH_LUA)
	option(MARSHMALLOW_CONTRIB_LUA "Use bundled Lua library" ON)
	if (MARSHMALLOW_CONTRIB_LUA)
		if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lua/code/src)
			message(FATAL_ERROR "\n** lua submodule is unavailable **\n"
			                    "Please run the following command: \n"
			                    "  git submodule update --init contrib/lua"
			)
		endif()

		unset(LUA_INCLUDE_DIR CACHE)
		unset(LUA_LIBRARY CACHE)

		set(LUA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lua/code/src
		                    ${CMAKE_CURRENT_SOURCE_DIR}/lua/code/extra
		)
		set(LUA_LIBRARY marshmallow_lua)
		set(LUA_FOUND TRUE)

		add_subdirectory(lua)

		set(LUA_INCLUDE_DIR ${LUA_INCLUDE_DIR} PARENT_SCOPE)
		set(LUA_LIBRARY ${LUA_LIBRARY} PARENT_SCOPE)
	else()
		find_package(Lua51 REQUIRED)
	endif()

	message(STATUS "Lua programming language library:\n"
		       "	${LUA_LIBRARY}"
	)
	set(LUA_FOUND ${LUA_FOUND} PARENT_SCOPE)
endif()

