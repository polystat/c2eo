cmake_minimum_required(VERSION 3.18)

project(c2eo)

# llvm_config
set(PATH_TO_LLVM_SOURCE "../../../llvm-clang")

set(PATH_TO_LLVM_BUILD "${PATH_TO_LLVM_SOURCE}/build")
include_directories("${PATH_TO_LLVM_SOURCE}/clang/include")
include_directories("${PATH_TO_LLVM_SOURCE}/llvm/include")
include_directories("${PATH_TO_LLVM_BUILD}/tools/clang/include")
include_directories("${PATH_TO_LLVM_BUILD}/include")
link_directories("${PATH_TO_LLVM_BUILD}/lib/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

# Source files list
set(SOURCE_FILES
        # Headers
        vardecl.h
        analyzers.h
        matchers.h
        util.h
        memory_manager.h
        unit_transpiler.h
        eo_object.h
        function_manager.h
        transpile_helper.h
        recorddecl.h
        record_manager.h
        aliases.h
        tracer.h
        process_variables.h
        enumdecl.h
        enum_manager.h

        # Sources
        main.cpp
        vardecl.cpp
        analyzers.cpp
        matchers.cpp
        util.cpp
        memory_manager.cpp
        unit_transpiler.cpp
        eo_object.cpp
        function_manager.cpp
        transpile_helper.cpp
        recorddecl.cpp
        record_manager.cpp
        tracer.cpp
        process_variables.cpp
        enumdecl.cpp
        enum_manager.cpp)

# add_executable(recvisitor main.cpp)
add_executable(c2eo ${SOURCE_FILES})

# llvm libraries
target_link_libraries(c2eo
        clangAST
        clangASTMatchers
        clangBasic
        clangFrontend
        clangSerialization
        clangTooling
        LLVMSupport
        LLVMFrontendOpenMP)

set(LLVM_LINK_COMPONENTS
        Support)
