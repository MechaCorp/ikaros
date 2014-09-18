# Install script for directory: /Users/Tim/Desktop/Ikaros/ikaros/Source/Modules/VisionModules/ImageOperators

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/ChangeDetector/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/CurvatureDetectors/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/EdgeDetectors/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/GaborFilter/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/GaussianFilter/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/DoGFilter/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/LoGFilter/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/MedianFilter/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/ZeroCrossings/cmake_install.cmake")
  include("/Users/Tim/Desktop/Ikaros/ikaros/Build/Source/Modules/VisionModules/ImageOperators/MorphologicalOperators/cmake_install.cmake")

endif()

