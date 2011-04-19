add_subdirectory(eastl)

if (MARSHMALLOW_CONTRIB_ZLIB)
	add_subdirectory(zlib)
endif()

if(MARSHMALLOW_CONTRIB_LIBPNG)
	add_subdirectory(libpng)
endif()

if(OPENGL_FOUND)
	add_subdirectory(glpng)
endif()

