/*
完成以下图像处理操作：
图像锐化
伽马变换
灰度化
Canny边缘检测
图像拼接
图像融合
纹理迁移
人脸美颜（瘦脸、磨皮）
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;//创建主窗口对象
    w.show();//显示主窗口

    return a.exec();
}
