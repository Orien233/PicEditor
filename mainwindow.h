#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr); //主窗口类的构造函数
    ~MainWindow();  //主窗口类的析构函数

    QImage MattoQImage(const cv::Mat &mat); //将OpenCV Mat转换为QImage
    cv::Mat QImagetoMat(const QImage &image); //将QImage转换为OpenCV Mat
    QImage ImageCenter(QImage  qimage,QLabel *qLabel); //图像居中函数
    void ShowImage(QImage qimage,QLabel *qLabel);

    QImage GrayScale(QImage image); //灰度化函数
    QImage CannyEdgeDetection(QImage image, double lowThreshold, double highThreshold); //Canny边缘检测函数
    QImage Sharpen(QImage image,int level); //图像锐化函数
    QImage GammaTransform(QImage image, int level); //伽马变换函数

    QImage Stitching(QImage image1, QImage image2); //图像拼接函数
    QImage Fusion(QImage image1, QImage image2); //图像融合函数
    QImage TextureTransfer(QImage source, QImage target); //纹理迁移函数
    QImage SkinSmoothing(QImage image,int level); //磨皮函数
    QImage FaceSlimming(QImage image,int level); //瘦脸函数

    QStringList srcImagePaths; //源图像路径列表
    int ImageIndex = 0; //当前图像索引

private slots:
    void on_Action_Load_triggered(); //打开图像文件槽函数
    void on_Action_Save_triggered(); //保存图像文件槽函数

    void on_IfGrayscale_clicked(); //灰度化按钮点击槽函数
    void on_IfEdgeDetect_clicked(); //Canny边缘检测按钮点击槽函数

    void on_GammaCorrectionSlider_valueChanged(int value); //滑动条值改变槽函数
    void on_ImageSharpeningSlider_valueChanged(int value); //锐化滑动条值改变槽函数
    void on_SkinSmoothingSlider_6_valueChanged(int value); //磨皮滑动条值改变槽函数
    void on_FaceSlimmingSlider_7_valueChanged(int value); //瘦脸滑动条值改变槽函数

    void on_ImageSitchingButton_clicked(); //图像拼接按钮点击槽函数
    void on_ImageFusionButton_clicked(); //图像融合按钮点击槽函数
    void on_ImageTextureTransferButton_clicked(); //纹理迁移按钮点击槽函数

    void disableAllSliders(); //禁用所有滑动条函数
    void enableAllSliders(); //启用所有滑动条函数
    void disableAllFaceBeautySliders(); //禁用所有美容滑动条函数
    void enableAllFaceBeautySliders(); //启用所有美容滑动条函数
    void disableAllRadioButtons(); //禁用所有单选按钮函数
    void enableAllRadioButtons(); //启用所有单选按钮函数
    void disableAllPushButtons(); //禁用所有按钮函数
    void enableAllPushButtons(); //启用所有按钮函数
    
private:
    Ui::MainWindow *ui;
    QString currentImagePath;   //当前图像路径
    cv::Mat currentImage; // 当前图像的OpenCV Mat对象
    QImage currentQImage; // 当前图像的QImage对象
    QImage originalQImage; // 原始图像变量
    QImage resultQImage; // 处理后的图像变量
    std::vector<cv::Rect> faces; // 人脸检测结果

    bool isGrayscaleApplied;    // 灰度化状态
    bool isEdgeDetectApplied;   // 边缘检测状态

    static const QString FACE_CASCADE_PATH; // 人脸检测分类器路径
    static const QString FACE_MARKER_PATH;  // 人脸标记器路径
};

#endif // MAINWINDOW_H
