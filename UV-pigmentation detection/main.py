import cv2
import os
import numpy as np
import copy



def edge_demo(image):
    blurred = cv2.GaussianBlur(image, (3, 3), 0)

    gray = cv2.cvtColor(blurred, cv2.COLOR_BGR2GRAY)

    xgrad = cv2.Sobel(gray, cv2.CV_16SC1, 1, 0)  # xGrodient
    ygrad = cv2.Sobel(gray, cv2.CV_16SC1, 0, 1)  # yGrodient
    edge_output = cv2.Canny(xgrad, ygrad, 100, 150)  # edge


    return edge_output

def contours_demo(image):
    binary = edge_demo(image)
    OriImg_1 = cv2.imread("input2_output1.png")
    contours, heriachy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    for i, contour in enumerate(contours):
        cv2.drawContours(OriImg_1, contours, i, (0, 255, 255), 6)

    cv2.namedWindow("detect contours", 0)
    cv2.resizeWindow("detect contours", 600, 1000)
    cv2.imshow("detect contours", OriImg_1)
    cv2.imwrite("result_final.png", OriImg_1)


if __name__ == "__main__":
    LocalImg = cv2.imread("remove.png")
    OriImg = cv2.imread("input2_output1.png")
    print(OriImg.shape)
    print(LocalImg.shape)
    LocalImg = cv2.resize(LocalImg, (3648, 5472))
    LocalImg[LocalImg < 255] = 0
    src = LocalImg
    print(LocalImg.shape)
    contours_demo(src)

    cv2.waitKey(0)
    cv2.destroyAllWindows()



