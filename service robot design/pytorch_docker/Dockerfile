#基础镜像（基础镜像中已安装python3，python3-pip， curl，cmake-3.10.2，torch-1.8.1，torchaudio-0.8.1， torchvision-0.9.1，scikit-build-0.11.1, Cython-0.29.22, matplotlib-3.3.4, scipy-1.5.4, tensorboard-2.4.1, tqdm4.60.0, boto3-1.17.47, Flask-1.1.2, thop-0.0.31, PyYAML-5.4.1, opencv-python-4.5.1.48, opencv-contrib-python-4.5.1.48）

FROM ccr.cloudminds.com/library/pytorch:1.8.1-cuda10.2-cudnn7-runtime-ubuntu18.04-libs

#将编译好的算法、模型和脚本拷贝到镜像
ADD ./requirements.txt /opt/requirements.txt
ADD ./yolov5 /opt/yolov5
ADD ./run_nohup.sh /opt/run_nohup.sh
#添加相应的环境变量
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8
#安装依赖库
RUN pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple -r /opt/requirements.txt
#添加启动脚本为可执行权限
RUN chmod u+x /opt/run_nohup.sh

#设置启动脚本
CMD ["/opt/run_nohup.sh"]