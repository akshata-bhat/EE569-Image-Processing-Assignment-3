EE569 Homework Assignment 3
Date: March 4, 2018
Name : Akshata Bhat
USC ID : 9560895350
Email : akshatab@usc.edu
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
SYSTEM CONFIGURATION IN WHICH THIS CODE RAN:

OS: Linux Ubuntu 64-bit 16.04

Software: Terminal
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
STEPS TO DEBUG CODE:


1. Open Terminal

2. Run this command in the project folder:

STEPS TO DEBUG CODE:
1. Open Command prompt.
2. Run this command in the project folder:
mkdir build; cd build; cmake ..; make
3. You should see the program executable named - ./hw3_prob1a in this build folder
4. hw3_prob1a executable should be run with these arguments:
program_name =./hw3_prob1a
hw3_prob1a <width> <height> <input1.raw> <input2.raw> <input3.raw> <input4.raw> <input5.raw> <input6.raw> <input7.raw> <input8.raw> <input9.raw> <input10.raw>  <input11.raw> <input12.raw> 
where;
program_name: the name of the .exe file generated
input_image1: the name of the .raw file along with path to be taken as original input
input_image2: the name of the .raw file along with path to be taken as original input
input_image3: the name of the .raw file along with path to be taken as original input
input_image4: the name of the .raw file along with path to be taken as original input
input_image5: the name of the .raw file along with path to be taken as original input
input_image6: the name of the .raw file along with path to be taken as original input
input_image7: the name of the .raw file along with path to be taken as original input
input_image8: the name of the .raw file along with path to be taken as original input
input_image9: the name of the .raw file along with path to be taken as original input
input_image10: the name of the .raw file along with path to be taken as original input
input_image11: the name of the .raw file along with path to be taken as original input
input_image12: the name of the .raw file along with path to be taken as original input
width: Width of the image
height: Height of the image
bytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)
