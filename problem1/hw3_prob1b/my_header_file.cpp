
// Author : Akshata Bhat
// USC ID : 9560895350
// Email : akshatab@usc.edu
// Submission Date : 3/25/2018

#include "my_header_file.h"

using namespace std;

//Constructor
image::image(int Width,int Height, int BytesPerPixel, const char* filename)
{
    width = Width;
    height = Height;
    bytesPerPixel = BytesPerPixel;
    inputImage1D = read_1Darray_from_file(width, height, filename);
}

image::image(){}

// Destructor
image::~image(void){
}

float** image::create_2Darray(float** array, int width, int height)
{
    array = new float *[height];
    for (int i = 0; i < height; ++i) {
        array[i] = new float[width];
        memset(array[i], 0, width *sizeof(float));
    }
    return array;
}

double** image::create_2Darray(double** array, int width, int height)
{
    array = new double *[height];
    for (int i = 0; i < height; ++i) {
        array[i] = new double[width];
        memset(array[i], 0, width *sizeof(double));
    }
    return array;
}

int** image::create_2Darray(int** array, int width, int height)
{
    int i;
    array = new int *[height];
    for (i = 0; i < height; ++i) {
        array[i] = new int[width];
        memset(array[i], 0, width *sizeof(int));
    }
    return array;
}

unsigned char** image::create_2Darray(unsigned char** array, int width, int height)
{
    int i;
    array = new unsigned char *[height];
    for (i = 0; i < height; ++i) {
        array[i] = new unsigned char[width];
        memset(array[i], 0, width *sizeof(unsigned char));
    }
    return array;
}

unsigned char* image::create_1Darray(unsigned char* array, int size)
{
    int i;
    array = new unsigned char[size];
    memset(array, 0, size *sizeof(unsigned char));
    return array;
}

double* image::create_1Darray(double* array, int size)
{
    int i;
    array = new double[size];
    memset(array, 0, size *sizeof(double));
    return array;
}

int* image::create_1Darray(int* array, int size)
{
    int i;
    array = new int[size];
    memset(array, 0, size *sizeof(int));
    return array;
}

unsigned char** image::convert_1Dto2D(unsigned char* input1D, int width, int height)
{
    int i, j,count=0;
    unsigned char** output2D=NULL;
    output2D = create_2Darray(output2D, width, height);
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            output2D[i][j]=input1D[count];
            count=count+1;
        }
    }
    return output2D;
}

unsigned char* image::convert_2Dto1D(unsigned char** input2D, int width, int height)
{
    int i, j,count=0;
    int totalBytes = width*height;
    unsigned char* output1D = NULL;
    output1D = new unsigned char[totalBytes];
    memset(output1D,0,totalBytes);
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            output1D[count]=input2D[i][j];
            count=count+1;
        }
    }
    return output1D;
}

int** image::convert_1Dto2D(int* input1D, int width, int height)
{
    int i, j,count=0;
    int** output2D=NULL;
    output2D = create_2Darray(output2D, width, height);
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            output2D[i][j]=input1D[count];
            count=count+1;
        }
    }
    return output2D;
}

int* image::convert_2Dto1D(int** input2D, int width, int height)
{
    int i, j,count=0;
    int totalBytes = width*height;
    int* output1D = NULL;
    output1D = new int[totalBytes];
    memset(output1D,0,totalBytes);
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            output1D[count]=input2D[i][j];
            count=count+1;
        }
    }
    return output1D;
}
void image::write_1Darray_to_file(unsigned char* array, int width, int height, const char* filename)
{
    FILE* fptr;
    if (!(fptr=fopen(filename,"wb"))) {
        cout << "Cannot open file." << endl;
        exit(1);
    }
    fwrite(array, sizeof(unsigned char),width*height, fptr);
    fclose(fptr);
}

unsigned char* image::read_1Darray_from_file(int width, int height, const char* filename)
{
    int totalBytes = width*height;
    unsigned char *array = NULL;
    array = new unsigned char[totalBytes];
    memset(array, 0, totalBytes);
    FILE* fptr;
    if (!(fptr=fopen(filename,"rb"))) {
        cout << "Cannot open file." << endl;
        exit(1);
    }
    fread(array, sizeof(unsigned char),width*height, fptr);
    fclose(fptr);
    return array;
}

unsigned char** image::read_2Darray_from_file(int width, int height, const char* filename)
{
    int totalBytes = width*height;
    unsigned char *array1D = NULL;
    array1D = new unsigned char[totalBytes];
    memset(array1D, 0, totalBytes);
    FILE* fptr;
    if (!(fptr=fopen(filename,"rb"))) {
        cout << "Cannot open file." << endl;
        exit(1);
    }
    fread(array1D, sizeof(unsigned char),width*height, fptr);
    fclose(fptr);


    unsigned char** array2D = NULL;
    array2D = create_2Darray(array2D, width, height);

    int index = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            array2D[i][j] = array1D[index++];
        }
    }

    return array2D;
}

unsigned char* image::convert_int_to_char_1D(int* inputArray, int width, int height){
    unsigned  char* outputArray = NULL;
    outputArray = new unsigned char[width*height];

    for (int i = 0; i < width*height; ++i) {
        outputArray[i] = (unsigned char)inputArray[i];
    }
    return outputArray;
}

double** image::convert_char_to_double_2D(unsigned char** inputArray, int width, int height){
    double** outputArray = NULL;
    outputArray = image::create_2Darray(outputArray, width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            outputArray[i][j] = (double)inputArray[i][j];
        }
    }
    return outputArray;
}


unsigned char min_val(unsigned char a, unsigned char b, unsigned char c)
{

    if( a<=b && a<=c )
        return a;

    else if( b<=a && b<=c )
        return b;

    else if( c<=a && c<=b )
        return c;

}

unsigned char image::max_val(unsigned char a, unsigned char b, unsigned char c)
{

    if( a>=b && a>=c )
        return a;

    else if( b>=a && b>=c )
        return b;

    else if( c>=a && c>=b )
        return c;

}

int image::mostFrequentValue(int a[], int length)
{
    int count = 1, tempValCount;
    int tempVal = 0 ;
    int mostFrequent = a[0];
    for (int i = 0; i < (length - 1); i++)
    {
        tempVal = a[i];
        tempValCount = 0;
        for (int j = 1; j < length; j++)
        {
            if (tempVal == a[j])
                tempValCount++;
        }
        if (tempValCount > count)
        {
            count = tempValCount;
            mostFrequent = tempVal;
        }
    }
    return mostFrequent;
}

int image::median_value_calc(int list[], int length)
{
    int center = (length+1)/2;
    int insert_pos,valueTo_insert;

    for(int i=1; i<length; ++i){
        insert_pos=i;
        valueTo_insert = list[insert_pos];

        while(insert_pos > 0 && list[insert_pos-1] > valueTo_insert){
            list[insert_pos]=list[insert_pos-1];
            insert_pos--;
        }
        if(insert_pos!=i){
            list[insert_pos]=valueTo_insert;
        }
    }
    return list[center];
}

int image::min_val_index(float intensity, float b[])
{
    float diff[256];
    float min;
    int position = 0;
    for(int i=0; i<256; i++ )
    {
        diff[i] = abs(intensity - b[i]);
    }
    min = diff[0];
    for(int i=1; i<256; i++)
    {
        if(min >= diff[i])
        {
            min = diff[i];
            position = i;
        }
    }
    return position;
}

double image::psnr_calc(unsigned char a[], unsigned char b[], int width, int height)
{
    double MSE = 0.0;
    double psnr = 0.0;
    double MSE_diff = 0.0;
    int MAX = 255;
    int i;

    for(i=0; i < width*height; i++)
    {
        MSE_diff = abs(int(a[i]) - int(b[i]));
        MSE = MSE + MSE_diff*MSE_diff;
    }
    MSE = double(MSE)/double(width*height);

    if(MSE == 0)
        return 99.0;

    psnr = 10*log10(MAX*MAX/MSE);
    return psnr;
}

//Function to perform insertion sort with only the non-zero elements sorted
int* image::insertion_sort_increasing(int* list,int length) {
    int *final = NULL;
    final = new int[length];
    memset(final, 0, 4 * sizeof(int));
    int insert_pos, valueTo_insert;
    for (int i = 1; i < length; ++i) {
        insert_pos = i;
        valueTo_insert = list[insert_pos];
        while (insert_pos > 0 && list[insert_pos - 1] > valueTo_insert) {
            list[insert_pos] = list[insert_pos - 1];
            insert_pos--;
        }

        if (insert_pos != i) {
            list[insert_pos] = valueTo_insert;
        }
    }
    int l=0;
    for (int k = 0; k < length; ++k) {
        if (list[k] > 0) {
            if(list[k] == list[k-1]) {
                continue;
            }
            final[l] = list[k];
            l++;
        }
    }
    return final;
}

// Function to sort the array in decreasing order - max first
int* image::sort_decreasing(int* list,int length) {
    //int array[10]={ 50, 9, 2, 7, 5, 8, 2, 0, 3, 58};
    int i,j=0,temp1;
    bool swapped = false;
    for(i=0;i<length;i++){
        for(j=0;j<(length-i);j++){//This"length-i"isneededcauseattheendofeachloop-the
            if(list[j]<list[j+1]){//largestelementispushedtowardstheendandhencethereisnoneed
                temp1=list[j];
                list[j]=list[j+1];
                list[j+1]=temp1;
                swapped=true;
            }
        }
        if(!(swapped)){
            break;
        }
    }

    return list;
}

double* image::sort_decreasing(double* list,int length) {
    //int array[10]={ 50, 9, 2, 7, 5, 8, 2, 0, 3, 58};
    int i,j=0;
    double temp1;
    bool swapped = false;
    for(i=0;i<length;i++){
        for(j=0;j<(length-i);j++){//This"length-i"isneededcauseattheendofeachloop-the
            if(list[j]<list[j+1]){//largestelementispushedtowardstheendandhencethereisnoneed
                temp1=list[j];
                list[j]=list[j+1];
                list[j+1]=temp1;
                swapped=true;
            }
        }
        if(!(swapped)){
            break;
        }
    }

    return list;
}