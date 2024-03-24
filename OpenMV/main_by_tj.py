# AprilTags Example
#
# This example shows the power of the OpenMV Cam to detect April Tags
# on the OpenMV Cam M7. The M4 versions cannot detect April Tags.

#V1
"""
import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.skip_frames(30)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()
    for tag in img.find_apriltags(families=image.TAG25H9): # defaults to TAG36H11 without "families".
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        degress = 180 * tag.rotation() / math.pi
        print(tag.id(),degress)
"""


#V2
# AprilTags Example
#
# This example shows the power of the OpenMV Cam to detect April Tags
# on the OpenMV Cam M7. The M4 versions cannot detect April Tags.
import ustruct
import sensor, image, time, math, json
import pyb
from pyb import UART,LED
from time import sleep
uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters

threshold_index = 0 # 0 for red, 1 for green, 2 for blue
color_id = [0, 1, 2]#分别代表红 绿 蓝
color_points = []
thresholds = [(13, 99, 19, 120, -24, 56),#(26, 67, 14, 127, -43, 64),#(47, 91, 10, 121, -73, 127),#(27, 80, 10, 93, -29, 103),#(22, 100, 16, 87, -42, 100), # generic_red_thresholds  (0,100,14,127,-4,127),#
               (91, 100, -21, 127, -44, 100),(26, 100, -82, -22, -55, 127),#(85, 100, -100, -23, -16, 127),#(67, 100, -84, -30, -14, 127),#(75, 100, -128, 127, -36, 127), # generic_green_thresholds
              (0, 100, 14, 101, -30, 127)] # generic_blue_thresholds

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.HQVGA) # we run out of memory if the resolution is much bigger...
#sensor.skip_frames(30)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
#sensor.set_auto_exposure(False,7000)#在这里调节曝光度，调节完可以比较清晰地看清激光点
#sensor.set_windowing(20, 20, 100, 90)
#sensor.set_windowing(80,20,200,200)
sensor.set_brightness(0)   #设置亮度
sensor.set_contrast(0) #对比度
sensor.skip_frames(time = 20)       # 跳过帧
#sensor.set_gainceiling(2)   #增益上限
#sensor.set_auto_gain(False,gain_db=-1) #增益
#sensor.set_auto_exposure(False,2000)  #曝光速度

clock = time.clock()

# c_x 是图像的x中心位置
# c_y 是图像的y中心位置

f_x = (2.8 / 3.984) * 160 # 默认值
f_y = (2.8 / 2.952) * 120 # 默认值
c_x = 160 * 0.5 # 默认值(image.w * 0.5)
c_y = 120 * 0.5 # 默认值(image.h * 0.5)


'''
全局变量
'''
pencil_points = [[67, 30], [248, 16], [294, 216], [37, 229], [166, 110]]  #铅笔画的方形的定点坐标和中心点坐标，共5组
laser_on_pencil_servo_value = [[102.243, 72.6351], [79.8784, 71.8379], [79.5676, 96.3784], [102.689, 96.5541], [91.6757, 84.8243], [102.392, 72.3919], [79.7973, 71.7027], [79.6081, 95.6487], [102.851, 96.2297], [91.3379, 84.4189]] #激光在铅笔痕迹上时，舵机的角度。用于校准
taruge_rect_in_QQVGA = []   #A4纸黑线的定点坐标列表
function_running_num = 0 #当前运行功能选择
red_threshold = [(15, 75, 20, 90, 0, 60)]   #红色光点阈值
green_threshold = [(50, 100, -65, -15, -15, 40)]    #绿色光点阈值
laser_threshold=[(0, 99, 6, 120, -24, 56)]#[(15, 75, 20, 90, 0, 60)]#[(85, 100, -20, 20, -20, 20)]   #激光颜色阈值
black_line_threshold=[(0, 30, -20, 20, -20, 20)]   #激光颜色阈值

def degrees(radians):
    return (180 * radians) / math.pi

#两点之间线性插值,num为插值点数
def linear_interpolation(arr, p1, p2, num):
    x1 = p1[0]
    y1 = p1[1]
    x2 = p2[0]
    y2 = p2[1]
    if (x2-x1) == 0:
        k = (y2 - y1)/10000
    else:
        k = (y2 - y1)/(x2-x1)
    for i in range(num + 1):
        x0 = x1 + i*(x2-x1)/num
        y0 = y1 +k*(x0 - x1)
        temp = [x0, y0]
        arr.append(temp)#添加
def linear_interpolation1(arr, x1, y1, x2, y2, num):
    if (x2-x1) == 0:
        k = (y2 - y1)/10000
    else:
        k = (y2 - y1)/(x2-x1)
    for i in range(num + 1):
        x0 = x1 + i*(x2-x1)/num
        y0 = y1 +k*(x0 - x1)
        temp = [x0, y0]
        arr.append(temp)#添加


def find_color(color_id):
    for blob in img.find_blobs([thresholds[color_id]], pixels_threshold=1, area_threshold=1, merge=True):
        # These values depend on the blob not being circular - otherwise they will be shaky.
        if blob.elongation() <= 1.0:#返回一个介于0和1之间的值，该值表示对象的长度（不是圆形）。一条线将是1
            color_points.append([blob.cx(), blob.cy()])#每个点的信息都是一个子列表
#            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,0,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,0))
            print("\nblob", blob)
            print("blob.code()", blob.code()) #一帧产生多个图片会有多个code  震惊!竟然具有跟踪功能 就像deepsort那样
            print("blob.rect()", blob.rect())# x y w h(47, 47, 2, 2)
            print("blob.cx(), blob.cy()", blob.cx(), blob.cy())#区域中心点坐标 47 47
            break#找到一个就不找了
        # These values are stable all the time.
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy(),color=(0, 0, 0))
        # Note - the blob rotation is unique to 0-180 only.
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)


def sending_data(cx,cy,cw,ch):
    global uart;
    #frame=[0x2C,18,cx%0xff,int(cx/0xff),cy%0xff,int(cy/0xff),0x5B];
    #data = bytearray(frame)
    data = ustruct.pack("<bbhhhhb",      #格式为俩个字符俩个短整型(2字节)
                   0x2C,                      #帧头1
                   0x12,                      #帧头2
                   int(cx), # up sample by 4   #数据1
                   int(cy), # up sample by 4    #数据2
                   int(cw), # up sample by 4    #数据1
                   int(ch), # up sample by 4    #数据2
                   0x5B)
    uart.write(data);   #必须要传入一个字节数组

#rects.corners())坐标点元组 eg:((40, 105), (109, 100), (105, 7), (37, 1))

rect_flag = 0
rect_index = 0
Function_num = 0
while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.5)     #进行畸形矫正
    if uart.any():
        a = uart.read(1).decode() #uart.read()为一个字节串，加.decode() 变成字符串
        if a == '1':
            Function_num = 1
            LED(1).off()
            LED(2).off()
            LED(3).off()
            LED(1).on()
            print('reset mode')
        elif a == '2':
            Function_num = 2
            LED(1).off()
            LED(2).off()
            LED(3).off()
            LED(2).on()
            print('calibration mode')
        elif a == '3':
            Function_num = 3
            LED(1).off()
            LED(2).off()
            LED(3).off()
            LED(3).on()
            print('A4 tracking mode')
        elif a == '4':
            Function_num = 4
            sensor.set_brightness(-3)   #设置亮度
            sensor.set_contrast(3) #对比度
            sensor.set_auto_exposure(False,1000)  #曝光速度
            LED(1).off()
            LED(2).off()
            LED(3).off()
            LED(1).on()
            LED(2).on()
            print('A4 tracking mode')

    if Function_num == 2:#校准功能
#        global pencil_points,laser_threshold
        pencil_points = []  #先清空原有数据
        linear_arr = []#线性插值得到的数组，四个边首尾相连
        sensor.set_brightness(-3)   #设置亮度
        sensor.set_contrast(3) #对比度
        sensor.set_auto_exposure(False,3000)  #曝光速度
        loop = True
        while loop:
            img = sensor.snapshot()
            blobs = img.find_blobs(laser_threshold)
            if blobs:
                img.draw_cross(blobs[0].cx(),blobs[0].cy(),color=(0,0,0))
                print(blobs[0].cx(),blobs[0].cy())
                sending_data(blobs[0].cx(),blobs[0].cy(),0,0)
                if uart.any():
                    a = uart.read(1).decode() #uart.read()为一个字节串，加.decode() 变成字符串
                    if a == 'R':
                        if len(pencil_points)<5:#少于5组数据，未输入完成
                            #坐标追加如
                            pencil_points.append([blobs[0].cx(),blobs[0].cy()])
                            LED(1).on()
                            LED(2).on()
                            LED(3).on()
                            sleep(0.5)
                            LED(1).off()
                            LED(2).off()
                            LED(3).off()
                            LED(2).on()
                            print('point remember!')
                        else: #等于5组数据，输入完成
                            loop = False
                            #优化了插值数量，与矩形长度关联起来
                            linear_interpolation(linear_arr, pencil_points[0], pencil_points[1], 10)
                            linear_interpolation(linear_arr, pencil_points[1], pencil_points[2], 10)
                            linear_interpolation(linear_arr, pencil_points[2], pencil_points[3], 10)
                            linear_interpolation(linear_arr, pencil_points[3], pencil_points[0], 10)
                            print("linear_arr:", linear_arr)
                            print('point finished!')
#                screen.get_touch()  #获取触屏触摸
#                if screen.touch_exist():    #如果触摸存在
#                    if len(pencil_points)<5:#少于5组数据，未输入完成
#                        #坐标追加如
#                        pencil_points.append([blobs[0].cx(),blobs[0].cy()])
##                        buzzer.frequency(500)
##                        sleep(0.5)
##                        buzzer.frequency(0)
#                    else: #等于5组数据，输入完成
#                        loop = False
            for n in range(len(pencil_points)):
                img.draw_cross(pencil_points[n][0],pencil_points[n][1],color=(0,0,255))
                img.draw_string(pencil_points[n][0],pencil_points[n][1],str(n),color=(0,0,255))

#        global laser_threshold,pencil_points,laser_on_pencil_servo_value,laser_threshold,pencil_points

        sensor.set_brightness(-3)   #设置亮度
        sensor.set_contrast(3) #对比度
        sensor.set_auto_exposure(False,3000)  #曝光速度
#        servo.channel_on(servo_rotation)   #打开两个舵机信号
#        servo.channel_on(servo_pitch)
#        laser_move_by_degress(90,90)

        laser_point_stable_n = 0 #用于判断激光点是否稳定的计数

        move_pencil_step_tot = 30 #每个边细分成多少份进行移动
        move_pencil_point_n = 0  #临时存储变量，用于存储当前执行到铅笔方形的哪个角
        move_pencil_step_n = 0  #临时存储变量，用于存储在当前边的第几步。步总数参考上述变量。
        laser_calibration_enter_mode = True #录入模式，停止移动。录入完成后继续移动。
        loop = True
        while loop:
            color_points = []
            img = sensor.snapshot()
            if rect_index==len(linear_arr)-1:
                rect_index = 0
            #颜色识别
            #需要用到的变量  color_points  eg:[[42, 113], [44, 109]] 第一个为红色的坐标，第二个为绿色的坐标
            print("#####边线识别开始#####")
            print(len(linear_arr))
            print("index:",rect_index)
            find_color(color_id[0])#判断红色
            if(color_points):#如果找到红色,则继续找绿色,否则退出
                if abs(color_points[0][0] - int(linear_arr[rect_index][0])) < 6 and abs(color_points[0][1] - int(linear_arr[rect_index][1])) < 6:
                    rect_index += 1
                if color_points[0][0]>300 or color_points[0][1]>300 or linear_arr[rect_index][0]>300 or linear_arr[rect_index][1]>300:#((color_points[0][0]-linear_arr[rect_index][0])**2+(color_points[0][1]-linear_arr[rect_index][1])**2)>25 and rect_index>6:
                    continue;
                else:
                    print(linear_arr[rect_index][0],linear_arr[rect_index][1],color_points[0][0],color_points[0][1])
                    sending_data(-linear_arr[rect_index][0],-linear_arr[rect_index][1],-color_points[0][0],-color_points[0][1])
                    img.draw_cross(int(linear_arr[rect_index][0]), int(linear_arr[rect_index][1]),color=(0, 0, 255))

#            screen.display(img)
    elif Function_num == 3:#追踪A4纸


        #img.lens_corr(1.5)     #进行畸形矫正
        color_points = []#每帧清零,防止一直append
        #linear_arr = []#线性插值得到的数组，四个边首尾相连


        #矩形识别+线性插值得到数组
        #需要用到的变量  rects.corners()矩形的四个点,长度为4 [0]左下 [1]右下 [2]右上 [3]左上  eg:((36, 77), (124, 64), (115, 3), (28, 10))
        #linear_arr 线性插值得到的数组，首尾相连，长度不确定  eg:[[36.0, 77.0], [44.8, 75.7], [53.6, 74.4], [62.4, 73.1], [71.2, 71.8]......
        print("#####矩形识别开始#####")
        if rect_flag <20:
            linear_arr = []#线性插值得到的数组，四个边首尾相连
            img.midpoint(1, bias=0.6, threshold=True, offset=5, invert=True)
            for rects in img.find_rects(threshold = 50000):
                img.draw_rectangle(rects.rect(), color = (255, 0, 0))#红色
    #            for points in rects.corners():
    #                img.draw_circle(points[0], points[1], 2, color = (0, 0, 255))#蓝色
                print("rects:", rects)#rects: {"x":39, "y":4, "w":72, "h":101, "magnitude":49142}
                print("points:", rects.corners())#points: ((41, 105), (111, 101), (105, 4), (39, 5))
                print("rects.rect", rects.rect())#返回检测到的矩形的外接长方形的(x, y, w, h)

                #优化了插值数量，与矩形长度关联起来
                linear_interpolation(linear_arr, rects.corners()[3], rects.corners()[2], 5)
                linear_interpolation(linear_arr, rects.corners()[2], rects.corners()[1], 5)
                linear_interpolation(linear_arr, rects.corners()[1], rects.corners()[0], 5)
                linear_interpolation(linear_arr, rects.corners()[0], rects.corners()[3], 5)
                print("linear_arr:", linear_arr)
                rect_flag = rect_flag+1
                break#只找一个矩形
        elif rect_index<len(linear_arr):

            if rect_index==len(linear_arr)-1:
                rect_index = 0
            #颜色识别
            #需要用到的变量  color_points  eg:[[42, 113], [44, 109]] 第一个为红色的坐标，第二个为绿色的坐标
            print("#####颜色识别开始#####")
            print(len(linear_arr))
            print("index:",rect_index)
            find_color(color_id[0])#判断红色
            if(color_points):#如果找到红色,则继续找绿色,否则退出
                if ((color_points[0][0]-linear_arr[rect_index][0])**2+(color_points[0][1]-linear_arr[rect_index][1])**2)<9:
                    rect_index = rect_index+1
                if color_points[0][0]>300 or color_points[0][1]>300 or linear_arr[rect_index][0]>300 or linear_arr[rect_index][1]>300:#((color_points[0][0]-linear_arr[rect_index][0])**2+(color_points[0][1]-linear_arr[rect_index][1])**2)>25 and rect_index>6:
                    continue;
                else:
                    print(linear_arr[rect_index][0],linear_arr[rect_index][1],color_points[0][0],color_points[0][1])
                    sending_data(-linear_arr[rect_index][0],-linear_arr[rect_index][1],-color_points[0][0],-color_points[0][1])
                    img.draw_cross(int(linear_arr[rect_index][0]), int(linear_arr[rect_index][1]),color=(255, 0, 0))
            else:
                color_points = []#每帧清零,防止一直append
                find_color(color_id[1])#判断混合色
                if(color_points):#如果找到红色,则继续找绿色,否则退出
                    if ((color_points[0][0]-linear_arr[rect_index][0])**2+(color_points[0][1]-linear_arr[rect_index][1])**2)<9:
                        rect_index = rect_index+1
                    if color_points[0][0]>300 or color_points[0][1]>300 or linear_arr[rect_index][0]>300 or linear_arr[rect_index][1]>300:#((color_points[0][0]-linear_arr[rect_index][0])**2+(color_points[0][1]-linear_arr[rect_index][1])**2)>25 and rect_index>6:
                        continue;
                    else:
                        print(linear_arr[rect_index][0],linear_arr[rect_index][1],color_points[0][0],color_points[0][1])
                        sending_data(-linear_arr[rect_index][0],-linear_arr[rect_index][1],-color_points[0][0],-color_points[0][1])
                        img.draw_cross(int(linear_arr[rect_index][0]), int(linear_arr[rect_index][1]),color=(255, 0, 0))

    elif Function_num == 4:#追踪A4纸
        print("#####红色激光跟踪开始#####")
        color_points = []#每帧清零,防止一直append
        find_color(color_id[1])#判断红色
        if(color_points):#如果找到红色,则继续找绿色,否则退出
            find_color(color_id[2])#判断绿色
        if(len(color_points) == 2):
            print("同时找到红色和绿色,本次为有效判断")
            print("color_points:", color_points)
            print("\n")
            if color_points[0][0]>300 or color_points[0][1]>300 or color_points[1][0]>300 or color_points[1][1]>300:#((color_points[0][0]-linear_arr[rect_index][0])**2+(color_points[0][1]-linear_arr[rect_index][1])**2)>25 and rect_index>6:
                continue;
            else:
                print(-color_points[1][0],-color_points[1][1],-color_points[0][0],-color_points[0][1])
                sending_data(-color_points[1][0],-color_points[1][1],-color_points[0][0],-color_points[0][1])

#    print(clock.fps())







