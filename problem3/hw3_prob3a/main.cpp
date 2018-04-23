#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

void readme();
//https://docs.opencv.org/3.0-beta/doc/tutorials/features2d/feature_detection/feature_detection.html
/** @function main */

int main( int argc, char** argv )
{
    if( argc != 3 )
    { readme(); return -1; }

    Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_COLOR );
    Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_COLOR );

    if( !img_1.data || !img_2.data )
    { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 600;

    /// SIFT DETECTOR
    Ptr<SIFT> detector = SIFT::create( minHessian );
    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;

    detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
    detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );

    //-- Draw keypoints
    Mat img_keypoints_1; Mat img_keypoints_2;

    drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //-- Show detected (drawn) keypoints
//    imshow("Keypoints_truck_SIFT_600", img_keypoints_1 );
//    imshow("Keypoints_car_SIFT_600", img_keypoints_2 );
//    imwrite("Keypoints_truck_SIFT_600.jpg", img_keypoints_1 );
//    imwrite("Keypoints_car_SIFT_600.jpg", img_keypoints_2 );

    imshow("Keypoints_Ferrari1_SIFT_600", img_keypoints_1 );
    imshow("Keypoints_Ferrari2_SIFT_600", img_keypoints_2 );
    imwrite("Keypoints_Ferrari1_SIFT_600.jpg", img_keypoints_1 );
    imwrite("Keypoints_Ferrari2_SIFT_600.jpg", img_keypoints_2 );

    /// SURF DETECTOR
    Ptr<SURF> detector1 = SURF::create( minHessian );

    detector1->detect( img_1, keypoints_1 );
    detector1->detect( img_2, keypoints_2 );

    drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //-- Show detected (drawn) keypoints
//    imshow("Keypoints_truck_SURF_600", img_keypoints_1 );
//    imwrite("Keypoints_truck_SURF_600.jpg", img_keypoints_1);
//    imshow("Keypoints_car_SURF_600", img_keypoints_2 );
//    imwrite("Keypoints_car_SURF_600.jpg", img_keypoints_2);
    imshow("Keypoints_Ferrari1_SURF_600", img_keypoints_1 );
    imwrite("Keypoints_Ferrari2_SURF_600.jpg", img_keypoints_1);
    imshow("Keypoints_Ferrari1_SURF_600", img_keypoints_2 );
    imwrite("Keypoints_Ferrari2_SURF_600.jpg", img_keypoints_2);

    waitKey();
    return 0;
}

/** @function readme */
void readme()
{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }

