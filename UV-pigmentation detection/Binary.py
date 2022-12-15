import cv2 as cv
import os
import numpy as np
import copy


def watershed_algorithm(image):
    blur = cv.pyrMeanShiftFiltering(image,sp=10,sr=100)
    gray = cv.cvtColor(blur, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    cv.namedWindow("binary image", 0)
    cv.resizeWindow("binary image", 600, 1000)
    cv.imshow('binary image', binary)
    cv.imwrite('binary.png', binary)


src = cv.imread("input2_output1.png")
src = cv.resize(src, None, fx=0.5, fy=0.5)

cv.namedWindow("input image", 0)
cv.resizeWindow("input image", 600, 1000)
cv.imshow('input image', src)

watershed_algorithm(src)
cv.waitKey(0)
cv.destroyAllWindows()