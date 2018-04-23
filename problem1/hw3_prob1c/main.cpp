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
    compositeTextures = obj.create_2Darray(compositeTextures, 25, width*height);
    energyImage = obj.create_2Darray(energyImage, width, height);

    for (int i = 0; i < 25; ++i) {
        featureImage2D = feature_extraction(grayscaleImage2D, Lawsfilters[i], width, height, 5);
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
        for (int j = 0; j < 5; ++j) {
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

    int N=5;
    double E5[5] =  {(double)-1/6 , (double)-2/6 , 0 , (double) 2/6  , (double)1/6 };
    double S5[5] =  {(double)-1/4, 0 , (double) 2/4, 0, (double)-1/4};
    double W5[5] =  {(double)-1/6, (double)2/6, 0 , (double)-2/6 , (double)1/6};
    double L5[5] =  {(double)1/16, (double) 4/16 , (double) 6/16, (double) 4/16, (double)1/16};
    double R5[5] =  {(double)1/16, (double) -4/16 , (double) 6/16, (double) -4/16, (double)1/16};


    image object1;
    int index=0;
    double** LawsFilters = object1.create_2Darray(LawsFilters, 25, N*N);
    LawsFilters[index++] = generate_tensor_product(E5, E5, N);//E3E3
    LawsFilters[index++] = generate_tensor_product(E5, S5, N);//E3S3
    LawsFilters[index++] = generate_tensor_product(E5, W5, N);//E3L3
    LawsFilters[index++] = generate_tensor_product(E5, L5, N);//S3E3
    LawsFilters[index++] = generate_tensor_product(E5, R5, N);//S3S3


    LawsFilters[index++] = generate_tensor_product(S5, E5, N);//E3L3
    LawsFilters[index++] = generate_tensor_product(S5, S5, N);//S3E3
    LawsFilters[index++] = generate_tensor_product(S5, W5, N);//S3S3
    LawsFilters[index++] = generate_tensor_product(S5, L5, N);//S3L3
    LawsFilters[index++] = generate_tensor_product(S5, R5, N);//S3L3


    LawsFilters[index++] = generate_tensor_product(W5, E5, N);//S3L3
    LawsFilters[index++] = generate_tensor_product(W5, S5, N);//L3E3
    LawsFilters[index++] = generate_tensor_product(W5, W5, N);//L3S3
    LawsFilters[index++] = generate_tensor_product(W5, L5, N);//L3L3
    LawsFilters[index++] = generate_tensor_product(W5, R5, N);//E3E3


    LawsFilters[index++] = generate_tensor_product(L5, E5, N);//E3S3
    LawsFilters[index++] = generate_tensor_product(L5, S5, N);//L3E3
    LawsFilters[index++] = generate_tensor_product(L5, W5, N);//L3S3
    LawsFilters[index++] = generate_tensor_product(L5, L5, N);//L3L3
    LawsFilters[index++] = generate_tensor_product(L5, R5, N);//E3E3


    LawsFilters[index++] = generate_tensor_product(R5, E5, N);//E3S3
    LawsFilters[index++] = generate_tensor_product(R5, S5, N);//E3L3
    LawsFilters[index++] = generate_tensor_product(R5, W5, N);//S3E3
    LawsFilters[index++] = generate_tensor_product(R5, L5, N);//S3S3
    LawsFilters[index++] = generate_tensor_product(R5, R5, N);//S3L3


    index=0;
    double** normalized_texture1 = NULL;
    normalized_texture1 = normalize_image(texture1.inputImage1D, width, height);

    double** composite_textures = NULL;
    composite_textures = generate_9D_feature_vectors(normalized_texture1, width, height, LawsFilters, 5);


    char* filename1 = "OUTPUT.txt";
    FILE* fptr;
    if (!(fptr=fopen(filename1,"wb"))) {
        cout << "Cannot open file." << endl;
        exit(1);
    }
    for(int i = 0; i < width*height; i++){
        for(int j = 0 ; j < 25; j ++){
            fprintf(fptr, "%lf\t", composite_textures[i][j]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);

    double x = 0 ;
    std::ifstream inFile;
    inFile.open("file_matlab.txt");
    int pca_dim = 600*450*5;
    int pca_height = 600*450;
    int pca_width = 5;

    double** pca_array = NULL;
    image obj;
    pca_array = obj.create_2Darray(pca_array, 5, width*height);

    if(!inFile){
        std::cout<<"Unable to open file"<<std::endl;
        exit(1);
    }
    int row = 0;
    int col = 0;
    while(inFile >> x){
        pca_array[row][col] = x;
        col++;
        if(col == pca_width)
        {
            col = 0;
            row++;
        }
    }
    inFile.close();

    for(int i = 0; i < 600*450; i++){
        for(int j = 0; j < 5 ; j++){
            cout<<pca_array[i][j]<<endl;
        }
    }
    index=0;
    double** centroid = object1.create_2Darray(centroid, 5, 6);
    centroid[index++] = pca_array[21095];//82*width+108];
    centroid[index++] = pca_array[24895];//82*width+485];
    centroid[index++] = pca_array[55093];//336*width+108];
    centroid[index++] = pca_array[132891];//336*width+485];
    centroid[index++] = pca_array[222680];//200*width+300];
    centroid[index++] = pca_array[218883];//200*width+150];


    int* labels = NULL;
    int* centroid_count = NULL;
    labels = object1.create_1Darray(labels, width*height);
    double** prev_centroid = NULL;
    prev_centroid = object1.create_2Darray(prev_centroid, 5, 6);
    double** sum = NULL;
    sum = object1.create_2Darray(sum, 5, 6);
    //while(prev_centroid[0]!=centroid[0] && prev_centroid[1]!=centroid[1] && prev_centroid[2]!=centroid[2] && prev_centroid[3]!=centroid[3] && prev_centroid[4]!=centroid[4] && prev_centroid[5]!=centroid[5]) {
    for (int l = 0; l < 20 ; ++l) {
        //cout << "l = " << l << endl;
        for (int k = 0; k < width*height; ++k) {
            labels[k] = closest_centroid(centroid, pca_array[k]);
            //if(labels[k] == 4)
            // cout << k << endl;
        }
        index=0;

        centroid_count = new int[6] {0, 0, 0, 0, 0, 0};

        for (int i = 0; i < 6; ++i) {
            memset(sum[i], 0.0, 5 *sizeof(double));
        }


        for(int i=0;i<height*width;i++){
            for (int j = 0; j < 5; ++j) {
                if (labels[i]==0){
                    sum[0][j] = sum[0][j] + pca_array[i][j];
                    centroid_count[0]+=1;
                }
                if (labels[i]==1) {
                    sum[1][j] = sum[1][j] + pca_array[i][j];
                    centroid_count[1] += 1;

                }
                if (labels[i]==2) {
                    sum[2][j] = sum[2][j] + pca_array[i][j];
                    centroid_count[2]+=1;
                }
                if (labels[i]==3){
                    sum[3][j] = sum[3][j] + pca_array[i][j];
                    centroid_count[3]+=1;
                }
                if (labels[i]==4){
                    sum[4][j] = sum[4][j] + pca_array[i][j];
                    centroid_count[4]+=1;
                }
                if (labels[i]==5){
                    sum[5][j] = sum[5][j] + pca_array[i][j];
                    centroid_count[5]+=1;
                }
            }
        }

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j< 5; ++j) {
                double val = centroid_count[i]/5.0;
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

    char* filenameblah = "Composite_output_pca.raw";
    object1.write_1Darray_to_file(outputImage, width, height, filenameblah);
    return 0;

}

/* for (int j = 0; j < width*height; ++j) {
     //cout << labels[j] << "\t" ;
 }*/


