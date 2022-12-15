import cv2
import numpy as np

img = cv2.imread(binarylzx.png)  # 读取要进行开运算的图像
open_img = cv2.morphologyEx(img,cv2.MORPH_OPEN,kernel=np.ones((5,5),np.uint8))
remove== cv2.morphologyEx(open_img ,cv2.MORPH_CLOSE,kernel=np.ones((5,5),np.uint8))
cv2.imshow('original_close',binarylzx.png)
cv2.imshow('remove',remove)
cv2.moveWindow("original_close", 1000, 400)
cv2.waitKey(0)
cv2.destroyAllWindows()


