cmake_minimum_required(VERSION 3.20)

project(c2eo)

set(PATH_TO_LLVM_SOURCE "~/llvm-clang/" CACHE PATH [FORCE])
set(PATH_TO_LLVM_BUILD "${PATH_TO_LLVM_SOURCE}/build")

# for tests to default output all variables
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

include_directories("${PATH_TO_LLVM_SOURCE}/clang/include")
include_directories("${PATH_TO_LLVM_SOURCE}/llvm/include")

include_directories("${PATH_TO_LLVM_BUILD}/tools/clang/include")
include_directories("${PATH_TO_LLVM_BUILD}/include")

link_directories("${PATH_TO_LLVM_BUILD}/lib/")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-rtti)

# Source files list
set(SOURCE_FILES
    # Headers
    vardecl.h
    funcdecl.h
    analyzers.h
    matchers.h
    generator.h
    util.h

    # Sources
    main.cpp
    vardecl.cpp
    funcdecl.cpp
    analyzers.cpp
    matchers.cpp
    generator.cpp
    util.cpp
)


#add_executable(recvistor main.cpp)
add_executable(c2eo ${SOURCE_FILES})


target_link_libraries(c2eo
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  LLVMSupport
  LLVMFrontendOpenMP
  )

set(LLVM_LINK_COMPONENTS
  Support
  )

#  llvm_config
