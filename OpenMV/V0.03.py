import sensor, image, time, pyb
import ustruct

# 最大颜色阈值
pink_threshold = (27, 58, -16, 24, 17, 58)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_whitebal(False)

clock = time.clock()
img_width = sensor.width()
img_height = sensor.height()

# 初始化串口
uart = pyb.UART(3, 115200)

global_center_x = img_width / 2

global_center_y = img_height / 2


def sending_data(cx,cy,cw,ca,cb,cc):
    global uart;
    #frame=[0x2C,18,cx%0xff,int(cx/0xff),cy%0xff,int(cy/0xff),0x5B];
    #data = bytearray(frame)
    data = ustruct.pack("<bbhhhhhhb",      #格式为俩个字符俩个短整型(2字节)
                        0x5b,                      #帧头1
                        0x38,                      #帧头2
                        int(cx), # up sample by 4   #数据1
                        int(cy), # up sample by 4    #数据2
                        int(cw), # up sample by 4    #数据1
                        int(ca), # up sample by 4    #数据1
                        int(cb), # up sample by 4    #数据1
                        int(cc), # up sample by 4    #数据1
                        #                   int(ch), # up sample by 4    #数据2
                        0xb5)
    uart.write(data);   #必须要传入一个字节数组
K = 1000
Cross_Flag = 0
deviation = 0

def detect_cross(img):
    """
    检测图像中的十字形状。
    返回一个布尔值，指示是否检测到十字。
    """
    global cross_detected
    lines = img.find_lines(threshold=1000, theta_margin=25, rho_margin=25)
    horizontal_lines = [line for line in lines if abs(line.theta() - 0) < 15 or abs(line.theta() - 180) < 15]
    vertical_lines = [line for line in lines if abs(line.theta() - 90) < 15]

    # 如果同时存在水平线和垂直线，认为检测到十字
    if len(horizontal_lines) > 0 and len(vertical_lines) > 0:
        if not cross_detected:  # 如果之前未检测到十字
            cross_detected = True  # 更新状态为已检测到十字
            return True  # 这是从未检测到到检测到的状态变化，返回True
        # 如果之前已经检测到了十字，这次还是检测到，不需要发送信号，返回False
    else:
        cross_detected = False  # 如果没有检测到十字，重置状态为未检测到
    return False
while True:
    clock.tick()
    img = sensor.snapshot()

    # 寻找最大颜色阈值的色块
    max_blob = None
    max_area = 0

    blobs = img.find_blobs([pink_threshold], area_threshold=500)
    if blobs:
        for blob in blobs:
            if blob[4] > max_area:
                max_blob = blob
                max_area = blob[4]

    if max_blob:
        x, y, width, height = max_blob[0:4]
        center_x = max_blob[5]
        center_y = max_blob[6]
        color_code = max_blob[8]

        b = max_blob
        Lm = (b[2]+b[3])/2
        length = K/Lm


        # 添加颜色说明
        if color_code == 1:  # 红色
            img.draw_string(x, y - 10, "red", color=(0xFF, 0x00, 0x00))

        # 用矩形标记最大颜色阈值的色块
        img.draw_rectangle([x, y, width, height])

        rect_width = width
        rect_height = height

        sending_data(0,center_x - global_center_x,length,Cross_Flag,deviation,0)
        print(center_x - global_center_x)
        #        uart.write("".format(rect_width, rect_height, center_x - global_center_x, center_y - global_center_y))
        img.draw_cross(center_x, center_y)

    if detect_cross(img):  # 当从未检测到十字变为检测到十字时
        #        sending_data("command", 9   )  # 只在这种情况下发送信号
        print("Cross detected and signal sent.")

    # print(clock.fps())
