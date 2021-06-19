import cv2
import numpy as np


cv_image=cv2.imread('sudoku.png',1)

cv2.imshow("Original_Image", cv_image)
cv2.waitKey(3) 
    
# Convert the image to gray scale
gray = cv2.cvtColor(cv_image,cv2.COLOR_BGR2GRAY)  # Convert to grascale image
cv2.imshow("Gray_Image", gray)
cv2.waitKey(3) 

# Convert the image to binary with edges detected
lower_threshold = 0#TO DO
upper_threshold = 1500#TO DO
edges = cv2.Canny(gray,lower_threshold,upper_threshold,apertureSize = 3)   # Canny edge detector to make it easier for hough transform to "agree" on lines
#cv2.imshow("Canny_Image", edges)
#cv2.waitKey(3) 

min_intersections = 10000  # TO DO: Play with this parameter to change sensitivity.
lines = cv2.HoughLines(edges,1,np.pi/180,min_intersections)     # Run Hough Transform
for line in lines:                         # Plot lines over original feed
    for rho,theta in line:
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a*rho
        y0 = b*rho
        x1 = int(x0 + 1000*(-b))
        y1 = int(y0 + 1000*(a))
        x2 = int(x0 - 1000*(-b))
        y2 = int(y0 - 1000*(a))
        cv2.line(cv_image,(x1,y1),(x2,y2),(0,0,255),2)
        
cv2.imshow("Line_Detected_Image", cv_image)
cv2.waitKey(0)
print("Detecting Lines...")