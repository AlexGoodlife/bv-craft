include(FetchContent)
add_subdirectory(glad)
add_subdirectory(cutility)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)

# FetchContent_GetProperties(glfw)
# if(NOT glfw_POPULATED)
#     FetchContent_Populate(glfw)
#
#     set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
#     set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
#     set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
#     set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
#
#     add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
# endif()
FetchContent_MakeAvailable(glfw)
