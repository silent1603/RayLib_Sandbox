cmake_minimum_required(VERSION 3.25)
project(Game)

# The configurations we support
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/Vendors/raylib/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


set(BOX2D_BUILD_DOCS OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(Vendors/box2d)
add_subdirectory(Vendors/raylib)
add_subdirectory(Vendors/jolt/Build)


file(GLOB_RECURSE PROJECT_HEADERS Game/Src/*.hpp Game/Src/*.h)
file(GLOB_RECURSE PROJECT_SOURCES Game/Src/*.cpp 
                    Vendors/rlImGui/rlImGui.cpp 
                    Vendors/imgui/imgui.cpp 
                    Vendors/imgui/imgui_tables.cpp 
                    Vendors/imgui/imgui_widgets.cpp 
                    Vendors/imgui/imgui_draw.cpp 
                    Vendors/imgui/backends/imgui_impl_glfw.cpp 
                    Vendors/imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB_RECURSE PROJECT_SHADERS Game/Shaders/*.comp
                          Game/Shaders/*.frag
                          Game/Shaders/*.geom
                          Game/Shaders/*.vert 
                          )

file(GLOB_RECURSE PROJECT_RES Game/Res/*.*)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Resources" FILES ${PROJECT_RES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${PROJECT_RES})

target_include_directories(Game PUBLIC "${PROJECT_SOURCE_DIR}/Game/Sources/" 
                               PUBLIC "${PROJECT_SOURCE_DIR}/Vendors/Jolt/" 
                               PUBLIC "${PROJECT_SOURCE_DIR}/Vendors/raylib/" 
                               PUBLIC "${PROJECT_SOURCE_DIR}/Vendors/box2d/include/" 
                               PUBLIC "${PROJECT_SOURCE_DIR}/Vendors/rlImGui/" 
                               PUBLIC "${PROJECT_SOURCE_DIR}/Vendors/imgui/" 
                               PUBLIC "${PROJECT_SOURCE_DIR}/Vendors/imgui/backends/")

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
   
endif()
    


target_link_libraries(${PROJECT_NAME}  Jolt box2d raylib )
set_target_properties(${PROJECT_NAME}  Jolt box2d raylib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Game/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS} )

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Game/Res $<TARGET_FILE_DIR:${PROJECT_NAME}>
        DEPENDS ${PROJECT_RES} )
