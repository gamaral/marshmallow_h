add_executable(core_demo0 "demo0.cpp")
add_executable(core_demo1 "demo1.cpp")

target_link_libraries(core_demo0 marshmallow_core marshmallow_events)
target_link_libraries(core_demo1 marshmallow_core marshmallow_events)

