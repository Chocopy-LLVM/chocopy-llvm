include_directories(${PROJECT_SOURCE_DIR}/include/semantic)
include_directories(${PROJECT_SOURCE_DIR}/include/parser)

add_library(semantic-lib chocopy_semant.cpp)
target_link_libraries(semantic-lib cjson parser-lib fmt::fmt)

add_executable(semantic chocopy_semant.cpp)
target_link_libraries(semantic cjson parser-lib fmt::fmt)
target_compile_definitions(semantic PUBLIC -DPA2=1)