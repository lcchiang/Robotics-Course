#!/usr/bin/python

# 2.12 Lab 7 object detection: a node for adjusting gamma
# Luke Roberto Oct 2017
# Jacob Guggenheim 2019
# Jerry Ng 2019, 2020


import numpy as np
import cv2  # OpenCV module
from matplotlib import pyplot as plt
import time
import math
from tkinter import *

global tk
tk = Tk()
global g
g= Scale(tk, from_ = 0.01, to = 10, label = 'Gamma', orient = HORIZONTAL, resolution = 0.01)
g.pack()
def main():
    # Open up the webcam
    cap = cv2.VideoCapture(0)

    while True:
        ret, cv_image = cap.read()
        tk.update()
        # visualize it in a cv window
        cv2.imshow("Original_Image", cv_image)
        cv2.waitKey(3)

        # gamma adjust
        gamma = g.get()
        gamma_im = adjust_gamma(cv_image, gamma=gamma)
        cv2.imshow("Adjusted Gamma", gamma_im)
        cv2.waitKey(3)

def adjust_gamma(image, gamma=1.0):

   invGamma = 1.0 / gamma
   table = np.array([((i / 255.0) ** invGamma) * 255
      for i in np.arange(0, 256)]).astype("uint8")

   return cv2.LUT(image, table)

if __name__=='__main__':
    main()
