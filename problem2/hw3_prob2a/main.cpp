// Author : Akshata Bhat
// USC ID : 9560895350
// Email : akshatab@usc.edu
// Submission Date : 03/25/2018

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "my_header_file.h"
using namespace std;

unsigned char** generate_grayscale_image(unsigned char** ImageR, unsigned char** ImageG, unsigned char** ImageB, int width, int height, int bytes_per_pixel){
    unsigned char** grayImage = NULL;
    grayImage = create_2Darray(grayImage, width, height);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            grayImage[i][j]=(0.21 * ImageR[i][j]) + (0.72 * ImageG[i][j]) + (0.07 * ImageB[i][j]);
        }
    }
    return grayImage;
}

int calculate_gradient_per_pixel(int* sobel_mask, int* image_mask, int N){
    int result = 0;
    for (int i = 0; i < N*N; ++i) {
        result = result + sobel_mask[i]*image_mask[i];
    }
    return result;
}


int** generate_sobel_gradient(unsigned char** grayscaleImage, int* sobelMask, int width, int height, int N) {
    int **gradientImage = NULL;
    gradientImage = create_2Darray(gradientImage, width, height);
    int i, j, x, y, xStart = 0, yStart = 0, xLimit = 0, yLimit = 0, index = 0, row, col, bond = 0;
    int *imgNxN = NULL;
    imgNxN = new int[N * N];
    int hit_value;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            xStart = i - (N / 2);
            yStart = j - (N / 2);
            yLimit = j + (N / 2);
            xLimit = i + (N / 2);
            index = 0;
            memset(imgNxN, 0, N * N);
            for (x = xStart; x <= xLimit; ++x) {
                for (y = yStart; y <= yLimit; ++y) {
                    row = x;
                    col = y;
                    if (row < 0) row = 0;
                    if (col < 0) col = 0;
                    if (row > height - 1) row = height - 1;
                    if (col > width - 1) col = width - 1;
                    imgNxN[index] = int(grayscaleImage[row][col]);
                    //cout << imgNxN[index] << endl;
                    index++;
                }
            }
            gradientImage[i][j] = calculate_gradient_per_pixel(sobelMask, imgNxN, N);
        }
    }
    return gradientImage;
}

int** calculate_magnitude(int** xGradient, int** yGradient, int width, int height){
    int** imageMagnitude = NULL;
    imageMagnitude = create_2Darray(imageMagnitude, width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width ; ++j) {
            imageMagnitude[i][j] = int(sqrt(pow(xGradient[i][j], 2) + pow(yGradient[i][j], 2)));
        }
    }
    return imageMagnitude;
}

void sobel_edge_detector(char* inputFilename, char* grayImage, char* X_output, char* Y_output, char* outputFilename, int width, int height, int bytesPerPixel)
{
    int totalBytes = width* height* bytesPerPixel;
    unsigned char* Imagedata = NULL;
    Imagedata = new unsigned char[totalBytes];
    Imagedata = read_1Darray_from_file(width*bytesPerPixel, height, inputFilename);

    int i, j, count=0;
    unsigned char **Image2D_R = NULL;
    unsigned char **Image2D_G = NULL;
    unsigned char **Image2D_B = NULL;
    Image2D_R = create_2Darray(Image2D_R, width, height);
    Image2D_G = create_2Darray(Image2D_G, width, height);
    Image2D_B = create_2Darray(Image2D_B, width, height);


    while(count<totalBytes) {
        for(i=0;i<height;i++) {
            for(j=0;j<width;j++) {
                Image2D_R[i][j]=Imagedata[count];
                Image2D_G[i][j]=Imagedata[count+1];
                Image2D_B[i][j]=Imagedata[count+2];
                count=count+3;
            }
        }
    }

    int *Sobel_Gx = NULL;
    Sobel_Gx = new int[3 * 3]{-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int* Sobel_Gy = NULL;
    Sobel_Gy = new int[3 * 3] {1, 2, 1, 0, 0, 0, -1, -2, -1};

    unsigned char** grayscaleImage2D = NULL;
    grayscaleImage2D = generate_grayscale_image(Image2D_R, Image2D_G, Image2D_B, width, height, bytesPerPixel);
    unsigned char* grayout = NULL;
    grayout = convert_2Dto1D(grayscaleImage2D, width, height);
    write_1Darray_to_file(grayout, width, height, grayImage);
    int** ImageGradientX_2D = NULL;
    int** ImageGradientY_2D = NULL;
    int** imageMagnitude2D = NULL;
    ImageGradientX_2D = generate_sobel_gradient(grayscaleImage2D, Sobel_Gx, width, height, 3);
    ImageGradientY_2D = generate_sobel_gradient(grayscaleImage2D, Sobel_Gy, width, height, 3);

    //Writing the X and Y gradient outputs to files
    int* ImageGradientX_1D_int = convert_2Dto1D(ImageGradientX_2D, width, height);
    int* ImageGradientY_1D_int = convert_2Dto1D(ImageGradientY_2D, width, height);
    int* ImageGradientX_normalized = NULL;
    ImageGradientX_normalized = new int [width*height];
    int* ImageGradientY_normalized = NULL;
    ImageGradientY_normalized = new int [width*height];


    int min = ImageGradientX_1D_int[0];
    int max = ImageGradientX_1D_int[0];
    for (int l = 0; l < height*width; ++l) {
        if(ImageGradientX_1D_int[l] > max)
            max = ImageGradientX_1D_int[l];
        else if(ImageGradientX_1D_int[l] < min)
            min = ImageGradientX_1D_int[l];
    }
    for (int k = 0; k < height*width ; ++k) {
        ImageGradientX_normalized[k] = (255*(ImageGradientX_1D_int[k]-min)/(max-min));
    }

    min = ImageGradientY_1D_int[0];
    max = ImageGradientY_1D_int[0];
    for (int l = 0; l < height*width; ++l) {
        if(ImageGradientY_1D_int[l] > max)
            max = ImageGradientY_1D_int[l];
        else if(ImageGradientY_1D_int[l] < min)
            min = ImageGradientY_1D_int[l];
    }
    for (int k = 0; k < height*width ; ++k) {
        ImageGradientY_normalized[k] = (255*(ImageGradientY_1D_int[k]-min)/(max-min));
    }

    unsigned char* ImageGradientX_1D = convert_int_to_char_1D(ImageGradientX_normalized, width, height);
    unsigned char* ImageGradientY_1D = convert_int_to_char_1D(ImageGradientY_normalized, width, height);

    write_1Darray_to_file(ImageGradientX_1D, width, height, X_output);
    write_1Darray_to_file(ImageGradientY_1D, width, height, Y_output);

    //Calculating the magnitude of Gradient in X and Y direction
    imageMagnitude2D = calculate_magnitude(ImageGradientX_2D, ImageGradientY_2D, width, height);

    // Convert the magnitude array into 1D to calculate min, max values and the threshold
    int* imageMagnitude1D_sorted = NULL;
    imageMagnitude1D_sorted = convert_2Dto1D(imageMagnitude2D, width, height);

    //Sorting the magnitude array to get the max and min values
    imageMagnitude1D_sorted = sort_decreasing(imageMagnitude1D_sorted, width*height);


    max = imageMagnitude1D_sorted[0];
    min = imageMagnitude1D_sorted[height*width-1];

    //Normalization of the magnitude array
    int* imageMagnitude1D_normalized = NULL;
    imageMagnitude1D_normalized = new int[width*height];
    for (int k = 0; k < height*width ; ++k) {
        imageMagnitude1D_normalized[k] = (int)(255*(imageMagnitude1D_sorted[k]-min)/(max-min));
    }

    //Sorting the normalized array
    int* imageNormalized_sorted = NULL;
    imageNormalized_sorted = sort_decreasing(imageMagnitude1D_normalized, width*height);


    //Calculating the threshold value
    int threshold_location = (0.1 * width*height);
    int threshold_value = imageNormalized_sorted[threshold_location];

    // Normalizing the 2D magnitude array for the thresholding
    int** imageMagnitude2D_normalized = NULL;
    imageMagnitude2D_normalized = create_2Darray(imageMagnitude2D_normalized, width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width ; ++j) {
            imageMagnitude2D_normalized[i][j] = (255*(imageMagnitude2D[i][j]-min)/(max-min));
        }
    }

    //Thresholding the 2D normalized array
    unsigned char** imageMagnitude2D_thresholded = NULL;
    imageMagnitude2D_thresholded = create_2Darray(imageMagnitude2D_thresholded, width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(imageMagnitude2D_normalized[i][j] < threshold_value)
                imageMagnitude2D_thresholded[i][j] = (unsigned char)255;
            else
                imageMagnitude2D_thresholded[i][j] = (unsigned char)0;
        }
    }

    unsigned char* finalOutput = NULL;
    finalOutput = convert_2Dto1D(imageMagnitude2D_thresholded, width, height);

    write_1Darray_to_file(finalOutput, width, height, outputFilename);
}

void LOG_edge_detector(char* inputFilename, int width, int height, int bytesPerPixel, char* grayOutputFilename, char* outputFilename)
{
    int totalBytes = width* height* bytesPerPixel;
    unsigned char* Imagedata = NULL;
    Imagedata = read_1Darray_from_file(width*bytesPerPixel, height, inputFilename);

    int i, j, count=0;
    unsigned char **Image2D_R = NULL;
    unsigned char **Image2D_G = NULL;
    unsigned char **Image2D_B = NULL;
    Image2D_R = create_2Darray(Image2D_R, width, height);
    Image2D_G = create_2Darray(Image2D_G, width, height);
    Image2D_B = create_2Darray(Image2D_B, width, height);

    while(count<totalBytes) {
        for(i=0;i<height;i++) {
            for(j=0;j<width;j++) {
                Image2D_R[i][j]=Imagedata[count];
                Image2D_G[i][j]=Imagedata[count+1];
                Image2D_B[i][j]=Imagedata[count+2];
                count=count+3;
            }
        }
    }

    int *LOG = NULL;
    LOG = new int[9 * 9]{0, 1, 1, 2, 2, 2, 1, 1, 0,
                      1, 2, 4, 5, 5, 5, 4, 2, 1,
                      1, 4, 5, 3, 0, 3, 5, 4, 1,
                      2, 5, 3, -12, -24, -12, 3, 5, 2,
                      2, 5, 0, -24, -40, -24, 0, 5, 2,
                      2, 5, 3, -12, -24, -12, 3, 5, 2,
                      1, 4, 5, 3, 0, 3, 5, 4, 1,
                      1, 2, 4, 5, 5, 5, 4, 2, 1,
                      0, 1, 1, 2, 2, 2, 1, 1, 0};

    unsigned char** grayscaleImage2D = NULL;
    grayscaleImage2D = generate_grayscale_image(Image2D_R, Image2D_G, Image2D_B, width, height, bytesPerPixel);
    unsigned char* grayout = NULL;
    grayout = convert_2Dto1D(grayscaleImage2D, width, height);
    //write_1Darray_to_file(grayout, width, height, grayImage);
    int** ImageLOG_2D = NULL;
    ImageLOG_2D = generate_sobel_gradient(grayscaleImage2D, LOG, width, height, 9);

    /*int* intensity_counter = NULL;
    intensity_counter = new int[256];
    memset(intensity_counter, 0, 256* sizeof(int));
    for (int l = 0; l < height; ++l) {
        for (int k = 0; k < width; ++k) {
                intensity_counter[ImageLOG_2D[i][j]] +=1;
        }
    }*/

    // Convert the magnitude array into 1D to calculate min, max values for normalization

    //Sorting the magnitude array to get the max and min values
    //int* imageLOG_sorted_1D = NULL;
    //imageLOG_sorted_1D = sort_decreasing(ImageLOG_2D, width*height);
    double min = ImageLOG_2D[0][0];
    double max = ImageLOG_2D[0][0];
    for (int l = 0; l < height; ++l) {
        for (int k = 0; k < width; ++k) {
            if(ImageLOG_2D[l][k] > max)
                max = ImageLOG_2D[l][k];
            else if(ImageLOG_2D[l][k] < min)
                min = ImageLOG_2D[l][k];
        }
    }


    // Normalizing the 2D magnitude array for the thresholding
    int** image2D_normalized = NULL;
    image2D_normalized = create_2Darray(image2D_normalized, width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width ; ++j) {
            image2D_normalized[i][j] = (int)(255.0*((double)ImageLOG_2D[i][j]-min)/(max-min));
        }
    }

    //Thresholding the 2D normalized array
    unsigned char** image2D_thresholded_out = NULL;
    int** image2D_zerocrossing = NULL;
    image2D_thresholded_out = create_2Darray(image2D_thresholded_out, width, height);
    image2D_zerocrossing = create_2Darray(image2D_zerocrossing, width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(image2D_normalized[i][j] < 123) {
                image2D_thresholded_out[i][j] = (unsigned char) 64;
                image2D_zerocrossing[i][j] = -1;
            }
            else if(image2D_normalized[i][j] > 156){
                image2D_thresholded_out[i][j] = (unsigned char)192;
                image2D_zerocrossing[i][j] = 1;
            }
            else{
                image2D_thresholded_out[i][j] = (unsigned char)128;
                image2D_zerocrossing[i][j] = 0;
            }
        }
    }

    unsigned char** output = NULL;
    output = create_2Darray(output, width, height);
    unsigned char* image1D_thresholded = NULL;
    image1D_thresholded = convert_2Dto1D(image2D_thresholded_out, width, height);
    write_1Darray_to_file(image1D_thresholded,width, height, grayOutputFilename);

    int x, y,N = 3, xStart = 0, yStart = 0, xLimit = 0, yLimit = 0, index = 0, row, col, zero_crossing =0;    int *imgNxN = NULL;
    imgNxN = new int[N * N];
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            if (image2D_zerocrossing[i][j] == 0) {
                xStart = i - (N / 2);
                yStart = j - (N / 2);
                yLimit = j + (N / 2);
                xLimit = i + (N / 2);
                index = 0;
                memset(imgNxN, 0, N * N* sizeof(int));
                for (x = xStart; x <= xLimit; ++x) {
                    for (y = yStart; y <= yLimit; ++y) {
                        row = x;
                        col = y;
                        if (row < 0) row = 0;
                        if (col < 0) col = 0;
                        if (row > height - 1) row = height - 1;
                        if (col > width - 1) col = width - 1;
                        imgNxN[index] = image2D_zerocrossing[row][col];
                        index++;
                    }
                }
                zero_crossing=0;
                for (int k = 0; k < 9; ++k) {
                    if (imgNxN[k] == 1 || imgNxN[k] == -1) {
                        zero_crossing += 1;
                    }
                }
                if(zero_crossing > 0)
                    output[i][j] = (unsigned char) 0;
                else if(zero_crossing == 0)
                    output[i][j] = (unsigned char) 255;
                }
            else
                output[i][j] = (unsigned char)255;
        }
    }

    unsigned char* final = NULL;
    final = convert_2Dto1D(output, width, height);
    write_1Darray_to_file(final, width, height, outputFilename);
}



int main(int argc, char *argv[]) {
    FILE *fptr1;

    if (argc != 6) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "Syntax : hw3_prob2a <input_image.raw> <input_image_noisy.raw> <width> <height> <bytes_per_pixel>" << endl;
        return 0;
    }

    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    int bytesPerPixel = atoi(argv[5]);

    char* X_output = "X_gradient_boat.raw";
    char* Y_output = "Y_gradient_boat.raw";
    char* grayOutput = "Grayoutput_Sobel_Boat.raw";
    char* outputFilename = "Sobel_output_boat.raw";
    sobel_edge_detector(argv[1], grayOutput, X_output, Y_output, outputFilename, width, height, bytesPerPixel);

    X_output = "X_gradient_boat_noisy.raw";
    Y_output = "Y_gradient_boat_noisy.raw";
    grayOutput = "Grayout_Sobel_boat_noisy.raw";
    outputFilename = "Sobel_output_boat_noisy.raw";
    sobel_edge_detector(argv[2], grayOutput, X_output, Y_output, outputFilename, width, height, bytesPerPixel);

    grayOutput = "Grayout_LOG_boat.raw";
    outputFilename = "LOG_output_boat.raw";
    LOG_edge_detector(argv[1], width, height, bytesPerPixel, grayOutput, outputFilename);

    grayOutput = "Grayout_LOG_boat_noisy.raw";
    outputFilename = "LOG_output_boat_noisy.raw";
    LOG_edge_detector(argv[2], width, height, bytesPerPixel, grayOutput, outputFilename);
    return 0;
}

