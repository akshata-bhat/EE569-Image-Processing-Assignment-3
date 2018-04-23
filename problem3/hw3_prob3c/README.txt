EE569 Homework Assignment 3
Date: March 28, 2018
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
3. You should see the program executable named - ./hw3_prob3c in this build folder
4. hw3_prob3c executable should be run with these arguments:
program_name =./hw3_prob3c
./BagOfWords <sampleImage1> <sampleImage2> <sampleImage3> <targetImage>
where;
program_name: the name of the .exe file generated
<img1> First image to be compared
<img2> Second image to be compared
<img3> third image to be compared
<target image> Image to be compared
