# compile the bison file into the output
# Assuming using https://github.com/lexxmark/winflexbison
if (MSVC)
    flex_target(lexer chocopy.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c COMPILE_FLAGS "--wincompat")
else ()
    flex_target(lexer chocopy.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
endif ()
bison_target(parser chocopy.y ${CMAKE_CURRENT_BINARY_DIR}/chocopy.tab.c COMPILE_FLAGS "-v -t -d")


ADD_FLEX_BISON_DEPENDENCY(lexer parser)

SET(SOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c ${CMAKE_CURRENT_BINARY_DIR}/chocopy.tab.c chocopy_parse.cpp)
SET_SOURCE_FILES_PROPERTIES(${SOURCE_FILES} PROPERTIES LANGUAGE CXX)

add_library(parser-lib ${SOURCE_FILES} chocopy_ast.cpp chocopy_logging.cpp)
target_include_directories(parser-lib PRIVATE ${PROJECT_SOURCE_DIR}/include/semantic/)
target_include_directories(parser-lib PRIVATE ${PROJECT_SOURCE_DIR}/include/parser/)
target_include_directories(parser-lib PRIVATE ${PROJECT_BINARY_DIR}/src/parser/)
target_include_directories(parser-lib PRIVATE ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(parser-lib fmt::fmt cjson)

add_executable(parser ${SOURCE_FILES} chocopy_ast.cpp chocopy_logging.cpp ../semantic/chocopy_semant.cpp)
target_include_directories(parser PRIVATE ${PROJECT_SOURCE_DIR}/include/semantic/)
target_include_directories(parser PRIVATE ${PROJECT_SOURCE_DIR}/include/parser/)
target_include_directories(parser PRIVATE ${PROJECT_BINARY_DIR}/src/parser/)
target_include_directories(parser PRIVATE ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(parser fmt::fmt cjson)
target_compile_definitions(parser PUBLIC -DPA1=1)
