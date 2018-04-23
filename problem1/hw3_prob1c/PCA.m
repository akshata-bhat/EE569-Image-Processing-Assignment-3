clc;

close all;

data=load('/Users/akshata/Documents/file.txt','-ascii');

coeff=pca(data,'NumComponents',5);

finaldata=data*coeff;

dlmwrite('/Users/akshata/Documents/file_matlab.txt',finaldata,'\t');

