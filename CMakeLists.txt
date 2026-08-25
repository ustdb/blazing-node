cmake_minimum_required(VERSION 3.15)

project(blazing_node LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


message(STATUS
    "Node.js include: ${CMAKE_JS_INC}"
)


execute_process(
    COMMAND
        node
        -p
        "require('node-addon-api').include"
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE
        NODE_ADDON_API_INCLUDE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Remove surrounding quotes returned by node-addon-api.
string(
    REPLACE
        "\""
        ""
        NODE_ADDON_API_INCLUDE
        "${NODE_ADDON_API_INCLUDE}"
)

message(STATUS
    "node-addon-api include: ${NODE_ADDON_API_INCLUDE}"
)


add_library(${PROJECT_NAME} SHARED
    native/src/addon.cxx
    native/src/io/printf.cxx
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        PREFIX ""
        SUFFIX ".node"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME}
    SYSTEM PRIVATE
        ${CMAKE_JS_INC}
        ${NODE_ADDON_API_INCLUDE}
)


target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        NAPI_CPP_EXCEPTIONS
)

if(MSVC)

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /EHsc
            /W4
    )

else()

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra

            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wold-style-cast
            -Wcast-align
            -Wcast-qual
            -Wpointer-arith
            -Wwrite-strings

            -Wformat=2
            -Wundef
            -Wuninitialized
            -Wmaybe-uninitialized
            -Winit-self
            -Wswitch
            -Wswitch-enum
            -Wimplicit-fallthrough
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op

            -Wmissing-declarations
#            -Wmissing-prototypes
#            -Wstrict-prototypes
#            -Wold-style-definition

            -Wnon-virtual-dtor
            -Woverloaded-virtual
            -Wuseless-cast
            -Wzero-as-null-pointer-constant

            -Wpointer-arith
            -Warray-bounds
            -Wstringop-overflow
            -Wstringop-overread

            -Wdouble-promotion
            -Wfloat-equal
            -Wmissing-field-initializers
            -Wredundant-decls
            -Wdeprecated
            -Wdeprecated-declarations

            -Wexceptions
            -fexceptions
            -O2
    )

endif()
