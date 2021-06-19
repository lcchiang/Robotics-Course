#!/usr/bin/python

# 2.12 Lab 9
# Luke Roberto Oct 2017
# Jerry Ng 2019, 2020

import time
import numpy as np
import cv2  # OpenCV module
import math
from tkinter import *


global tk
tk = Tk()
global lower, upper, inters
lower = Scale(tk, from_ = 1, to = 1500, label = 'lower threshold', orient = HORIZONTAL)
lower.pack()
upper = Scale(tk, from_ = 1, to = 1500, label = 'upper threshold', orient = HORIZONTAL)
upper.pack()
upper.set(1500)
inters = Scale(tk, from_ = 1, to = 1000, label = 'Min Intersections', orient = HORIZONTAL)
inters.pack()
inters.set(50)

def main():
    # Open up the webcam
    cap = cv2.VideoCapture(0)
    while True:
        tk.update()
        # Read from the webcam, frame by frame
        ret, cv_image = cap.read()

        # 2. visualize it in a cv window
        cv2.imshow("Original_Image", cv_image)
        cv2.waitKey(3) 
        
        # 3. Hough Transform
        gray = cv2.cvtColor(cv_image,cv2.COLOR_BGR2GRAY)  # Convert to grascale image
        

        #TODO: Play with these parameters
        lower_threshold = lower.get()
        upper_threshold = upper.get()
        edges = cv2.Canny(gray,lower_threshold,upper_threshold,apertureSize = 3)   # Canny edge detector to make it easier for hough transform to "agree" on lines

        min_intersections = inters.get()                           # TO DO: Play with this parameter to change sensitivity.
        lines = cv2.HoughLines(edges,1,np.pi/180,min_intersections)     # Run Hough Transform
        num_lines = 0;
        if lines is not None:
            shape = lines.shape

            for i in range(shape[0]):                         # Plot lines over original feed
                for rho,theta in lines[i]:
                    a = np.cos(theta)
                    b = np.sin(theta)
                    x0 = a*rho
                    y0 = b*rho
                    x1 = int(x0 + 1000*(-b))
                    y1 = int(y0 + 1000*(a))
                    x2 = int(x0 - 1000*(-b))
                    y2 = int(y0 - 1000*(a))
                    cv2.line(cv_image,(x1,y1),(x2,y2),(0,0,255),2)
                    num_lines += 1
                
        cv2.imshow("Line_Detected_Image", cv_image)
        cv2.waitKey(3)
        time.sleep(0.01)    

if __name__=='__main__':
    main()
