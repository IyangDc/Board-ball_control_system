# -*- coding: utf-8 -*-
import sys
from imutils.video.pivideostream import PiVideoStream
from imutils.video import FPS
from picamera.array import PiRGBArray
from picamera import PiCamera
import numpy as np
import imutils
import time
import cv2
import serial
import math
import matplotlib.pyplot as plt
import matplotlib.font_manager as font_manager
list=[1,8.9,0,3.7,8.9,0,3.7];
#for arg1 in sys.argv:
#	list.append(arg1)
#print(list)
#----------------------------------Draw circle-------
def b_circle(r):
    delta_sita = math.pi/50
    x = np.zeros(100)
    y = np.zeros(100)
    sita = 0
    for i in range(0,100):
	x[i] = int(r * math.cos(sita))
	y[i] = int(r * math.sin(sita))
	sita += delta_sita
    #plt.plot(x,y)
    #plt.show()	
    return x,y

#----------------------------------------------------------------------------------------对象声明
class model_ball:
#对象生成时，球的坐标和目标的坐标均初始化为0
    def __init__(self):
        self.point = 0
        self.set = 0
    #获得小球坐标
    def gain_pos(self,circle):
        self.point = circle
	#print('X_point :{}'.format(circle))
    #获得目标坐标
    def gain_set(self,set_1):
        self.set = set_1
#-------------------------------------------------------------------------------------------------------------------------
class PID(model_ball):
#初始化PID
    def __init__(self):
        self.P = 0
        self.I = 0
        self.D = 0
        #上一次的时间初始化为0
        self.time_p = 0
        #积分计算值
        self.c_I = 0.0
        self.err_pre1 = 0
    #设定参数值
    def set_Kp(self,P):
        self.P = P
    def set_Ki(self,I):
        self.I = I
    def set_Kd(self,D):
        self.D = D
    #继承误差
    def set_x_err_prev(self,err_current):
        self.err_pre1 = err_current
    def f1(self):
	self.c_I= 0
    #计算并输出控制量
    def out(self,model_ball):
        time.sleep(0.001)
        time_c = time.time()
        #计算当x,y坐标当前的误差
        err_c = model_ball.set - model_ball.point
        #print('err :{}'.format(err_c))
	#print('set :{}'.format(model_ball.set))
	#计算两次采样时间差
        dt = time_c - self.time_p
        print('Dt :{}'.format(dt))
        #计算两次采样微分值
        x_de = err_c - self.err_pre1     
        #将本次误差继承到下次
        self.set_x_err_prev(err_c)
        #将本次时间继承到下次
        self.time_p = time_c
        #计算微分值
        self.c_I += err_c * dt
	#print('P :{}'.format(self.P))
        #PID计算公式
        out = (((err_c * self.P) + (self.I * self.c_I) + (self.D * x_de / dt)))
        return (out,err_c)
#-------------------------------------------------------------------------------------------------------------------------生成对象
ball_x = model_ball()
ball_y = model_ball()
pid_x = PID()
pid_y = PID()

pid_x.set_Kp(float(list[1]))
pid_y.set_Kp(float(list[4]))

pid_x.set_Ki(float(list[2]))
pid_y.set_Ki(float(list[5]))

pid_x.set_Kd(float(list[3]))
pid_y.set_Kd(float(list[6]))




#initialize the camera and grab a reference to the raw cameracapture
camera = PiCamera()
camera.resolution = (320,320)
camera.framerate = 30
rawCapture = PiRGBArray(camera,size=(320,320))
stream = camera.capture_continuous(rawCapture,format="bgr",use_video=True,burst=True)
camera.close()

#FPS start
vs = PiVideoStream().start()
time.sleep(2.0)
fps = FPS().start()

params = cv2.SimpleBlobDetector_Params()
#change thresholds
params.minThreshold = 0
params.maxThreshold = 255

#fliter by color
params.filterByColor = True
params.blobColor = 0

#Setup the detector whith default parameters
detector = cv2.SimpleBlobDetector_create(params)

#init uart
port = serial.Serial("/dev/ttyUSB0", baudrate=115200,bytesize=8,parity="N",stopbits=1,timeout=0)
time.sleep(0.5)

#draw plot
#plt.close()
#fig=plt.figure()
#ax=fig.add_subplot(1,1,1)
#ax.axis("equal")
#plt.grid(True)
#plt.ion()
#xx=0
#yy=[]
#draw the circle
c_x,c_y = b_circle(250)
print(c_x)
print(c_y)
#time.sleep(10)
time1 = 0
flag2 = 1;
j = 0
while(True):
	time1 = time1 + 1
	#print('j :{}'.format(j))
#	if (time1 % 12)== 0:
        if (time1 % 7)== 0:
	    if flag2 == 1:
	       	ball_x.gain_set(c_x[j])
		ball_y.gain_set(c_y[j])
		#ball_x.gain_set(0)
                #ball_y.gain_set(0)
		j= j + 1
		if j == 100:
		    j = 0
	print('setx :{}'.format(ball_x.set))
        print('sety :{}'.format(ball_y.set))

	frame = vs.read()
	frame = imutils.resize(frame,width=160)
	#cv2.imshow("CameraStream",frame)
	#blob 
	keypoints = detector.detect(frame)
	if(keypoints):
	    for i in range (0,len(keypoints)):
		x = keypoints[i].pt[0]
		y = keypoints[i].pt[1]
		Position_x = int(x*10-800)
		Position_y = int(y*10-600)
		#Draw keypoints
    		#im_keypoints = cv2.drawKeypoints(frame,keypoints,np.array([]),(0,0.255),cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
		ball_x.gain_pos(Position_x)
		ball_y.gain_pos(Position_y)
		#Cal the control value
		s1,err_C1 = pid_x.out(ball_x)
		s2,err_C2 = pid_y.out(ball_y)
		if s1 > 10000:
                    pid_x.f1()
                    pid_y.f1()
		print('errx :{}'.format(err_C1))
		print('erry :{}'.format(err_C2))
		#print "out10-x: %2f :"%(s1)
		#print "out10-y: %2f :"%(s2)
		s1 = s1 + 1000.0
		s2 = s2 + 1000.0
		if s1 >= 2000:
		   s1 = 2000
		if s1 <= 0:
		   s1 = 0
		if s2 >= 2000:
		   s2 = 2000
		if s2 <= 0:
                   s2 = 0
		s2 = s2 /100
		s1 = s1 /100
		#uart sent
		Uart_buf = bytearray([255,int(s1),int(s2)])
		port.write(Uart_buf)
	        port.write("\r\n")
		#print "X: %.2f"%(Position_x)
		#print "Y: %.2f"%(Position_y)
		print "out10x: %2f :"%(s1)
		print "out10y: %2f :"%(s2)
		#ax.scatter(xx+10,err_C1,c='b',marker='.')
		#plt.pause(0.001)
	else:
	    Position_x = 0
	    Position_y = 0
            Uart_buf = bytearray([255,10,10])
            port.write(Uart_buf)
            port.write("\r\n")
	    print('X :{}'.format(Position_x))
	    print('Y :{}'.format(Position_y))
			#x_l = Circles_x & (0xff)
			#x_h = (Circles_x >> 8) & (0xff)
			
			#Uart_buf = bytearray([255,254,x_h,x_l,253,252])
			#print(' :{}'.format(Uart_buf))
			#send data by serial
			#port.write(Uart_buf)
#	cv2.circle(frame,im_keypoints,(255,0,0),2)
	#show keypoints
	#cv2.circle(frame,(int(ball_x.set+800),int(ball_y.set+600)),20,(255,0,0),3)
	cv2.imshow('circles',frame)
    	#cv2.imshow("Keypoints",frame)
	key = cv2. waitKey(1)&0xFF
	if key == ord("q"):
		break
	fps.update()
fps.stop()
print "[INFO] elasped time: {:.2f}".format(fps.elapsed())
print "[INFO] approx. FPS: {:.2F}".format(fps.fps())
cv2.destroyAllWindows()
vs.stop()

