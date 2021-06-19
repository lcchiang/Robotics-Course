#!/usr/bin/python

# 2.12 Lab 4 object detection: a node for de-noising
# Luke Roberto Oct 2017
# Jerry Ng April 2020
import numpy as np
import cv2  # OpenCV module
from matplotlib import pyplot as plt
import time
from tkinter import *
import math

global tk
tk = Tk()
global l_b, u_b
l_b = Scale(tk, from_ = 0, to = 1500, label = 'Lower Threshold', orient = HORIZONTAL)
l_b.pack()
u_b = Scale(tk, from_ = 0, to = 1500, label = 'Upper Threshold', orient = HORIZONTAL)
u_b.pack()
u_b.set(1500)

def main():
    # Open up the webcam
    cap = cv2.VideoCapture(0)
    while True:
        tk.update()
        # Read from the webcam 
        ret, frame = cap.read()

        # visualize it in a cv window
        cv2.imshow("Original_Image", frame)
        cv2.waitKey(3)

        lower_threshold = l_b.get()
        upper_threshold = u_b.get()
        grayIm = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

        cannyIm = cv2.Canny(grayIm, lower_threshold, upper_threshold, apertureSize = 3)
        cv2.imshow("Canny_Image", cannyIm)
        cv2.waitKey(3)
        time.sleep(0.02)

if __name__=='__main__':
    main()
