import sensor, image, time, math, pyb

# 定义红色小球的颜色阈值，可能需要根据你的环境调整
# LAB颜色空间的红色阈值示例
red_threshold = (30, 100, 15, 127, 15, 127)

# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_whitebal(False)  # 关闭白平衡

# 初始化串口
uart = pyb.UART(3, 115200)

while(True):
    img = sensor.snapshot()

    # 查找满足红色阈值的色块
    blobs = img.find_blobs([red_threshold], pixels_threshold=100, area_threshold=100, merge=True)
    if blobs:
        # 假设最大的色块是我们的目标小球
        largest_blob = max(blobs, key=lambda b: b.pixels())
        # 计算小球的中心坐标
        ball_center_x = largest_blob.cx()
        ball_center_y = largest_blob.cy()

        # 绘制小球的中心
        img.draw_cross(ball_center_x, ball_center_y, color=(255, 0, 0))

        # 计算图像的全局中心坐标
        global_center_x = img.width() // 2
        global_center_y = img.height() // 2

        # 计算差值
        delta_x = ball_center_x - global_center_x
        delta_y = ball_center_y - global_center_y

        # 通过串口发送差值数据
        uart.write("Delta X: %d, Delta Y: %d\n" % (delta_x, delta_y))

    time.sleep(100)
