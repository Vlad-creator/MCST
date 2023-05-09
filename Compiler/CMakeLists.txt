cmake_minimum_required(VERSION 3.13.4)
project(numlex)

set(PARSER_DIR parser)
set(SOURCE_DIR source)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

flex_target(scanner
  ${PARSER_DIR}/lex.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)

bison_target(parser
  ${PARSER_DIR}/grammar.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/numgrammar.tab.hh"
)

add_flex_bison_dependency(scanner parser)

add_executable(${PROJECT_NAME} 
  ${SOURCE_DIR}/driver.cpp
  ${SOURCE_DIR}/node.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/CodeGen.cpp 
  ${SOURCE_DIR}/NameTab.cpp
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)

llvm_map_components_to_libnames(llvm_libs support core codegen irreader)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${llvm_libs})