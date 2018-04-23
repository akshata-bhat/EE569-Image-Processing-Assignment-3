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
3. You should see the program executable named - ./hw3_prob1b in this build folder
4. hw3_prob1a executable should be run with these arguments:
program_name =./hw3_prob1b
hw3_prob1b <input.raw> <width> <height>"
where;
program_name: the name of the .exe file generated
input_image1: the name of the .raw file along with path to be taken as original input
width: Width of the image
height: Height of the image
