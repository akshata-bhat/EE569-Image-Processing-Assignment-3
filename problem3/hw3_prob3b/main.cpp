// Author : Akshata Bhat
// USC ID : 9560895350
// Email : akshatab@usc.edu
// Submission Date : 3/28/2018


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

int main( int argc, char** argv )
{
    if( argc != 3 )
    { readme(); return -1; }

    // Load the input images into matrices to perform further processing
    Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_COLOR );
    Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_COLOR );

    if( !img_1.data || !img_2.data )
    { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

    //Detect the keypoints using SIFT Detector by setting the minimum Hessian threshold value
    int minHessian = 400;

    printf("--------------------------------------------\n");
    printf("SIFT DETECTOR\n");

    /// Create an object of the SIFT DETECTOR
    Ptr<SIFT> detector1 = SIFT::create( minHessian );
    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;

    //Call the detectAndCompute() function to compute the keypoints and descriptors for both the images
    detector1->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
    detector1->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );

    // Create an instance of the Flann matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    // Use the matches() funcition to find all the keypoints which match in both the images
    matcher.match( descriptors_1, descriptors_2, matches );
    double max_dist = 0; double min_dist = 100;

    //Perform a calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist )
            min_dist = dist;
        if( dist > max_dist )
            max_dist = dist;
    }
    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );

    // Using the minimum and max distances filter out the best matching keypoints
    std::vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_1.rows; i++ )
    { if( matches[i].distance <= max(2*min_dist, 0.2) )
        { good_matches.push_back( matches[i]); }
    }

    //Draw only "good" matches using the drawMatches function
    Mat img_matches;
    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
                 good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Show detected matches
    imshow( "Good Matches_SIFT", img_matches );
    imwrite( "Good Matches_SIFT.jpg", img_matches );
    for( int i = 0; i < (int)good_matches.size(); i++ )
    { printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }


    /// SURF DETECTOR
    printf("--------------------------------------------\n");
    printf("SURF DETECTOR\n");
    Ptr<SURF> detector2 = SURF::create( minHessian );

    detector2->detect( img_1, keypoints_1 );
    detector2->detect( img_2, keypoints_2 );


    // IMAGE MATCHING

    max_dist = 0, min_dist = 100;
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist )
            min_dist = dist;
        if( dist > max_dist )
            max_dist = dist;
    }
    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );


    std::vector< DMatch > good_matches2;
    for( int i = 0; i < descriptors_1.rows; i++ )
    { if( matches[i].distance <= max(2*min_dist, 0.2) )
        { good_matches2.push_back( matches[i]); }
    }

    //-- Draw only "good" matches
    drawMatches( img_1, keypoints_1, img_2, keypoints_2,
                 good_matches2, img_matches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Show detected matches
    imshow( "Good Matches_SURF", img_matches );
    imwrite( "Good Matches_SURF.jpg", img_matches );
    for( int i = 0; i < (int)good_matches.size(); i++ )
    { printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }
    waitKey();
    return 0;
}

/** @function readme */
void readme()
{ std::cout << " Usage: ./image_matcher <img1> <img2>" << std::endl; }


