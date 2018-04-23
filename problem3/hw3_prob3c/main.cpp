#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <math.h>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readme();
int* calculate_l2_distance(Mat descriptor, Mat vocabulary) {
    double *euclideanDist = new double[vocabulary.rows];
    double currentDist = 0.0;
    double minDist = 0.0;
    int index = 0;
    int vocabRows = vocabulary.rows;

    int *frequency = new int[vocabRows];
    for (int i = 0; i < vocabRows;i++) {
        frequency[i] = 0;
    }

    for (int i = 0; i < descriptor.rows; i++) {
        for (int k = 0; k < vocabulary.rows;k++) {
            for (int j = 0; j < descriptor.cols; j++) {
                double intDiff = (descriptor.at<float>(i,j)-vocabulary.at<float>(k,j));
                currentDist += pow(intDiff , 2);
            }
            euclideanDist[k] = (sqrt(currentDist));
            currentDist = 0.0;
        }
        minDist = euclideanDist[0]; index = 0;

        for (int k = 1; k<vocabulary.rows; k++) {
            if (minDist>euclideanDist[k]) {
                minDist = euclideanDist[k];
                index = k;
            }
        }
        frequency[index] += 1;
    }
    return frequency;
}

void writetoFile(string path,int *vocab_img,int numRows) {
    ofstream arrayData;
    arrayData.open(path);
    if (arrayData.is_open()) {
        for (int i = 0; i<numRows; i++) {
            arrayData << vocab_img[i];
            arrayData << "\n";
        }
        arrayData.close();
    }
    else
    {
        cout << "unable to open file" << endl;
    }
}

double compare_codewords(int *inputImage, int *targetImage) {
    double error = 0.0;
    for (int j = 0; j < 8;j++) {
        error += abs(inputImage[j]-targetImage[j]);
    }
    error =error/8.0;
    return error;
}

int main( int argc, char** argv )
{
    if( argc != 5 )
    { readme();
        return -1; }

    //Read the sample images and the target images into matrices for future processing

    Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_COLOR );//Ferrari_1
    Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_COLOR );//Optimus prime
    Mat img_3 = imread( argv[3], CV_LOAD_IMAGE_COLOR );//Bumblebee
    Mat img_4 = imread( argv[4], CV_LOAD_IMAGE_COLOR );//Ferrari_2

    if( !img_1.data || !img_2.data || !img_3.data || !img_4.data)
    {
        std::cout<< " Images not found. Check the path for the images " << std::endl;
        return -1;
    }

    //Set the minimum Hessian threshold value to get an optimal number of points to detect
    int minHessian = 400;

    // Create an object of the SIFT DETECTOR by passing the Hessian threshold value
    Ptr<SIFT> Siftdetector = SIFT::create( minHessian );

    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<KeyPoint> keypoints_3, keypoints_4;
    Mat descriptors_1, descriptors_2;
    Mat descriptors_3, descriptors_4;

    //Calculate the keypoints and descriptors for all the 4 images
    Siftdetector->detect(img_1, keypoints_1);
    Siftdetector->detect(img_2, keypoints_2);
    Siftdetector->detect(img_3, keypoints_3);
    Siftdetector->detect(img_4, keypoints_4);

    Siftdetector->compute(img_1, keypoints_1, descriptors_1);
    Siftdetector->compute(img_2, keypoints_2, descriptors_2);
    Siftdetector->compute(img_3, keypoints_3, descriptors_3);
    Siftdetector->compute(img_4, keypoints_4, descriptors_4);

    // Creating a training set of 8 clusters by passing the descriptors of the images which are to be compared against the target image
    BOWKMeansTrainer BGOWORDS(8);
    BGOWORDS.add(descriptors_1);
    BGOWORDS.add(descriptors_2);
    BGOWORDS.add(descriptors_3);

    // Create a vocabulary of words from the descriptors of the sample images and using the cluster() function.
    // This function generates the cluster centers or centroids for training
    Mat vocabulary = BGOWORDS.cluster();

    //in vocabulary each row will be a descriptor belonging to that image number

    // declare variables
    int *vocab_img1 = new int[vocabulary.rows];
    int *vocab_img2 = new int[vocabulary.rows];
    int *vocab_img3 = new int[vocabulary.rows];
    int *vocab_img4 = new int[vocabulary.rows];


    // Calculate the Euclidean distance between every descriptor of an image and the centroid
    // of each cluster of the vocabulary and assign labels to that feature descriptor
    vocab_img1 = calculate_l2_distance(descriptors_1, vocabulary);
    vocab_img2 = calculate_l2_distance(descriptors_2, vocabulary);
    vocab_img3 = calculate_l2_distance(descriptors_3, vocabulary);
    vocab_img4 = calculate_l2_distance(descriptors_4, vocabulary);

    // Get the frequency of how many feature descriptors or codewords belong to each cluster and write the values to a file
    writetoFile("histCount1.txt", vocab_img1, 8);
    writetoFile("histCount2.txt", vocab_img2, 8);
    writetoFile("histCount3.txt", vocab_img3, 8);
    writetoFile("histCount4.txt", vocab_img4, 8);


        //Now compare the number of codewords in every cluster in the target image with
        // those of the remaining images and calculate the error distance between them

    double error1 = compare_codewords(vocab_img1, vocab_img4);
    double error2 = compare_codewords(vocab_img2, vocab_img4);
    double error3 = compare_codewords(vocab_img3, vocab_img4);


    // Find the pair of images which have the least distance between them by comparing codeword count in each cluster.

    double min = error1;
    if(error1 < min)
        min = error1;
    else if(error2 < min)
        min = error2;
    else if(error3 < min)
        min = error3;


    // Confirm that the images with the minimum error are most similar compared to other pairs
    if(min == error1)
        cout << argv[4] << " is closest to " << argv[1] << endl;
    else if(min == error2)
        cout << argv[4] << " is closest to " << argv[2] << endl;
    else
        cout << argv[4] << " is closest to " << argv[3] << endl;
    //cout << error1 << endl;
    //cout << error2 << endl;
    //cout << error3 << endl;

    return 0 ;

}


void readme()
{ std::cout << " Usage: ./BagOfWords <sampleImage1> <sampleImage2> <sampleImage3> <targetImage>" << std::endl; }
