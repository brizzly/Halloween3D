cmake_minimum_required(VERSION 3.4)
project(halloween)
set(OpenGL_GL_PREFERENCE GLVND)


find_package(PkgConfig REQUIRED)

function(configure_deps ARG_TARGET)
    set(ARG_DEPS ${ARGN})
    foreach (DEP IN LISTS ARG_DEPS)
        pkg_check_modules(${DEP} REQUIRED ${DEP})
        if (NOT ${DEP}_FOUND)
            message(FATAL_ERROR "Could not configure target ${ARG_TARGET} as ${DEP} was not found.")
        endif ()

        # set link directory, only required by apple with his weird library directories
        target_link_directories(${ARG_TARGET} PRIVATE ${${DEP}_LIBRARY_DIRS})
        # add the necessary includes
        target_include_directories(${ARG_TARGET} PRIVATE ${${DEP}_INCLUDE_DIRS})
        # link to the libraries
        target_link_libraries(${ARG_TARGET} ${${DEP}_LIBRARIES})
    endforeach (DEP)
endfunction(configure_deps)


add_executable(halloween
        bspfile.cpp
        bsptree.cpp
        camera.cpp
        collision.cpp
#        collision2.cpp
        console.cpp
        demo.cpp
        display.cpp
        ellipsoide.cpp
        enveffect.cpp
        filesys.cpp
        frustrum.cpp
        game.cpp
        gameover.cpp
        halloween_linux.cpp
        image.cpp
        input.cpp
        interface.cpp
        jpeg.cpp
#        libsound_linux.cpp
        liste.cpp
        maths.cpp
        menu.cpp
        menu_init.cpp
        menu_sel.cpp
        monster.cpp
        mskmodel.cpp
        net_temp_linux.cpp
        objet.cpp
        objet_proc.cpp
        opengl.cpp
        physic.cpp
        player.cpp
        script.cpp
        shader.cpp
        sorting.cpp
        story.cpp
        textdraw.cpp
        tga.cpp
        timer.cpp
        dsound.cpp
        libbass.cpp
#       net_server.cpp  net_dcomp.cpp   net_client.cpp  net_common.cpp
)


configure_deps(halloween gl sdl)

target_link_directories(halloween PRIVATE ${CMAKE_SOURCE_DIR})  # for libbass.so

#install(TARGETS halloween RUNTIME DESTINATION bin)
target_link_libraries(halloween bass)
#target_link_libraries(halloween bass coldet)
set_target_properties(halloween PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wno-write-strings -Wno-unused-parameter")
#target_compile_options(halloween PRIVATE "-fsanitize=undefined")
#target_link_options(halloween PRIVATE "-fsanitize=undefined")
#target_compile_definitions(halloween PRIVATE H_LINUX)
