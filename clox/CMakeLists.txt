add_library(clox
            "./src/chunk.cpp"  "./headers/chunk.hpp"
            "./src/memory.cpp" "./headers/memory.hpp"
            "./src/debug.cpp"  "./headers/debug.hpp"
            "./src/value.cpp"  "./headers/value.hpp"
            "./src/vm.cpp"     "./headers/vm.hpp"

            "./headers/common.hpp"
)
