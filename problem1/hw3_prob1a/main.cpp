// Author : Akshata Bhat
// USC ID : 9560895350
// Email : akshatab@usc.edu
// Submission Date : 03/28/2018

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "my_header_file.h"
using namespace std;

//Function to generate the tensor product
double* generate_tensor_product(double* filter1, double* filter2, int N){
    int index=0;
    image object;
    double* product = NULL;
    product = object.create_1Darray(product, N*N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            product[index++] = filter1[i]*filter2[j];
        }
    }
    return product;
}

double generate_feature_vectors(double* laws_mask, double* image_mask, int N){
    double result = 0;
    for (int i = 0; i < N*N; ++i) {
        result = result + laws_mask[i]*image_mask[i];
    }
    return result;
}

double** feature_extraction(double** grayscaleImage2D, double* lawsMask, int width, int height, int N) {
    image obj;
    double **featureImage = NULL;
    featureImage = obj.create_2Darray(featureImage, width, height);
    int i, j, x, y, xStart = 0, yStart = 0, xLimit = 0, yLimit = 0, index = 0, row, col, bond = 0;
    double *imgNxN = NULL;
    imgNxN = new double[N * N];
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            xStart = i - (N / 2);
            yStart = j - (N / 2);
            yLimit = j + (N / 2);
            xLimit = i + (N / 2);
            index = 0;
            memset(imgNxN, 0, N * N* sizeof(double));
            for (x = xStart; x <= xLimit; ++x) {
                for (y = yStart; y <= yLimit; ++y) {
                    row = x;
                    col = y;
                    if (row < 0) row = 0;
                    if (col < 0) col = 0;
                    if (row > height - 1) row = height - 1;
                    if (col > width - 1) col = width - 1;
                    imgNxN[index] = grayscaleImage2D[row][col];
                    index++;
                }
            }
            featureImage[i][j] = generate_feature_vectors(lawsMask, imgNxN, N);
        }
    }
    return featureImage;
}

double calculate_energy_of_image(double** featureImage, int width, int height){
    double energy_value = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            energy_value = energy_value+pow(featureImage[i][j],2);
        }
    }
    energy_value = energy_value/(double (width*height));
    return  energy_value;
}

double* generate_9D_feature_vector(double** grayscaleImage2D, int width,int height, double** Lawsfilters, int N) {
    image obj;
    double** featureImage2D = NULL;
    double* energyValues = NULL;
    energyValues = obj.create_1Darray(energyValues, 9);
    for (int i = 0; i < 9; ++i) {
        featureImage2D = feature_extraction(grayscaleImage2D, Lawsfilters[i], width, height, N);
        energyValues[i] = calculate_energy_of_image(featureImage2D, width, height);
    }
    return energyValues;
}

int closest_centroid(double** centroid, double* feature_vectors){
    image obj;
    double sum =0.0, min=0.0;
    double* sorted_value = NULL;
    double* value = NULL;
    double* original_value = NULL;
    value = obj.create_1Darray(value, 4);
    for (int i = 0; i < 4; ++i) {
        sum = 0.0;
        for (int j = 0; j < 9; ++j) {
            sum = sum + pow((centroid[i][j] - feature_vectors[j]),2);
        }
        sum = sqrt(sum);
        value[i] = sum;
    }

    if(value[0] < value[1])
        min = value[0];
    else
        min = value[1];
    if(min > value[2])
        min = value[2];
    if(min > value[3])
        min = value[3];

    if(min == value[0])
        return 0;
    else if(min == value[1])
        return 1;
    else if(min == value[2])
        return 2;
    else
        return 3;


}

double** normalize_image(unsigned char* inImage, int width, int height){
    image obj;
    unsigned char** inputImage = NULL;
    inputImage = obj.convert_1Dto2D(inImage, width, height);
    double** finalOutput = NULL;
    finalOutput = obj.create_2Darray(finalOutput, width, height);
    double sum = 0, mean;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            sum = sum + (double)inputImage[i][j];
        }
    }
    mean = sum/double(width*height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            finalOutput[i][j] = (double)inputImage[i][j] - mean;
        }
    }
    return finalOutput;
}

void writetoFile(string path,double **inputImage, int rows, int cols) {
    ofstream arrayData;
    arrayData.open(path);
    if (arrayData.is_open()) {
        for (int i = 0; i<rows; i++) {
            for (int j = 0; j < cols; ++j) {
                arrayData << inputImage[i][j];
                arrayData << "\t";
            }
            arrayData << "\n";
        }
        arrayData.close();
    }
    else
    {
        cout << "unable to open file" << endl;
    }
}

int main(int argc, const char *argv[]) {
    FILE *fptr1;

    if (argc != 15) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "Syntax : hw3_prob1a <width> <height> <input1.raw> <input2.raw> <input3.raw> <input4.raw> <input5.raw> <input6.raw> "
                " <input7.raw> <input8.raw> <input9.raw> <input10.raw>  <input11.raw> <input12.raw> "
             << endl;
        return 0;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    image texture1(width, height, 1, argv[3]);
    image texture2(width, height, 1, argv[4]);
    image texture3(width, height, 1, argv[5]);
    image texture4(width, height, 1, argv[6]);
    image texture5(width, height, 1, argv[7]);
    image texture6(width, height, 1, argv[8]);
    image texture7(width, height, 1, argv[9]);
    image texture8(width, height, 1, argv[10]);
    image texture9(width, height, 1, argv[11]);
    image texture10(width, height, 1, argv[12]);
    image texture11(width, height, 1, argv[13]);
    image texture12(width, height, 1, argv[14]);

    int N=5;
    double* E5 = NULL;
    E5 = new double[N] {-1.0/6.0, -2.0/6.0, 0, 2.0/6.0, 1.0/6.0};

    double* S5 = NULL;
    S5 = new double[N] {-1.0/4.0, 0, 2.0/4.0, 0, -1.0/4.0};

    double* W5 = NULL;
    W5 = new double[N] {-1.0/6.0, 2.0/6.0, 0, -2.0/6.0, 1.0/6.0};

    image object1;
    int index=0;
    double** LawsFilters = object1.create_2Darray(LawsFilters, 9, N*N);
    LawsFilters[index++] = generate_tensor_product(E5, E5, N);//E5E5
    LawsFilters[index++] = generate_tensor_product(E5, S5, N);//E5S5
    LawsFilters[index++] = generate_tensor_product(E5, W5, N);//E5W5
    LawsFilters[index++] = generate_tensor_product(S5, E5, N);//S5E5
    LawsFilters[index++] = generate_tensor_product(S5, S5, N);//S5S5
    LawsFilters[index++] = generate_tensor_product(S5, W5, N);//S5W5
    LawsFilters[index++] = generate_tensor_product(W5, E5, N);//W5E5
    LawsFilters[index++] = generate_tensor_product(W5, S5, N);//W5S5
    LawsFilters[index++] = generate_tensor_product(W5, W5, N);//W5W5

    index=0;
    double** normalized_texture1 = NULL;
    double** normalized_texture2 = NULL;
    double** normalized_texture3 = NULL;
    double** normalized_texture4 = NULL;
    double** normalized_texture5 = NULL;
    double** normalized_texture6 = NULL;
    double** normalized_texture7 = NULL;
    double** normalized_texture8 = NULL;
    double** normalized_texture9 = NULL;
    double** normalized_texture10 = NULL;
    double** normalized_texture11 = NULL;
    double** normalized_texture12 = NULL;
    normalized_texture1 = normalize_image(texture1.inputImage1D, width, height);
    normalized_texture2 = normalize_image(texture2.inputImage1D, width, height);
    normalized_texture3 = normalize_image(texture3.inputImage1D, width, height);
    normalized_texture4 = normalize_image(texture4.inputImage1D, width, height);
    normalized_texture5 = normalize_image(texture5.inputImage1D, width, height);
    normalized_texture6 = normalize_image(texture6.inputImage1D, width, height);
    normalized_texture7 = normalize_image(texture7.inputImage1D, width, height);
    normalized_texture8 = normalize_image(texture8.inputImage1D, width, height);
    normalized_texture9 = normalize_image(texture9.inputImage1D, width, height);
    normalized_texture10 = normalize_image(texture10.inputImage1D, width, height);
    normalized_texture11 = normalize_image(texture11.inputImage1D, width, height);
    normalized_texture12 = normalize_image(texture12.inputImage1D, width, height);


    double** feature_vectors = object1.create_2Darray(feature_vectors, 12, 9);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture1, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture2, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture3, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture4, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture5, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture6, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture7, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture8, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture9, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture10, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture11, width, height, LawsFilters, N);
    feature_vectors[index++] = generate_9D_feature_vector(normalized_texture12, width, height, LawsFilters, N);

    //writetoFile("FeatureValues_texture.txt", feature_vectors, 12, 9);


    index=0;
    double** centroid = object1.create_2Darray(centroid, 4, 9);
    centroid[index++] = feature_vectors[0];
    centroid[index++] = feature_vectors[1];
    centroid[index++] = feature_vectors[2];
    centroid[index++] = feature_vectors[6];


    int* labels = NULL;
    int* centroid_count = NULL;
    labels = object1.create_1Darray(labels, 12);
    double** prev_centroid = NULL;
    prev_centroid = object1.create_2Darray(prev_centroid, 4, 9);
    while(prev_centroid[0]!=centroid[0] && prev_centroid[1]!=centroid[1] && prev_centroid[2]!=centroid[2] && prev_centroid[3]!=centroid[3]) {
        for (int k = 0; k < 12; ++k) {
            labels[k] = closest_centroid(centroid, feature_vectors[k]);
        }
        index=0;

        centroid_count = new int[4] {0, 0, 0, 0};

        while(index<12){
            switch(labels[index]){
                case 0:
                    for (int i = 0; i < 9; ++i) {
                        centroid[0][i] = centroid[0][i] + feature_vectors[index][i];
                        centroid_count[0]+=1;
                        //cout << centroid_count[0] << endl;
                    }
                    break;
                case 1:
                    for (int i = 0; i < 9; ++i) {
                        centroid[1][i] = centroid[1][i] + feature_vectors[index][i];
                        centroid_count[1]+=1;
                    }
                    break;
                case 2:
                    for (int i = 0; i < 9; ++i) {
                        centroid[2][i] = centroid[2][i] + feature_vectors[index][i];
                        centroid_count[2]+=1;
                        //cout << centroid[2][i] << endl;
                    }
                    break;
                case 3:
                    for (int i = 0; i < 9; ++i) {
                        centroid[3][i] = centroid[3][i] + feature_vectors[index][i];
                        centroid_count[3]+=1;
                    }
                    break;
            }
            index++;
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 9; ++j) {
                centroid[i][j] = centroid[i][j]/(centroid_count[i]/9.0);
            }
        }
        prev_centroid = centroid;
    }

    for (int j = 0; j < 12; ++j) {
        if(labels[j] == 0 )
            cout << "Label value: " << labels[j] << "\t" << "Rock" << endl ;
        else if(labels[j] == 1 )
            cout << "Label value: " << labels[j] << "\t" << "Grass" << endl ;
        else if(labels[j] == 2 )
            cout << "Label value: " << labels[j] << "\t" << "Weave" << endl ;
        else
            cout << "Label value: " << labels[j] << "\t" << "Sand" << endl ;
    }


    return 0;
}
