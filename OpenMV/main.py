import time, sensor, image, ustruct
from image import SEARCH_EX, SEARCH_DS
from pyb import UART,LED
one=LED(1)
#从imgae模块引入SEARCH_EX和SEARCH_DS。使用from import仅仅引入SEARCH_EX,
#SEARCH_DS两个需要的部分，而不把image模块全部引入。
clock = time.clock()
#重启传感器
sensor.reset()
#初始化传感器
sensor.set_contrast(1)      #设置对比度
sensor.set_gainceiling(16)  #设定增益曲线
sensor.set_framesize(sensor.QQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)
#sensor.skip_frames(2000) #跳过两秒等待稳定  不加
#串口3定义
uart1 = UART(3,115200,bits=8, parity=None, stop=1)
cross_detected = False
#初始模板
templatedemo = ["/1.pgm", "/2.pgm", "/3.pgm", "/4.pgm","/5.pgm", "/6.pgm", "/7.pgm", "/8.pgm"]
##分类模板  暂定12张
templates = (["/33.pgm", "/34.pgm", "/36.pgm","/306.pgm","/311.pgm","/312.pgm"], #ok
            ["/42.pgm", "/43.pgm", "/44.pgm","/45.pgm", "/403.pgm","/410.pgm"],  #ok
            ["/53.pgm", "/55.pgm", "/501.pgm","/504.pgm","/507.pgm","/508.pgm"], #待定
            ["/64.pgm","/65.pgm", "/602.pgm","/606.pgm","/603.pgm","/604.pgm"],  #ok
            ["/75.pgm", "/76.pgm","/701.pgm","/702.pgm","/706.pgm","/707.pgm"],  #ok
            ["/85.pgm","/82.pgm", "/801.pgm","/803.pgm","/804.pgm","/806.pgm"])  #8\


imgarray=[0,0,0,0,0,0,0,0]  #初始八个数据模板
imgarrays=[0,0,0,0,0,0]     #单个数字模板
first=0                     #初识别数字


"""
sending_data部分由于使用了两个openmv
因此只需要帧头帧尾不同
stm32单片机即可知道是从那边接收而来
"""
def sending_data(value):
    global uart1
    data_str = str(value) + '\n'  # 将整数转换为字符串并附加换行符
    datasum = data_str.encode()  # 将字符串（包括换行符）编码为字节串
    uart1.write(datasum)  # 发送字节串


"""
病房号识别输入，返回存储一到八图片的模组
"""
def get_first(img):
    global imgarray
    i=0
    for t in imgarray:
        r = img.find_template(t, 0.60, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
        i+=1
        if r:
#            print(r)
            return i
    return 0

"""
判断是否输入成功
如若成功，则openmv上的led灯
将对应亮相应的次数
"""
send_flag = 0;
def isget(num):
    if num is not 0:
        for i in range (num):
            one.on()
#            print(num) #使用时删除 给自己看的
            time.sleep_ms(300)
            one.off()
            time.sleep_ms(300)
        sending_data(num + 1000)
        print(num)


"""
传入对应的数字匹配模组
将会导入对应的图片
如数字7，那么会导入数字7对应的图片
"""
def getimgs(imgarr):
    i=0
    for t in imgarr:
        imgarrays[i]=image.Image((t))
        i+=1
    return imgarrays

"""
模板匹配，将当前帧数据与模板数据
比对，如果存在，则框出对应位置，
同时调用串口发送给单片机
"""

def match_img(photos, img):
    for template in photos:
        # 调整匹配阈值和搜索参数以优化匹配效果
        r = img.find_template(template, 0.5, step=4, search=SEARCH_EX) # 提高匹配阈值至0.5
        if r:
            img.draw_rectangle(r)
            # 计算中心点位置
            cx = r[0] + r[2] // 2
            cy = r[1] + r[3] // 2
            img.draw_cross(cx, cy)

            # 判断位置，引入容错范围，避免微小偏差导致的错误判断
            deviation = cx - img.width() // 2
            threshold = 10 # 容错阈值，根据实际情况调整
            if deviation < -threshold:
                print("在中线左边")
                sending_data(888)  # 假设发送888代表在左边.左转
            elif deviation > threshold:
                print("在中线右边")
                sending_data(777)  # 假设发送777代表在右边。右转
            else:
                print("接近中心")
                # 根据需要决定是否需要发送特定数据表示卡片接近中心
            return  # 找到匹配后就返回，不再继续查找



#def match_img(photos, img):
#    for template in photos:
#        r = img.find_template(template, 0.5, step=4, search=SEARCH_EX)
#        if r:
#            img.draw_rectangle(r)
#            # 计算中心点位置
#            cx = r[0] + r[2] // 2
#            cy = r[1] + r[3] // 2
#            img.draw_cross(cx, cy)

#            # 判断位置
#            deviation = cx - img.width() // 2
#            if deviation < 0:
#                print("在中线左边")
#                sending_data(888)  # 假设发送-1代表在左边.左转
#            else:
#                print("在中线右边")
#                sending_data(777)  # 假设发送1代表在右边。右转
#            return  # 找到匹配后就返回，不再继续查找

def preprocess_image(img):
    # 应用Canny边缘检测
    edge_img = img.find_edges(image.EDGE_CANNY, threshold=(50, 80))
    return edge_img
"""
初始八张图片的导入函数
"""
def loadimg():
    i=0
    for t in templatedemo:
        imgarray[i]=image.Image(str(t))
        i+=1
    return imgarray

cross_detected = False
detection_delay_counter = 0  # 用于冷却的计数器
DETECTION_DELAY = 30  # 设置一个阈值，例如30次循环后才能再次识别新的十字
send_counter = 0  # 初始化发送计数器
send_threshold = 15  # 设置发送频率的阈值，例如每10帧发送一次
def detect_cross(img):
    global cross_detected, detection_delay_counter

    if detection_delay_counter > 0:  # 如果处于冷却状态
        detection_delay_counter -= 1  # 减少冷却计数
        return False  # 在冷却期间，不检测十字，直接返回False

    lines = img.find_lines(threshold=1000, theta_margin=25, rho_margin=25)
    horizontal_lines = [line for line in lines if abs(line.theta() - 0) < 15 or abs(line.theta() - 180) < 15]
    vertical_lines = [line for line in lines if abs(line.theta() - 90) < 15]

    if len(horizontal_lines) > 0 and len(vertical_lines) > 0:
        if not cross_detected:  # 如果之前未检测到十字
            cross_detected = True  # 更新状态为已检测到十字
            detection_delay_counter = DETECTION_DELAY  # 重置冷却计数器
            return True  # 这是从未检测到到检测到的状态变化，返回True
        # 如果之前已经检测到了十字，这次还是检测到，不需要发送信号，返回False
    else:
        cross_detected = False  # 如果没有检测到十字，重置状态为未检测到
    return False

#-------------------------------程序执行部分----------------------------
imgarray=loadimg()                              #加载初始八张图
detected_number = False  # 添加一个标志位，初始为False，表示还没有识别到数字

while (True):
    clock.tick()                                    #时钟初始化
    img = sensor.snapshot()                         #获取当前帧
    if imgarrays[0]!=0:                             #且当已经加载好了多模板库后只会执行当前if
        match_img(imgarrays,img)                    #比对当前帧图片与加载进来的模板库匹配
        # 使用颜色追踪来标记路径，这里假设路径是黑色的
        threshold = [(0, 0, 0)] # 根据实际情况调整阈值
        blobs = img.find_blobs(threshold, pixels_threshold=100, area_threshold=100, merge=True)

        if blobs:
            # 假设最大的blob是我们要跟踪的路径
            largest_blob = max(blobs, key=lambda b: b.area())
            # 绘制找到的路径
            img.draw_rectangle(largest_blob.rect(), color=127)
            img.draw_cross(largest_blob.cx(), largest_blob.cy(), color=127)

            # 计算偏差：图像中点 - 路径中心点的X坐标
            deviation = largest_blob.cx() - img.width() // 2
            print("Deviation:", deviation)

            # 根据偏差发送控制指令
            sending_data(deviation)

#        if detect_cross(img):  # 当从未检测到十字变为检测到十字时
#            # 也可以对发送十字检测信号使用类似的频率控制
#            sending_data(999)  # 只在这种情况下发送信号
#            print("Cross detected and signal sent.")

    else:
        if first==0:                                #获取初始图片标志位
            first=get_first(img)                    #first用于判断当前是那张图片
            isget(first)                            #亮灯提醒是否识别到初始图片
        elif imgarrays[0]==0:                       #确定图片缓冲区第一次是空
            imgarrays=getimgs(templates[first-3])   #load photo 加载图片 执行一次


    #print("FPS:",clock.fps())                      #帧率显示



    # 在代码的最后部分，处理斜率和十字检测
