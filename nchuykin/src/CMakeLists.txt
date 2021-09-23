cmake_minimum_required(VERSION 3.18)

project(c2eo)


set(PATH_TO_LLVM_SOURCE "~/llvm-clang/")
set(PATH_TO_LLVM_BUILD "~/llvm-clang/build")
 
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
    analyzers.h
    matchers.h
    
    # Sources
    main.cpp
    vardecl.cpp
    analyzers.cpp
    matchers.cpp
        codeGenerator.h codeGenerator.cpp)


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
