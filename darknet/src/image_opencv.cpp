#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    if(im.c == 3) rgbgr_image(copy);

    Mat m(copy.h, copy.w, CV_8UC(copy.c));
    for(int y = 0; y < copy.h; ++y){
        unsigned char *row = m.ptr<unsigned char>(y);
        for(int x = 0; x < copy.w; ++x){
            for(int c = 0; c < copy.c; ++c){
                int dst_index = x*copy.c + c;
                int src_index = c*copy.h*copy.w + y*copy.w + x;
                float val = copy.data[src_index];
                row[dst_index] = (unsigned char)(val*255);
            }
        }
    }
    free_image(copy);
    return m;
}

image mat_to_image(Mat m)
{
    Mat src;
    if(m.channels() == 3){
        cvtColor(m, src, COLOR_BGR2RGB);
    } else {
        src = m;
    }

    int h = src.rows;
    int w = src.cols;
    int c = src.channels();
    image im = make_image(w, h, c);
    for(int y = 0; y < h; ++y){
        const unsigned char *row = src.ptr<unsigned char>(y);
        for(int x = 0; x < w; ++x){
            for(int k = 0; k < c; ++k){
                int dst_index = k*w*h + y*w + x;
                int src_index = x*c + k;
                im.data[dst_index] = row[src_index]/255.;
            }
        }
    }
    return im;
}

void *open_video_stream(const char *f, int c, int w, int h, int fps)
{
    VideoCapture *cap;
    if(f) cap = new VideoCapture(f);
    else cap = new VideoCapture(c);
    if(!cap->isOpened()) return 0;
    if(w) cap->set(cv::CAP_PROP_FRAME_WIDTH, w);
    if(h) cap->set(cv::CAP_PROP_FRAME_HEIGHT, h);
    if(fps) cap->set(cv::CAP_PROP_FPS, fps);
    return (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    if(m.empty()) return make_empty_image(0,0,0);
    return mat_to_image(m);
}

image load_image_cv(char *filename, int channels)
{
    int flag = -1;
    if (channels == 0) flag = -1;
    else if (channels == 1) flag = 0;
    else if (channels == 3) flag = 1;
    else {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    if(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        return make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    return im;
}

int show_image_cv(image im, const char* name, int ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    int c = waitKey(ms);
    if (c != -1) c = c%256;
    return c;
}

void make_window(char *name, int w, int h, int fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    if (fullscreen) {
        setWindowProperty(name, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    } else {
        resizeWindow(name, w, h);
        if(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
