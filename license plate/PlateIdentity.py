import cv2
import os
import sys
import numpy as np
import tensorflow as tf
import skimage.io as io
import skimage.color as color
import skimage.morphology as morphology
import skimage.feature as feature
import skimage.measure as measure
import math
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()
##svm
##def deskew(img):
##	m = cv2.moments(img)
##	if abs(m['mu02']) < 1e-2:
##		return img.copy()
##	skew = m['mu11']/m['mu02']
##	M = np.float32([[1, skew, -0.5*SZ*skew], [0, 1, 0]])
##	img = cv2.warpAffine(img, M, (SZ, SZ), flags=cv2.WARP_INVERSE_MAP | cv2.INTER_LINEAR)
##	return img
###来自opencv的sample，用于svm训练
##def preprocess_hog(digits):
##	samples = []
##	for img in digits:
##		gx = cv2.Sobel(img, cv2.CV_32F, 1, 0)
##		gy = cv2.Sobel(img, cv2.CV_32F, 0, 1)
##		mag, ang = cv2.cartToPolar(gx, gy)
##		bin_n = 16
##		bin = np.int32(bin_n*ang/(2*np.pi))
##		bin_cells = bin[:10,:10], bin[10:,:10], bin[:10,10:], bin[10:,10:]
##		mag_cells = mag[:10,:10], mag[10:,:10], mag[:10,10:], mag[10:,10:]
##		hists = [np.bincount(b.ravel(), m.ravel(), bin_n) for b, m in zip(bin_cells, mag_cells)]
##		hist = np.hstack(hists)
		
##		# transform to Hellinger kernel
##		eps = 1e-7
##		hist /= hist.sum() + eps
##		hist = np.sqrt(hist)
##		hist /= norm(hist) + eps
		
##		samples.append(hist)
##	return np.float32(samples)
##class SVM(StatModel):
##	def __init__(self, C = 1, gamma = 0.5):
##		self.model = cv2.ml.SVM_create()
##		self.model.setGamma(gamma)
##		self.model.setC(C)
##		self.model.setKernel(cv2.ml.SVM_RBF)
##		self.model.setType(cv2.ml.SVM_C_SVC)
###训练svm
##	def train(self, samples, responses):
##		self.model.train(samples, cv2.ml.ROW_SAMPLE, responses)
###字符识别
##	def predict(self, samples):
##		r = self.model.predict(samples)
##		return r[1].ravel()

#char_table = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
#              'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '川', '鄂', '赣', '甘', '贵',
#              '桂', '黑', '沪', '冀', '津', '京', '吉', '辽', '鲁', '蒙', '闽', '宁', '青', '琼', '陕', '苏', '晋',
#              '皖', '湘', '新', '豫', '渝', '粤', '云', '藏', '浙']


#def pre_process(orig_img):

#    gray_img = cv2.cvtColor(orig_img, cv2.COLOR_BGR2GRAY)
#    #cv2.imshow('gray_img', gray_img)
#    GaussianBlur_img = cv2.GaussianBlur(gray_img, (3, 3),0)
#    #cv2.imwrite('GaussianBlur_img.png', GaussianBlur_img)
#    #blur_img = cv2.blur(gray_img, (3, 3))
#    #cv2.imwrite('blur.png', blur_img)
#    #cv2.namedWindow('blur', cv2.WINDOW_NORMAL)
#    #cv2.imshow('blur', blur_img)   
#    #cv2.waitKey()
#    #ret, binary_img = cv2.threshold(gray_img, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
#    #canny_img=cv2.Canny(GaussianBlur_img,0.3,0.5)
#    #cv2.imshow('canny', canny_img) 
#    #cv2.imwrite('canny_img1.png', canny_img)
#    sobel_img = cv2.Sobel(GaussianBlur_img, cv2.CV_16S, 1, 0, ksize=3)
#    sobel_img = cv2.convertScaleAbs(sobel_img)
#    #cv2.namedWindow('sobel', cv2.WINDOW_NORMAL)
#    #cv2.imshow('sobel', sobel_img)
#    #cv2.imwrite('sobel.png', sobel_img)

#    hsv_img = cv2.cvtColor(orig_img, cv2.COLOR_BGR2HSV)

#    h, s, v = hsv_img[:, :, 0], hsv_img[:, :, 1], hsv_img[:, :, 2]
#    # 黄色[26，34],蓝色[100,124]
#    blue_img = (((h > 26) & (h < 34)) | ((h > 100) & (h < 124))) & (s > 70) & (v > 70)
#    blue_img = blue_img.astype('float32')
#    #cv2.imwrite('blue_imghsv.png', blue_img)
#    mix_img = np.multiply(sobel_img, blue_img)
#    #cv2.imwrite('mix2.png', mix_img)
#    mix_img = mix_img.astype(np.uint8)

#    ret, binary_img = cv2.threshold(mix_img, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
#    #cv2.imwrite('binary_img.png', binary_img)
#    #cv2.namedWindow('binary', cv2.WINDOW_NORMAL)
#    #cv2.imshow('binary',binary_img)   
#    #cv2.waitKey()
#    kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(22,7))
#    close_img = cv2.morphologyEx(binary_img, cv2.MORPH_CLOSE, kernel)
#    #cv2.imwrite('close_img2.png', close_img)

#    return close_img

## 车牌定位
#def locate_carPlate(orig_img,pred_image):
#    carPlate_list = []
#    contours,heriachy = cv2.findContours(pred_image,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)#cloneImg,
#    #if not len(contours):#contours1长度为0说明未检测到车牌
#    #    print("未检测到车牌")
#    #else:
#    #for i,contour in enumerate(contours):
#     #   cv2.drawContours(temp1_orig_img, contours, i, (255, 0, 0), 2)

#    #contours, hierarchy = cv2.findContours(img_mask[:, :, 0], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
#    if not len(contours):#contours1长度为0说明未检测到车牌
#        print("未检测到车牌")
#    else:
#        flag=0
#        for ii,cont in enumerate(contours):
#            x, y, w, h = cv2.boundingRect(cont)#获取最小外接矩形
#            img_cut_mask=pred_image[y:y+h,x:x+w]
#            if np.mean(img_cut_mask)>=75 and w>15 and h>15:
#                rect = cv2.minAreaRect(cont)
#                box = cv2.boxPoints(rect).astype(np.int32)#获取最小外接矩形四个顶点坐标
#                img1=cv2.drawContours(pred_image, contours, -1, (0, 0, 255), 2)
#                #cv2.imwrite('img1.png',img1)
#                img_contour=cv2.drawContours(pred_image, [box], 0, (0, 255, 0), 2)
#                #cv2.imwrite('img_contour.png',img_contour)

 
#                cont=cont.reshape(-1,2).tolist()
#                #转换矩阵[左上，左下，右上，右下]
#                box=sorted(box,key=lambda xy:xy[0])
#                box_left,box_right=box[:2],box[2:]#box的前2个是左侧的坐标，后2个是右侧的坐标
#                box_left=sorted(box_left,key=lambda x:x[1])#再按照上下即y进行排序，此时box_left中为左上和左下两个端点坐标
#                box_right=sorted(box_right,key=lambda x:x[1])
#                box=np.array(box_left+box_right)#[左上，左下，右上，右下]
 
#                x0,y0=box[0][0],box[0][1]#最小外接矩形的四个坐标
#                x1,y1=box[1][0],box[1][1]
#                x2,y2=box[2][0],box[2][1]
#                x3,y3=box[3][0],box[3][1]
 
#                def point_to_line_distance(X,Y):
#                    if x2-x0:
#                        k_up=(y2-y0)/(x2-x0)
#                        d_up=abs(k_up*X-Y+y2-k_up*x2)/(k_up**2+1)**0.5
#                    else:
#                        d_up=abs(X-x2)
#                    if x1-x3:
#                        k_down=(y1-y3)/(x1-x3)
#                        d_down=abs(k_down*X-Y+y1-k_down*x1)/(k_down**2+1)**0.5
#                    else:
#                        d_down=abs(X-x1)
#                    return d_up,d_down
 
#                d0,d1,d2,d3=np.inf,np.inf,np.inf,np.inf
#                l0,l1,l2,l3=(x0,y0),(x1,y1),(x2,y2),(x3,y3)
#                for each in cont: #计算cont中的坐标与矩形四个坐标的距离以及到上下两条直线的距离，对距离和进行权重的添加，选出四边形的4个顶点坐标
#                    x,y=each[0],each[1]
#                    dis0=(x-x0)**2+(y-y0)**2
#                    dis1=(x-x1)**2+(y-y1)**2
#                    dis2=(x-x2)**2+(y-y2)**2
#                    dis3=(x-x3)**2+(y-y3)**2
#                    d_up,d_down=point_to_line_distance(x,y)
#                    weight=0.975
#                    if weight*d_up+(1-weight)*dis0<d0:
#                        d0=weight*d_up+(1-weight)*dis0
#                        l0=(x,y)
#                    if weight*d_down+(1-weight)*dis1<d1:
#                        d1=weight*d_down+(1-weight)*dis1
#                        l1=(x,y)
#                    if weight*d_up+(1-weight)*dis2<d2:
#                        d2=weight*d_up+(1-weight)*dis2
#                        l2=(x,y)
#                    if weight*d_down+(1-weight)*dis3<d3:
#                        d3=weight*d_down+(1-weight)*dis3
#                        l3=(x,y)
 
#                p0 = np.float32([l0,l1,l2,l3])#左上角，左下角，右上角，右下角，形成的新box顺序需和原box中的顺序对应，以进行转换矩阵的形成
#                p1= np.float32([(0,0),(0,80),(240,0),(240,80)])
#                transform_mat=cv2.getPerspectiveTransform(p0,p1)#构成转换矩阵
#                lic=cv2.warpPerspective(orig_img,transform_mat,(240,80))#进行车牌矫正
#                cv2.imwrite('lic.png',lic)
#                flag+=1
#                #cv2.namedWindow('lic', cv2.WINDOW_NORMAL)
#                #cv2.imshow('lic',lic)
#                #cv2.waitKey(0)
#                #if len(contours)==1:#只有一个区域可以认为是车牌区域
#                #    flag+=1
#                #    #print('saving ',save_path+name[0:7]+'.png')
#                #    cv2.imwrite('lic.png',lic)
#                #    cv2.namedWindow('lic', cv2.WINDOW_NORMAL)
#                #    cv2.imshow('lic',lic)
#                #    cv2.waitKey(0)
#                #    #cv2.imencode('.png',lic)[1].tofile(save_path+name[0:7]+'.png')
 
#        if not flag:
#            print("未检测到车牌区域或车牌区域过小")
#        #car_plate = cv2.resize(lic,(car_plate_w,car_plate_h)) #调整尺寸为后面CNN车牌识别做准备
#        cv2.imwrite('car_plate.png',lic)
 
#    return lic

#def in_bboxes(bbox, bboxes):
#    for bb in bboxes:
#        minr0, minc0, maxr0, maxc0 = bb
#        minr1, minc1, maxr1, maxc1 = bbox
#        if minr1 >= minr0 and maxr1 <= maxr0 and minc1 >= minc0 and maxc1 <= maxc0:
#            return True
#    return False

#def extract_char(car_plate):
#    gray_plate = cv2.cvtColor(car_plate,cv2.COLOR_BGR2GRAY)
#    blur_char = cv2.GaussianBlur(gray_plate,(3,3),0)
    
    
#    #ret, binary_char = cv2.threshold(blur_char, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
#    #ret,binary_plate = cv2.threshold(gray_plate,0,255,cv2.THRESH_BINARY|cv2.THRESH_OTSU)#binary_plate
#    #char_img_list = get_chars(binary_plate)
#    #sobel_char = cv2.Sobel(gray_plate, cv2.CV_16S, 1, 0, ksize=3)
#    #sobel_char = cv2.convertScaleAbs(sobel_char)
#    #cv2.imshow('canny', img3) 
#    canny_char=cv2.Canny(blur_char,50,150)
#    cv2.imwrite('canny_char.png', canny_char)
#    #kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(4,5))
#    #close_img = cv2.dilate(canny_char, kernel,1)
#    #close_img = cv2.morphologyEx(canny_char, cv2.MORPH_CLOSE, kernel)
#    #cv2.imwrite('close_img.png', close_img)
#    label_img = measure.label(canny_char)
#    regions = measure.regionprops(label_img)
#    bboxes = []
#    for props in regions:
#        y0, x0 = props.centroid
#        minr, minc, maxr, maxc = props.bbox
    
#        if maxc - minc > canny_char.shape[1] / 7 or maxr - minr < canny_char.shape[0] / 3:
#            continue
        
#        bbox = [minr, minc, maxr, maxc]
#        if in_bboxes(bbox, bboxes):
#            continue
        
#        if abs(y0 - canny_char.shape[0] / 2) > canny_char.shape[0] / 4:
#            continue
        
#        bboxes.append(bbox)
    
#        bx = (minc, maxc, maxc, minc, minc)
#        by = (minr, minr, maxr, maxr, minr)
#        #ax.plot(bx, by, '-r', linewidth=2)
#    bboxes = sorted(bboxes, key=lambda x: x[1])
#    chars = []
#    for bbox in bboxes:
#        minr, minc, maxr, maxc = bbox
#        ch = gray_plate[minr:maxr, minc:maxc]
#        ret, char_img= cv2.threshold(ch, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
#        chars.append(char_img)
#        io.imsave('char.png',char_img)
        
#    #cv2.imwrite('canny_char.png', canny_char)
#    #dilate_char=cv2.dilate(canny_char, 3, 1)
#    #cv2.namedWindow('dilate_char', cv2.WINDOW_NORMAL)
#    #cv2.imshow('dilate_char', dilate_char) 
#    #cv2.waitKey(0)
#    return chars

#def cnn_select_carPlate(plate_list,model_path):
#    if len(plate_list) == 0:
#        return False,plate_list
#    g1 = tf.Graph()
#    sess1 = tf.Session(graph=g1)
#    with sess1.as_default():
#        with sess1.graph.as_default():
#            model_dir = os.path.dirname(model_path)
#            saver = tf.train.import_meta_graph(model_path)
#            saver.restore(sess1, tf.train.latest_checkpoint(model_dir))
#            graph = tf.get_default_graph()
#            net1_x_place = graph.get_tensor_by_name('x_place:0')
#            net1_keep_place = graph.get_tensor_by_name('keep_place:0')
#            net1_out = graph.get_tensor_by_name('out_put:0')

#            input_x = np.array(plate_list)
#            net_outs = tf.nn.softmax(net1_out)
#            preds = tf.argmax(net_outs,1) #预测结果
#            probs = tf.reduce_max(net_outs,reduction_indices=[1]) #结果概率值
#            pred_list,prob_list = sess1.run([preds,probs],feed_dict={net1_x_place:input_x,net1_keep_place:1.0})
#            # 选出概率最大的车牌
#            result_index,result_prob = -1,0.
#            for i,pred in enumerate(pred_list):
#                if pred==1 and prob_list[i]>result_prob:
#                    result_index,result_prob = i,prob_list[i]
#            if result_index == -1:
#                return False,plate_list[0]
#            else:
#                return True,plate_list[result_index]

#def cnn_recongnize_char(img_list,model_path):
#    g2 = tf.Graph()
#    sess2 = sess = tf.compat.v1.Session(graph=g2)
#    text_list = []

#    if len(img_list) == 0:
#        return text_list
#    with sess2.as_default():
#        with sess2.graph.as_default():
#            model_dir = os.path.dirname(model_path)
#            saver = tf.train.import_meta_graph(model_path)
#            saver.restore(sess2, tf.train.latest_checkpoint(model_dir))
#            graph = tf.get_default_graph()
#            net2_x_place = graph.get_tensor_by_name('x_place:0')
#            net2_keep_place = graph.get_tensor_by_name('keep_place:0')
#            net2_out = graph.get_tensor_by_name('out_put:0')

#            data = np.array(img_list,dtype=object)
#            # 数字、字母、汉字，从67维向量找到概率最大的作为预测结果
#            net_out = tf.nn.softmax(net2_out)
#            preds = tf.argmax(net_out,1)
#            #preds.dtype=tf.float32
#            #my_preds=object
#            net2_x_place = tf.placeholder(tf.float32)
#            net2_keep_place = tf.placeholder(tf.float32)
#            my_preds= sess2.run(preds, feed_dict={net2_x_place:data, net2_keep_place: 1.0})

#            for i in my_preds:
#                text_list.append(char_table[i])
#            return text_list

#if __name__ == '__main__':
#    cur_dir = sys.path[0]
#    car_plate_w,car_plate_h = 136,36
#    char_w,char_h = 20,20
#    plate_model_path = os.path.join(cur_dir, 'D://University/CarPlateIdentity-master/code/model/plate_recongnize/model.ckpt-510.meta')
#    char_model_path = os.path.join(cur_dir,'D://University/CarPlateIdentity-master/code/model/model.ckpt-620.meta')
#    img = cv2.imread('D://University//simple-car-plate-recognition-master//1.jpg')

#    # 预处理
#    pred_img = pre_process(img)

#    # 车牌定位提取
#    car_plate_list = locate_carPlate(img,pred_img)

#    # 字符提取
#    char_img_list = extract_char(car_plate_list)

#    # CNN字符识别
#    text = cnn_recongnize_char(char_img_list,char_model_path)
#    print(text)

#    cv2.waitKey(0)

def CannyThreshold(lowThreshold):
    detected_edges = cv2.GaussianBlur(gray,(3,3),0)
    detected_edges = cv2.Canny(detected_edges,lowThreshold,lowThreshold*ratio,apertureSize = kernel_size)
    dst = cv2.bitwise_and(img,img,mask = detected_edges)  # just add some colours to edges from original image.
    cv2.imshow('canny demo',dst)

lowThreshold = 0
max_lowThreshold = 100
ratio = 3
kernel_size = 3

img = cv2.imread('D://University//CarPlateIdentity-master//canny_char.png')
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
img = cv2.GaussianBlur(img,(3,3),0)
cv2.namedWindow('canny demo')

cv2.createTrackbar('Min threshold','canny demo',lowThreshold, max_lowThreshold, CannyThreshold)

CannyThreshold(0)  # initialization
if cv2.waitKey(0) == 27:
    cv2.destroyAllWindows()

