cmake_minimum_required(VERSION 2.6)

SET( 	HEAD_DEMO
	CRForestEstimator.cpp
	CRTree.cpp
	main.cpp
)

SET(CMAKE_BUILD_TYPE "Release")

#modify according to your opencv installation

SET( OPENCV_INCLUDES /usr/local/Cellar/opencv/2.4.9/include/opencv2 )
SET( OPENCV_LIB /usr/local/Cellar/opencv/2.4.9/lib )


INCLUDE_DIRECTORIES( . ${OPENCV_INCLUDES} )
LINK_DIRECTORIES( ${OPENCV_LIB} )

add_executable(head_pose_estimation ${HEAD_DEMO})

target_link_libraries (head_pose_estimation libopencv_core241 libopencv_highgui241 libopencv_imgproc241 )




