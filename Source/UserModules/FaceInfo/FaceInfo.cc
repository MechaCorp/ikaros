
#include "FaceInfo.h"
#include "/usr/local/Cellar/libfreenect/0.5.0/include/libfreenect/libfreenect.h"
#include <pthread.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stdint.h>
#include "HeadPose/CRForestEstimator.h"

using namespace ikaros;
using namespace std;
using namespace cv;

//pointer to the actual estimator
  CRForestEstimator* g_Estimate;
//input 3D image
  Mat g_im3D;
//maximum distance form the sensor - used to segment the person
  int g_max_z = 1300;
//head threshold - to classify a cluster of votes as a head
  int g_th = 400;
//threshold for the probability of a patch to belong to a head
  float g_prob_th = 1.0f;
//threshold on the variance of the leaves
  float g_maxv = 400.f;
//stride (how densely to sample test patches - increase for higher speed)
  int g_stride = 8;
//radius used for clustering votes into possible heads
  float g_larger_radius_ratio = 1.f;
//radius used for mean shift
  float g_smaller_radius_ratio = 5.f;
// Path to trees
  string g_treepath = "/Users/tobbe/Documents/Robot/ikaros/Source/UserModules/FaceInfo/HeadPose/trees/new_";
// Number of trees
  int g_ntrees = 10;
// Frame nr
  int g_frame_no = 0;
// Input image size
  int g_im_w = 640;
  int g_im_h = 480;
// Kinect's frame rate
  int g_fps = 30;

// Thread stuff
  pthread_t freenect_thread;
  pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;

// Depth stuff
  uint16_t *depth_mid, *depth_front;
  int got_depth = 0;

// Forest vector stuff
  std::vector< cv::Vec<float,POSE_SIZE> > g_means; //outputs
  std::vector< std::vector< const Vote* > > g_clusters; //full clusters of votes
  std::vector< Vote > g_votes; //all votes returned by the forest

void FaceInfo::Init() {
  FACES_SIZE_X  = GetInputSizeX("FACES");
  FACES_SIZE_Y  = GetInputSizeY("FACES");
  FACES         = GetInputMatrix("FACES");

  DEPTH         = GetInputMatrix("DEPTH");
  DEPTH_SIZE_X  = GetInputSizeX("DEPTH");
  DEPTH_SIZE_Y  = GetInputSizeY("DEPTH");

  TARGET_POSITION        = GetOutputArray("TARGET_POSITION");
  TARGET_POSITION_SIZE   = GetOutputSize("TARGET_POSITION");

  g_im3D.create(g_im_h, g_im_w, CV_32FC3);

  depth_mid   = (uint16_t*)malloc(640*480*2);
  depth_front = (uint16_t*)malloc(640*480*2);

  g_Estimate =  new CRForestEstimator();

  if( !g_Estimate->load_forest(g_treepath.c_str(), g_ntrees) ){
    cerr << "could not read forest!" << endl;
    exit(-1);
  }
}

FaceInfo::~FaceInfo() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

float horizontal_viewing_angle = 57.0;
float vertical_viewing_angle = 43.0;

bool read_data(){

  uint16_t *tmp;

  if (got_depth) {
    tmp = depth_front;
    depth_front = depth_mid;
    depth_mid = tmp;
    got_depth = 0;
  }
  else {
    return false;
  }

  return true;
}

void FaceInfo::Tick(){

  int i = 0;

  pthread_mutex_lock(&gl_backbuf_mutex);

  for (int y = 0; y < DEPTH_SIZE_Y; y++) {
    for (int x = 0; x < DEPTH_SIZE_X; x++) {
      depth_mid[i] = DEPTH[y][x];
      i++;
    }
  }

  got_depth++;

  pthread_cond_signal(&gl_frame_cond);
  pthread_mutex_unlock(&gl_backbuf_mutex);

  if( read_data() ){
    printf("Read data\n");

    g_means.clear();
    g_votes.clear();
    g_clusters.clear();

    g_Estimate->estimate(g_im3D,
                         g_means,
                         g_clusters,
                         g_votes,
                         g_stride,
                         g_maxv,
                         g_prob_th,
                         g_larger_radius_ratio,
                         g_smaller_radius_ratio,
                         false,
                         g_th
    );

    printf("%lu ", g_means.size());

    if( g_means.size() > 0 ){
      cout << "Estimated: " << g_means[0][0] << " " << g_means[0][1] << " " << g_means[0][2] << " " << g_means[0][3] << " " << g_means[0][4] << " " << g_means[0][5] <<endl;

      cout << g_means.size() << " " << flush;
      for(uint v=0;v<6;v++) {
        cout << g_means[0][v] << " ";
      }
      cout << endl;
    }

  }


  if(FACES[0][0] > 0) {
    TARGET_POSITION[0] = (180.0 + horizontal_viewing_angle/2.0) - (FACES[0][0] * horizontal_viewing_angle);
  }

  if(FACES[0][1] > 0) {
    TARGET_POSITION[1] = (FACES[0][1] * vertical_viewing_angle) - (225.0 + vertical_viewing_angle/2.0) * -1.00f;
  }

}

static InitClass init("FaceInfo", &FaceInfo::Create, "Source/UserModules/FaceInfo/");