set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Dynamixel)
add_subdirectory(Epuck)
add_subdirectory(SSC32)
add_subdirectory(Phidgets)

set(SOURCE ${SOURCE} PARENT_SCOPE)
set(MODULES_INCLUDS ${MODULES_INCLUDS} PARENT_SCOPE)
set(MODULES_LIBS ${MODULES_LIBS} PARENT_SCOPE)
