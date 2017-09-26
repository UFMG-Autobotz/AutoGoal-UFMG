#include <sstream>
#include <string>
#include <iostream>
//#include <opencv\highgui.h>
//#include <opencv\cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "robot.hpp"
#include <vector>

using namespace cv;


void on_trackbar( int, void* );
string intToString(int number);
void createTrackbars();
void drawObject(vector<robot> therobots,Mat &frame);
void morphOps(Mat &thresh);
void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed);
