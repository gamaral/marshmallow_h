add_subdirectory(eastl)

if (MARSHMALLOW_CONTRIB_ZLIB AND ZLIB_FOUND)
	add_subdirectory(zlib)
endif()

if(MARSHMALLOW_CONTRIB_LIBPNG AND PNG_FOUND)
	add_subdirectory(libpng)
endif()

if(OPENGL_FOUND)
	add_subdirectory(glpng)
endif()

