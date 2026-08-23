cmake_minimum_required(VERSION 3.15)

project(blazing_node LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# Node.js headers
# --------------------------------------------------

message(STATUS
    "Node.js include: ${CMAKE_JS_INC}"
)

# --------------------------------------------------
# node-addon-api
# --------------------------------------------------

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

# --------------------------------------------------
# Native addon
# --------------------------------------------------

add_library(${PROJECT_NAME} SHARED
    native/src/addon.cxx
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        PREFIX ""
        SUFFIX ".node"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_JS_INC}
        ${NODE_ADDON_API_INCLUDE}
)

# --------------------------------------------------
# Node-API C++ exceptions
# --------------------------------------------------

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
            -fexceptions
    )

endif()
