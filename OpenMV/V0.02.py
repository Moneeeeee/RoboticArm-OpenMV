# LOTS OF Blob Detection
import sensor, image, time, pyb

# 如果要保证颜色追踪效果的话， 需要对环境的严格控制
# 晚上光源的冷暖色等，都会对颜色追踪造成很大的影响

# 彩色图片颜色的阈值格式组成， 是由LAB颜色空间的各自最小值与最大值组成
# 点击右侧的颜色空间下拉选择按钮， 默认为RGB Color Space
# 参考右侧的LAB Color Space里面的参数
# （minL, maxL, minA, maxA, minB, maxB）
# 灰度图的阈值格式
# (min, max)

# 红色阈值=
pink_threshold =(42, 100, 7, 127, 5, 75)
#yellow_threshold = (36, 75, -20, 11, 23, 48)
## 蓝色阈值
#blue_threshold = (61, 95, -23, -10, -30, -10)
## 绿色阈值
#green_threshold =(50, 60, -48, -30, 15, 38)

# 颜色阈值的设定可以在 工具(Tools) -> 机器视觉(Machine Vision) -> 阈值编辑器(Threshold Editor) 中调试

# 颜色代码是find_blobs返回的blob对象中的一个成分， 用于标识，该色块是由在哪个阈值下选择的
# 颜色1: 红色的颜色代码
pink_color_code = 1 # code = 2^0 = 1
## 颜色2: 绿色的颜色代码
#yellow_color_code = 2 # code = 2^1 = 2
## 颜色3蓝的代码
#blue_color_code = 4# color_code_3 = 2^2 = 4
## 颜色4绿的代码
#green_color_code = 8# color_code_4 = 2^3 = 8


sensor.reset() # 初始化摄像头
sensor.set_pixformat(sensor.RGB565) # 选择像素模式 RGB565.
sensor.set_framesize(sensor.QVGA) # use QQVGA for speed.
sensor.skip_frames(time = 2000) # Let new settings take affect.
sensor.set_auto_whitebal(False) #关闭白平衡。白平衡是默认开启的，在颜色识别中，需要关闭白平#衡。#

clock = time.clock() # Tracks FPS.
img_width = sensor.width()
img_height = sensor.height()

# 初始化串口
uart = pyb.UART(3, 115200)

global_center_x = img_width / 2
global_center_y = img_height / 2


#应该也使得检测到的矩形框大小一样。这样能确保视深！
while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # 拍照，返回图像
    # 在图像中寻找满足颜色阈值约束(color_threshold, 数组格式), 像素阈值pixel_threshold， 色块面积大小阈值(area_threshold)的色块
    blobs = img.find_blobs([pink_threshold], area_threshold=300)
    if blobs:
    #如果找到了目标颜色
        for blob in blobs:
        #迭代找到的目标颜色区域
            x = blob[0]
            y = blob[1] #
            width = blob[2] # 色块矩形的宽度
            height = blob[3] # 色块矩形的高度

            center_x = blob[5] # 色块中心点x值
            center_y = blob[6] # 色块中心点y值

            color_code = blob[8] # 颜色代码

            # 添加颜色说明
            if color_code == pink_color_code:
                img.draw_string(x, y - 10, "red", color = (0xFF, 0x00, 0x00))

            # 计算矩形边框的长和宽
            rect_width = width
            rect_height = height

            #用矩形标记出目标颜色区域
            img.draw_rectangle([x, y, width, height])
            #print(center_x - global_center_x,y - img_height)
            uart.write("{},{},{},{}\n".format(rect_width, rect_height, center_x - global_center_x, center_y - global_center_y))
            #在目标颜色区域的中心画十字形标记
            img.draw_cross(center_x, center_y)



    #print(clock.fps())

