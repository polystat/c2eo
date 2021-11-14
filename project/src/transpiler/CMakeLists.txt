cmake_minimum_required(VERSION 3.18)

# set(CMAKE_C_COMPILER путь/до/компилятора)
#set(CMAKE_CXX_COMPILER "${PATH_TO_LLVM_SOURCE}/build/bin/clang")

#  llvm_config
set(PATH_TO_LLVM_SOURCE "~/llvm-clang/" CACHE PATH [FORCE])
set(PATH_TO_LLVM_BUILD "${PATH_TO_LLVM_SOURCE}/build")

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
    recorddecl.h
    stmt.h
    analyzers.h
    matchers.h
    generator.h


    util.h

    # Sources
    main.cpp
    vardecl.cpp
    funcdecl.cpp
    recorddecl.cpp
    stmt.cpp
    analyzers.cpp
    matchers.cpp
    generator.cpp


    util.cpp)

#add_executable(recvistor main.cpp)
add_executable(c2eo ${SOURCE_FILES})

#  llvm libraries
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

