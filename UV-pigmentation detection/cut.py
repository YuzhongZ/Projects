import cv2
import os
import numpy as np
import copy

points = []


def on_mouse(event,x,y,flags,param):
    global points, img, Cur_point, Start_point
    copyImg = copy.deepcopy(img)
    h,w = img.shape[:2]
    mask_img = np.zeros([h+2,w+2],dtype=np.uint8)
    if  event == cv2.EVENT_LBUTTONDOWN:
        Start_point = [x,y]
        points.append(Start_point)
        cv2.circle(img,tuple(Start_point),1,(255,255,255),0)
        cv2.imshow("",img)
    elif event == cv2.EVENT_MOUSEMOVE and flags == cv2.EVENT_FLAG_LBUTTON:
        Cur_point = [x,y]
        # print(points)
        cv2.line(img,tuple(points[-1]),tuple(Cur_point),(255,255,255))
        cv2.imshow("",img)
        points.append(Cur_point)
    elif event == cv2.EVENT_LBUTTONUP:
        Cur_point=Start_point
        cv2.line(img,tuple(points[-1]),tuple(Cur_point),(255,255,255))
        cv2.circle(img,tuple(Cur_point),1,(255,255,255))
        ret, image, mask, rect  = \
        cv2.floodFill(img,mask_img,(x,y),(255,255,255),cv2.FLOODFILL_FIXED_RANGE)
       # cv2.imwrite("maskImage.jpg",img)
        print(np.shape(image))
        segImg = np.zeros((h,w,3),np.uint8)
        src =cv2.bitwise_and(img,image)
       # cv2.imwrite("segImg.jpg",src)
        cv2.waitKey(0)
        img = cv2.imread('segImg.jpg')
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        # ret, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY_INV)
        # opencv
        ret, binary = cv2.threshold(gray, 240, 255, cv2.THRESH_BINARY)
        contours, hierarchy = cv2.findContours(binary, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        contours.sort(key=lambda c: cv2.contourArea(c), reverse=True)
        cv2.drawContours(copyImg, contours, -1, (0, 0, 255), 3)

        cv2.namedWindow("RoiImg", 0)
        cv2.resizeWindow("RoiImg", 600, 1000)
        cv2.imshow('RoiImg', copyImg)
        cv2.waitKey(0)

        cimg = np.zeros_like(img)
        cimg[:, :, :] = 255
        cv2.drawContours(cimg, contours, 1, color=(0, 0, 0), thickness=-1)

        cv2.namedWindow("maskImg", 0)
        cv2.resizeWindow("maskImg", 600, 1000)
        cv2.imshow('maskImg', cimg)
        cv2.waitKey(0)

        final = cv2.bitwise_or(copyImg, cimg)
        localimg = final
        cv2.imwrite("New_input.jpg", final)
        cv2.namedWindow("finalImg", 0)
        cv2.resizeWindow("finalImg", 600, 1000)
        cv2.imshow('finalImg', final)
        cv2.waitKey(0)
        return final


if __name__ == "__main__":
    path = 'input2_output1.png'
    img = cv2.imread(path)

    cv2.namedWindow("",0)
    cv2.resizeWindow("", 600, 1000)
    cv2.setMouseCallback("", on_mouse, 0)
    cv2.imshow("", img)

    cv2.waitKey(0)
    cv2.destroyAllWindows()