FROM gcc:latest as build

#ADD . /c2eo

WORKDIR /c2eo/llvm-project/build

RUN apt-get update && \
    apt-get install -y \
      libboost-dev libboost-program-options-dev \
      libgtest-dev \
      cmake ninja-build && \
# TODO: CMake 3.20 or higher is required.
    cmake --version && \
    cmake --no-warn-unused-cli -DBUILD_SHARED_LIBS:STRING=ON -DLLVM_TARGETS_TO_BUILD:STRING=X86 \
      -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE "-DLLVM_ENABLE_PROJECTS:STRING=clang;compiler-rt" \
      -DCMAKE_BUILD_TYPE:STRING=Debug -DLLVM_OPTIMIZED_TABLEGEN:STRING=ON \
      -DLLVM_USE_SPLIT_DWARF:STRING=ON -DLLVM_USE_LINKER:STRING=gold ../llvm -G Ninja && ls && \
      cmake --build . --config Debug --target all -j 10 -- -j1 -l 2

WORKDIR /c2eo/yar/build

RUN cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_DEPENDS_USE_COMPILER=FALSE -G "CodeBlocks - Unix Makefiles" .. && \
    cmake --build . --target c2eo -- -j 6

FROM ubuntu:latest

RUN groupadd -r sample && useradd -r -g sample sample
USER sample

WORKDIR /app

COPY --from=build /c2eo/yar/build/c2eo .

ENTRYPOINT ["./c2eo"]