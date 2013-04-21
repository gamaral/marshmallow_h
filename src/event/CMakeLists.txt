add_definitions(-DMARSHMALLOW_EVENT_LIBRARY)

file(GLOB MARSHMALLOW_EVENT_SRCS "*.cpp")
set(MARSHMALLOW_EVENT_LIBS "marshmallow_core")

add_library(marshmallow_event ${MARSHMALLOW_EVENT_SRCS})

target_link_libraries(marshmallow_event ${MARSHMALLOW_EVENT_LIBS})

install(TARGETS marshmallow_event
        LIBRARY DESTINATION lib COMPONENT runtime
        ARCHIVE DESTINATION lib COMPONENT development)

