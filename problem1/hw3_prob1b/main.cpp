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

double** normalize_image(unsigned char* inImage, int width, int height){
    image obj;
    unsigned char** inputImage_char = NULL;
    double** inputImage = NULL;
    inputImage_char = obj.convert_1Dto2D(inImage, width, height);
    inputImage = obj.convert_char_to_double_2D(inputImage_char, width, height);
    double** finalOutput = NULL;
    finalOutput = obj.create_2Darray(finalOutput, width, height);
    int i, j, x, y,N = 21, xStart = 0, yStart = 0, xLimit = 0, yLimit = 0, index = 0, row, col, bond = 0;
    double sumNxN = 0, meanNxN=0;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            xStart = i - (N / 2);
            yStart = j - (N / 2);
            yLimit = j + (N / 2);
            xLimit = i + (N / 2);
            sumNxN = 0;
            for (x = xStart; x <= xLimit; ++x) {
                for (y = yStart; y <= yLimit; ++y) {
                    row = x;
                    col = y;
                    if (row < 0) row = 0;
                    if (col < 0) col = 0;
                    if (row > height - 1) row = height - 1;
                    if (col > width - 1) col = width - 1;
                    sumNxN += (double)inputImage[row][col];
                }
            }
            meanNxN = sumNxN/double(N*N);
            inputImage[i][j] = (inputImage[i][j] - meanNxN);
            //  cout<<inputImage[i][j]<< " "<<i<<" "<<j<<endl;

        }
    }

    //  finalOutput = obj.convert_char_to_double_2D(inputImage, width, height);
    return inputImage;

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
    int i, j, x, y, xStart = 0, yStart = 0, xLimit = 0, yLimit = 0, index = 0, row, col;
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
            //  cout<<featureImage[i][j]<<" "<<i<<" "<<j<<endl;
        }
    }
    return featureImage;
}

double** calculate_energy_of_image(double** featureImage, int width, int height){
    image obj;
    double** energy_values = NULL;
    energy_values = obj.create_2Darray(energy_values, width, height);
    int i, j, x, y, xStart = 0, yStart = 0, xLimit = 0, yLimit = 0, index = 0, row, col, N=21;
    double sumNxN = 0;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            xStart = i - (N / 2);
            yStart = j - (N / 2);
            yLimit = j + (N / 2);
            xLimit = i + (N / 2);
            sumNxN = 0.0;
            for (x = xStart; x <= xLimit; ++x) {
                for (y = yStart; y <= yLimit; ++y) {
                    row = x;
                    col = y;
                    if (row < 0) row = 0;
                    if (col < 0) col = 0;
                    if (row > height - 1) row = height - 1;
                    if (col > width - 1) col = width - 1;
                    sumNxN += pow(featureImage[row][col],2);
                }
            }
            energy_values[i][j] = sumNxN/double(N*N);
            //    cout<<energy_values[i][j]<<" "<<i<<endl;
        }
    }
    return energy_values;
}

void save_composite_textures(int texture_number, double** energyValue, double** compositeTextures, int width, int height)
{
    int index=0;
    for(int i=0; i<height; i++){
        for (int j = 0; j < width; ++j) {
            compositeTextures[index++][texture_number] = energyValue[i][j];
        }
    }
}

double** generate_9D_feature_vectors(double** grayscaleImage2D, int width,int height, double** Lawsfilters, int N) {
    image obj;
    double** featureImage2D = NULL;
    double** energyImage = NULL;
    double** compositeTextures = NULL;
    compositeTextures = obj.create_2Darray(compositeTextures, 9, width*height);
    energyImage = obj.create_2Darray(energyImage, width, height);

    for (int i = 0; i < 9; ++i) {
        featureImage2D = feature_extraction(grayscaleImage2D, Lawsfilters[i], width, height, 3);
        energyImage = calculate_energy_of_image(featureImage2D, width, height);
        save_composite_textures(i, energyImage, compositeTextures, width, height);
    }
    return compositeTextures;
}

int closest_centroid(double** centroid, double* composite_texture_row){
    image obj;
    double sum =0.0, min=0.0;
    double* sorted_value = NULL;
    double* value = NULL;
    double* original_value = NULL;
    value = obj.create_1Darray(value, 6);
    for (int i = 0; i < 6; ++i) {
        sum = 0.0;
        for (int j = 0; j < 9; ++j) {
            //  cout << composite_texture_row[55093] << endl;

            sum = sum + pow((centroid[i][j] - composite_texture_row[j]),2);
            //cout <<i<<" "<<j<<"  "<< centroid[i][j]<<"  "<< composite_texture_row[j]<< endl;
        }
        sum = sqrt(sum);
        value[i] = sum;
        //cout<<value[i]<<"  " <<i<<endl;
    }

    if(value[0] < value[1])
        min = value[0];
    else
        min = value[1];
    if(min > value[2])
        min = value[2];
    if(min > value[3])
        min = value[3];
    if(min > value[4])
        min = value[4];
    if(min > value[5])
        min = value[5];

    if(min == value[0])
        return 0;
    else if(min == value[1])
        return 1;
    else if(min == value[2])
        return 2;
    else if(min == value[3])
        return 3;
    else if(min == value[4])
        return 4;
    else
        return 5;
}

int main(int argc, const char *argv[]) {
    FILE *fptr1;

    if (argc != 4) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "Syntax : hw3_prob1a <input.raw> <width> <height>"
             << endl;
        return 0;
    }

    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    image texture1(width, height, 1, argv[1]);

    int N=3;
    double* E3 = NULL;
    //E3 = new double[N] {1.0/6.0, 2.0/6.0, 1.0/6.0};
    E3 = new double[N] {-1.0/2.0, 0, 1.0/2.0};

    double* S3 = NULL;
    // S3 = new double[N] {-1.0/2.0, 0, 1.0/2.0};
    S3 = new double[N] {-1.0/2.0, 2.0/2.0, -1.0/2.0};

    double* L3 = NULL;
    // L3 = new double[N] {-1.0/2.0, 2.0/2.0, -1.0/2.0};
    L3 = new double[N] {1.0/6.0, 2.0/6.0, 1.0/6.0};

    image object1;
    int index=0;
    double** LawsFilters = object1.create_2Darray(LawsFilters, 9, N*N);
    LawsFilters[index++] = generate_tensor_product(E3, E3, N);//E3E3
    LawsFilters[index++] = generate_tensor_product(E3, S3, N);//E3S3
    LawsFilters[index++] = generate_tensor_product(E3, L3, N);//E3L3
    LawsFilters[index++] = generate_tensor_product(S3, E3, N);//S3E3
    LawsFilters[index++] = generate_tensor_product(S3, S3, N);//S3S3
    LawsFilters[index++] = generate_tensor_product(S3, L3, N);//S3L3
    LawsFilters[index++] = generate_tensor_product(L3, E3, N);//L3E3
    LawsFilters[index++] = generate_tensor_product(L3, S3, N);//L3S3
    LawsFilters[index++] = generate_tensor_product(L3, L3, N);//L3L3


    index=0;
    double** normalized_texture1 = NULL;
    normalized_texture1 = normalize_image(texture1.inputImage1D, width, height);

    double** composite_textures = NULL;
    composite_textures = generate_9D_feature_vectors(normalized_texture1, width, height, LawsFilters, 3);

    index=0;
    double** centroid = object1.create_2Darray(centroid, 9, 6);
    centroid[index++] = composite_textures[21095];//82*width+108];
    centroid[index++] = composite_textures[24895];//82*width+485];
    centroid[index++] = composite_textures[55093];//336*width+108];
    centroid[index++] = composite_textures[132891];//336*width+485];
    centroid[index++] = composite_textures[222680];//200*width+300];
    centroid[index++] = composite_textures[218883];//200*width+150];

    // for (int m = 0; m < height*width; ++m) {

    //   }

    int* labels = NULL;
    int* centroid_count = NULL;
    labels = object1.create_1Darray(labels, width*height);
    double** prev_centroid = NULL;
    prev_centroid = object1.create_2Darray(prev_centroid, 9, 6);
    double** sum = NULL;
    sum = object1.create_2Darray(sum, 9, 6);
    //while(prev_centroid[0]!=centroid[0] && prev_centroid[1]!=centroid[1] && prev_centroid[2]!=centroid[2] && prev_centroid[3]!=centroid[3] && prev_centroid[4]!=centroid[4] && prev_centroid[5]!=centroid[5]) {
    for (int l = 0; l < 20 ; ++l) {
        for (int k = 0; k < width*height; ++k) {
            labels[k] = closest_centroid(centroid, composite_textures[k]);

        }
        index=0;


        centroid_count = new int[6] {0, 0, 0, 0, 0, 0};

        for (int i = 0; i < 6; ++i) {
            memset(sum[i], 0.0, 9 *sizeof(double));
        }

        for(int i=0;i<height*width;i++){
            for (int j = 0; j < 9; ++j) {
                if (labels[i]==0){
                    sum[0][j] = sum[0][j] + composite_textures[i][j];
                    centroid_count[0]+=1;
                }
                if (labels[i]==1) {
                    sum[1][j] = sum[1][j] + composite_textures[i][j];
                    centroid_count[1] += 1;

                }
                if (labels[i]==2) {
                    sum[2][j] = sum[2][j] + composite_textures[i][j];
                    centroid_count[2]+=1;
                }
                if (labels[i]==3){
                    sum[3][j] = sum[3][j] + composite_textures[i][j];
                    centroid_count[3]+=1;
                }
                if (labels[i]==4){
                    sum[4][j] = sum[4][j] + composite_textures[i][j];
                    centroid_count[4]+=1;
                }
                if (labels[i]==5){
                    sum[5][j] = sum[5][j] + composite_textures[i][j];
                    centroid_count[5]+=1;
                }
            }
        }

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j< 9; ++j) {
                double val = centroid_count[i]/9.0;
                centroid[i][j] = sum[i][j]/double(val);
            }
        }

    }


    int* intensities = new int[6] {0, 51, 102, 153, 204, 255};
    unsigned char* outputImage = NULL;
    outputImage = object1.create_1Darray(outputImage, width*height);

    for (int k = 0; k < width*height; ++k) {
        outputImage[k] = (unsigned char)intensities[labels[k]];
    }

    char* filename1 = "Composite_output.raw";
    object1.write_1Darray_to_file(outputImage, width, height, filename1);
    return 0;

}


