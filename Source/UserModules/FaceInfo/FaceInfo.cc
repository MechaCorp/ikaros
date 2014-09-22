
#include "FaceInfo.h"

#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

#include "/usr/local/Cellar/libfreenect/0.5.0/include/libfreenect/libfreenect.h"
#include <pthread.h>

#include "CRForestEstimator.h"

using namespace ikaros;
using namespace std;
using namespace cv;

// Path to trees
  string g_treepath;
// Number of trees
  int g_ntrees;
// Patch width
  int g_p_width;
// Patch height
  int g_p_height;
//maximum distance form the sensor - used to segment the person
  int g_max_z = 0;
//head threshold - to classify a cluster of votes as a head
  int g_th = 300;
//threshold for the probability of a patch to belong to a head
  float g_prob_th = 1.0f;
//threshold on the variance of the leaves
  float g_maxv = 400.f;
//stride (how densely to sample test patches - increase for higher speed)
  int g_stride = 10;
//radius used for clustering votes into possible heads
  float g_larger_radius_ratio = 1.f;
//radius used for mean shift
  float g_smaller_radius_ratio = 5.f;
//
  int g_frame_no = 0;
//opengl window size
  int w,h;
//pointer to the actual estimator
  CRForestEstimator* g_Estimate;
//input 3D image
  Mat g_im3D, g_imD;
//input image size
  int g_im_w = 640;
  int g_im_h = 480;



// Freenect
  uint16_t *depth_mid, *depth_front;
  freenect_context *f_ctx;
  freenect_device *f_dev;
  int freenect_angle = 0;
  int freenect_led;

  volatile int die = 0;
  pthread_t freenect_thread;
  pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;
  int got_depth = 0;

// Output
  std::vector< cv::Vec<float,POSE_SIZE> > g_means; //outputs
  std::vector< std::vector< const Vote* > > g_clusters; //full clusters of votes
  std::vector< Vote > g_votes; //all votes returned by the forest

// Freenect methods
  void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp) {
    int i;
    uint16_t *depth = (uint16_t*)v_depth;

    pthread_mutex_lock(&gl_backbuf_mutex);
    for (i=0; i<640*480; i++) {
      depth_mid[i] = depth[i];
    }
    got_depth++;
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
  }

  void *freenect_threadfunc(void *arg) {
    // freenect_set_tilt_degs(f_dev,freenect_angle);
    freenect_set_led(f_dev,LED_RED);
    freenect_set_depth_callback(f_dev, depth_cb);

    freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));

    freenect_start_depth(f_dev);

    while (!die && freenect_process_events(f_ctx) >= 0) {}

    printf("\nshutting down streams...\n");

    freenect_stop_depth(f_dev);

    freenect_close_device(f_dev);
    freenect_shutdown(f_ctx);

    printf("-- done!\n");
    return NULL;
  }

  bool read_data( ){
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

  bool process() {
    if( read_data() ) {
      g_means.clear();
      g_votes.clear();
      g_clusters.clear();

      //do the actual estimation
      g_Estimate->estimate(   g_im3D,
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
      return true;
    }
    else {
      return false;
    }
  }

void FaceInfo::Init() {
  FACES_SIZE_X  = GetInputSizeX("FACES");
  FACES_SIZE_Y  = GetInputSizeY("FACES");
  FACES         = GetInputMatrix("FACES");

  TARGET_POSITION        = GetOutputArray("TARGET_POSITION");
  TARGET_POSITION_SIZE   = GetOutputSize("TARGET_POSITION");

  depth_mid = (uint16_t*)malloc(640*480*2);
  depth_front = (uint16_t*)malloc(640*480*2);

  if (freenect_init(&f_ctx, NULL) < 0) {
    printf("freenect_init() failed\n");
    exit(1);
  }

  freenect_set_log_level(f_ctx, FREENECT_LOG_ERROR);
  freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

  int nr_devices = freenect_num_devices (f_ctx);
  printf ("Number of devices found: %d\n", nr_devices);

  int user_device_number = 0;

  if (nr_devices < 1) {
    freenect_shutdown(f_ctx);
    exit(1);
  }

  if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
    printf("Could not open device\n");
    freenect_shutdown(f_ctx);
    exit(1);
  }

  bool res = pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL);

  if (res) {
    printf("pthread_create failed\n");
    freenect_shutdown(f_ctx);
    exit(1);
  }

  freenect_raw_tilt_state state;
  freenect_get_tilt_status(&state);

  g_im3D.create(g_im_h,g_im_w,CV_32FC3);
  g_imD.create(g_im_h,g_im_w,CV_16UC1);

  freenect_angle = freenect_get_tilt_degs(&state);
}

FaceInfo::~FaceInfo() {
    // Destroy data structures that you allocated in Init.
    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

float horizontal_viewing_angle = 57.0;
float vertical_viewing_angle = 43.0;

void FaceInfo::Tick(){

  if(FACES[0][0] > 0) {
    // printf("%f ", FACES[0][0]);
    // printf("%f\n", target_degrees);
    TARGET_POSITION[0] = (180.0 + horizontal_viewing_angle/2.0) - (FACES[0][0] * horizontal_viewing_angle);

  }
  if(FACES[0][1] > 0) {
    TARGET_POSITION[1] = (FACES[0][1] * vertical_viewing_angle) - (180.0 + vertical_viewing_angle/2.0);
  }

}

static InitClass init("FaceInfo", &FaceInfo::Create, "Source/UserModules/FaceInfo/");