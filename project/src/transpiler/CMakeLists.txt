cmake_minimum_required(VERSION 3.18)

project(c2eo)

# llvm_config
set(PATH_TO_LLVM_SOURCE "../../../llvm-clang")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PATH_TO_LLVM_SOURCE}")
    set(PATH_TO_LLVM_BUILD "${PATH_TO_LLVM_SOURCE}/build")
    include_directories("${PATH_TO_LLVM_SOURCE}/clang/include")
    include_directories("${PATH_TO_LLVM_SOURCE}/llvm/include")
    include_directories("${PATH_TO_LLVM_BUILD}/tools/clang/include")
    include_directories("${PATH_TO_LLVM_BUILD}/include")
    link_directories("${PATH_TO_LLVM_BUILD}/lib/")
else()
    find_package(Clang REQUIRED CONFIG
            HINTS
            /opt/homebrew/opt/llvm@16
            /opt/homebrew/opt/llvm
            /usr/local/opt/llvm@16
            /usr/local/opt/llvm
            /usr/lib/llvm-16)
    include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
    message(STATUS "Using installed LLVM/Clang ${LLVM_PACKAGE_VERSION} from ${LLVM_DIR}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_BUILD_TYPE Debug)
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
        type_manager.h


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
        enum_manager.cpp
        type_manager.cpp)

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
