#!/usr/bin/python

# 2.12 Lab 9
# Luke Roberto Oct 2017
# Jacob Guggenheim 2019
# Jerry Ng 2019, 2020


import numpy as np
import cv2  # OpenCV module
import time
import math
from matplotlib import pyplot as plt

def main():
    cap = cv2.VideoCapture(0)

    ret, cv_image = cap.read()    

    # otsu
    grayIm = cv2.cvtColor(cv_image,cv2.COLOR_BGR2GRAY)
    blurIm = cv2.GaussianBlur(grayIm,(15,5),0)
    blurThresh, otsuImage = cv2.threshold(blurIm,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)

    # show images
    images = [blurIm, 0, otsuImage]
    for i in range(1):
        plt.subplot(1,3,i*3+1), plt.imshow(images[i*3], 'gray'),
        plt.subplot(1,3,i*3+2), plt.hist(images[i*3].ravel(),256),  plt.axvline(x=blurThresh, color='c')
        plt.subplot(1,3,i*3+3), plt.imshow(images[i*3+2], 'gray')
    plt.show()
    plt.close()

if __name__=='__main__':
    main()