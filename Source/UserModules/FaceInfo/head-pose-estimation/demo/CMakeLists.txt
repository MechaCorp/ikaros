cmake_minimum_required(VERSION 2.6)

SET( 	HEAD_DEMO
		../CRForestEstimator.cpp
		../CRTree.cpp
		gl_camera.cpp
		main.cpp 
)

SET(CMAKE_BUILD_TYPE "Release")

IF(WIN32)

	SET( OPENCV_INCLUDES E:\\Documents\\Code\\libraries\\OpenCV-2.3.0\\include ) 
	SET( OPENCV_LIB E:\\Documents\\Code\\libraries\\OpenCV-2.3.0\\lib ) 

	SET( FREENECT_INCLUDES /home/matthias/libfreenect/include ) 
	SET( FREENECT_LIB /home/matthias/libfreenect/build/lib )
	
	SET( OPENNI_INCLUDES E:\\Documents\\Code\\libraries\\OpenNI\\Include ) 
	SET( OPENNI_LIB E:\\Documents\\Code\\libraries\\OpenNI\\Lib ) 

	SET( FREEGLUT_LIB E:\\Documents\\Code\\libraries\\freeglut\\lib ) 
	LINK_DIRECTORIES( ${OPENNI_LIB} ${OPENCV_LIB} ${FREEGLUT_LIB} )


ELSE(WIN32)

	SET( OPENCV_INCLUDES /usr/local/Cellar/opencv/2.4.9/include ) 
	SET( OPENCV_LIB /usr/local/Cellar/opencv/2.4.9/lib )
	
	SET( FREENECT_INCLUDES /usr/local/Cellar/libfreenect/0.5.0/include ) 
	SET( FREENECT_LIB /usr/local/Cellar/libfreenect/0.5.0/lib )
	
	SET( OPENNI_INCLUDES /usr/local/Cellar/openni/1.5.7.10/include ) 
	SET( OPENNI_LIB /usr/local/Cellar/openni/1.5.7/.10/lib ) 
	
	SET( LIBLO_INCLUDES /usr/local/Cellar/liblo/0.28/include ) 
	SET( LIBLO_LIB /usr/local/Cellar/liblo/0.28/lib ) 

	#assuming freeglut installed on linux
	LINK_DIRECTORIES( ${OPENNI_LIB} ${OPENCV_LIB} ${LIBLO_LIB} ${FREENECT_LIB} )

ENDIF(WIN32)
	
INCLUDE_DIRECTORIES( . .. ${OPENNI_INCLUDES} ${OPENCV_INCLUDES} ${LIBLO_INCLUDES} ${FREENECT_INCLUDES} )

	
add_executable( ../head_pose_estimation_demo ${HEAD_DEMO})


IF(WIN32)
target_link_libraries (../head_pose_estimation_demo opencv_core opencv_highgui opencv_imgproc freenect freeglut lo)
ELSE(WIN32)
target_link_libraries (../head_pose_estimation_demo opencv_core opencv_highgui opencv_imgproc freenect GLU glut lo)
ENDIF(WIN32)

