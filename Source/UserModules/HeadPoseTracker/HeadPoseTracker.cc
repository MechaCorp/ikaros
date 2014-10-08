
#include "HeadPoseTracker.h"
#include "/usr/local/Cellar/libfreenect/0.5.0/include/libfreenect/libfreenect.h"
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
  int g_max_z = 2000;
//head threshold - to classify a cluster of votes as a head
  int g_th = 300;
//threshold for the probability of a patch to belong to a head
  float g_prob_th = 1.0f;
//threshold on the variance of the leaves
  float g_maxv = 500.f;
//stride (how densely to sample test patches - increase for higher speed)
  int g_stride = 20;
//radius used for clustering votes into possible heads
  float g_larger_radius_ratio = 1.f;
//radius used for mean shift
  float g_smaller_radius_ratio = 5.f;
// Path to trees
  string g_treepath = "/Users/tobbe/Documents/Robot/ikaros/Source/UserModules/HeadPoseTracker/HeadPose/trees/new_";
// Number of trees
  int g_ntrees = 10;
// Frame nr
  int g_frame_no = 0;
// Input image size
  int g_im_w = 640;
  int g_im_h = 480;
// Kinect's frame rate
  int g_fps = 30;

// Depth stuff
  uint16_t *depth_mid, *depth_front;
  int got_depth = 0;

// Forest vector stuff
  std::vector< cv::Vec<float,POSE_SIZE> > g_means; //outputs
  std::vector< std::vector< const Vote* > > g_clusters; //full clusters of votes
  std::vector< Vote > g_votes; //all votes returned by the forest

void HeadPoseTracker::Init() {
  DEPTH         = GetInputMatrix("DEPTH");
  DEPTH_SIZE_X  = GetInputSizeX("DEPTH");
  DEPTH_SIZE_Y  = GetInputSizeY("DEPTH");

  HEADS         = GetOutputMatrix("HEADS");
  HEADS_SIZE_X  = GetOutputSizeX("HEADS");
  HEADS_SIZE_Y  = GetOutputSizeY("HEADS");

  set_matrix(HEADS, 0, HEADS_SIZE_X, HEADS_SIZE_Y);

  g_im3D.create(g_im_h, g_im_w, CV_32FC3);

  depth_mid   = (uint16_t*)malloc(640*480*2);
  depth_front = (uint16_t*)malloc(640*480*2);

  g_Estimate =  new CRForestEstimator();

  if( !g_Estimate->load_forest(g_treepath.c_str(), g_ntrees) ){
    cerr << "could not read forest!" << endl;
    exit(-1);
  }
}

HeadPoseTracker::~HeadPoseTracker() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

float horizontal_viewing_angle = 57.0;
float vertical_viewing_angle = 43.0;

bool read_data(){
  uint16_t *tmp;
  int valid_pixels = 0;
  float d = 0.f;
  int ii = 0;

  if(got_depth) {
    tmp = depth_front;
    depth_front = depth_mid;
    depth_mid = tmp;
    got_depth = 0;
  }
  else {
    return false;
  }

  // Generate 3D image
  for(int y = 0; y < g_im3D.rows; y++) {
    Vec3f* Mi = g_im3D.ptr<Vec3f>(y);
    for(int x = 0; x < g_im3D.cols; x++){
      d = (float)depth_mid[ii++];
      if ( d < g_max_z && d > 0 ){
       valid_pixels++;
       Mi[x][0] = ( float(d * (x - 320)) * 0.0017505f );
       Mi[x][1] = ( float(d * (y - 240)) * 0.0017505f );
       Mi[x][2] = d;
      }
      else {
       Mi[x] = 0;
      }
    }
  }

  return true;
}

void HeadPoseTracker::Tick(){

  int i = 0;

  for (int y = 0; y < DEPTH_SIZE_Y; y++) {
    for (int x = 0; x < DEPTH_SIZE_X; x++) {
      depth_mid[i] = DEPTH[y][x];
      i++;
    }
  }

  got_depth++;

  if( read_data() ) {
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

    if( g_means.size() > 0 ){
      //cout << "1: Estimated: " << floor(g_means[0][0]) << "\t" << floor(g_means[0][1]) << "\t" << floor(g_means[0][2]) << "\t" << floor(g_means[0][3]) << "\t" << floor(g_means[0][4]) << "\t" << floor(g_means[0][5]) <<endl;
      //cout << "2: Estimated: " << floor(g_means[1][0]) << "\t" << floor(g_means[1][1]) << "\t" << floor(g_means[1][2]) << "\t" << floor(g_means[1][3]) << "\t" << floor(g_means[1][4]) << "\t" << floor(g_means[1][5]) <<endl;

      // XYZ
      HEADS[0][0] = g_means[0][0];
      HEADS[0][1] = g_means[0][1];
      HEADS[0][2] = g_means[0][2];

      // Pitch, yaw, roll
      HEADS[0][3] = g_means[0][3];
      HEADS[0][4] = g_means[0][4];
      HEADS[0][5] = g_means[0][5];

      // Distance
      HEADS[0][6] = std::sqrt( g_means[0][0]*g_means[0][0] + g_means[0][1]*g_means[0][1] + g_means[0][2]*g_means[0][2] );

      // HEAD 2

      // XYZ
      HEADS[1][0] = g_means[1][0];
      HEADS[1][1] = g_means[1][1];
      HEADS[1][2] = g_means[1][2];

      // Pitch, yaw, roll
      HEADS[1][3] = g_means[1][3];
      HEADS[1][4] = g_means[1][4];
      HEADS[1][5] = g_means[1][5];

      // Distance
      HEADS[1][6] = std::sqrt( g_means[1][0]*g_means[1][0] + g_means[1][1]*g_means[1][1] + g_means[1][2]*g_means[1][2] );

      //::copy_matrix(HEADS, g_means, HEADS_SIZE_X, HEADS_SIZE_Y);
    }

  }


}

static InitClass init("HeadPoseTracker", &HeadPoseTracker::Create, "Source/UserModules/HeadPoseTracker/");