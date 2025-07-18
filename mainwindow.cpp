#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <vector>
#include <QDebug>

using namespace cv;

const QString MainWindow::FACE_CASCADE_PATH = "E:/Zdragon/BJTU_ZA/PicEditor/build-PicandVideoEditor-Desktop_Qt_5_12_2_MinGW_64_bit-Debug/debug/haarcascade_frontalface_default.xml";
const QString MainWindow::FACE_MARKER_PATH = "E:/Zdragon/BJTU_ZA/PicEditor/build-PicandVideoEditor-Desktop_Qt_5_12_2_MinGW_64_bit-Debug/debug/lbfmodel.yaml";

MainWindow::MainWindow(QWidget *parent) : //构造函数
    QMainWindow(parent),//调用父类构造函数
    ui(new Ui::MainWindow)
{
     qDebug() << "MainWindow constructor start";

    ui->setupUi(this);

    // 设置窗口大小为屏幕的80%
    QDesktopWidget* desktop = QApplication::desktop();
    QRect screenGeometry = desktop->screenGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    setMinimumSize(800, 600); // 设置最小窗口大小
    resize(screenWidth * 0.8, screenHeight * 0.8);// 设置窗口大小为屏幕的80%
    setWindowTitle("PicEditor"); // 设置窗口标题

    // 设置按钮和滑动条的初始状态
    isGrayscaleApplied = false;
    isEdgeDetectApplied = false;
    ui->IfGrayscale->setCheckable(true);
    ui->IfEdgeDetect->setCheckable(true);
    ui->IfGrayscale->setAutoExclusive(false);
    ui->IfEdgeDetect->setAutoExclusive(false);
    ui->IfGrayscale->setChecked(false);
    ui->IfEdgeDetect->setChecked(false);

    ui->GammaCorrectionSlider->setRange(-50, 50);
    ui->GammaCorrectionSlider->setValue(0);
    ui->GammaCorrectionSlider->repaint();  // 强制重绘
    
    ui->ImageSharpeningSlider->setRange(-50, 50);
    ui->ImageSharpeningSlider->setValue(0);
    ui->ImageSharpeningSlider->repaint();  // 强制重绘
    
    ui->SkinSmoothingSlider_6->setValue(0);
    ui->SkinSmoothingSlider_6->repaint();  // 强制重绘
    
    ui->FaceSlimmingSlider_7->setRange(-50, 50);
    ui->FaceSlimmingSlider_7->setValue(0);
    ui->FaceSlimmingSlider_7->repaint();  // 强制重绘

    disableAllSliders();
    disableAllRadioButtons();
    disableAllPushButtons();

    qDebug() << "MainWindow constructor end";
}

MainWindow::~MainWindow() //析构函数
{
    delete ui;
}

void MainWindow::disableAllSliders() //禁用所有滑动条函数
{
    ui->GammaCorrectionSlider->setEnabled(false);
    ui->ImageSharpeningSlider->setEnabled(false);
    ui->SkinSmoothingSlider_6->setEnabled(false);
    ui->FaceSlimmingSlider_7->setEnabled(false);
}

void MainWindow::enableAllSliders() //启用所有滑动条函数
{
    ui->GammaCorrectionSlider->setEnabled(true);
    ui->ImageSharpeningSlider->setEnabled(true);
    ui->SkinSmoothingSlider_6->setEnabled(true);
    ui->FaceSlimmingSlider_7->setEnabled(true);
}

void MainWindow::disableAllFaceBeautySliders() //禁用所有美容滑动条函数
{
    ui->SkinSmoothingSlider_6->setEnabled(false);
    ui->FaceSlimmingSlider_7->setEnabled(false);
}

void MainWindow::enableAllFaceBeautySliders() //启用所有美容滑动条函数
{
    ui->SkinSmoothingSlider_6->setEnabled(true);
    ui->FaceSlimmingSlider_7->setEnabled(true);
}

void MainWindow::disableAllRadioButtons() //禁用所有单选按钮函数
{
    ui->IfGrayscale->setEnabled(false);
    ui->IfEdgeDetect->setEnabled(false);
}

void MainWindow::enableAllRadioButtons() //启用所有单选按钮函数
{
    ui->IfGrayscale->setEnabled(true);
    ui->IfEdgeDetect->setEnabled(true);
}

void MainWindow::disableAllPushButtons() //禁用所有按钮函数
{
    ui->ImageSitchingButton->setEnabled(false);
    ui->ImageFusionButton->setEnabled(false);
    ui->ImageTextureTransferButton->setEnabled(false);
}

void MainWindow::enableAllPushButtons() //启用所有按钮函数
{
    ui->ImageSitchingButton->setEnabled(true);
    ui->ImageFusionButton->setEnabled(true);
    ui->ImageTextureTransferButton->setEnabled(true);
}

void MainWindow::ShowImage(QImage image, QLabel *label) //显示图像函数
{
    if (image.isNull() || label == nullptr) 
    {
        QMessageBox::warning(this, "Error", "Image is null or label is invalid.");
        return;
    }

    // 设置label的缩放模式和对齐方式
    label->setScaledContents(false); // 不自动缩放内容
    label->setAlignment(Qt::AlignCenter); // 居中对齐
    
    // 计算合适的缩放尺寸
    QSize labelSize = label->size();
    QSize imageSize = image.size();
    
    // 计算缩放比例，保持宽高比
    double widthRatio = static_cast<double>(labelSize.width()) / imageSize.width();
    double heightRatio = static_cast<double>(labelSize.height()) / imageSize.height();
    double scaleFactor = qMin(widthRatio, heightRatio);
    
    // 缩放图像
    QSize scaledSize = imageSize * scaleFactor;
    QImage scaledImage = image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    label->setPixmap(QPixmap::fromImage(scaledImage));
}

QImage MainWindow::MattoQImage(const cv::Mat &mat) //将OpenCV Mat转换为QImage
{
    if (mat.empty()) 
    {
        return QImage();
    }

    switch (mat.type()) 
    {
        case CV_8UC1:
        {
            QImage qimg(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
            return qimg.copy();
        }
        
        case CV_8UC3:
        {
            QImage qimg(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return qimg.rgbSwapped(); // BGR转RGB
        }
        
        case CV_8UC4:
        {
            QImage qimg(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            return qimg.rgbSwapped(); // BGRA转ARGB，需要交换R和B通道
        }
        
        default:
        {
            qDebug() << "Mat type not supported:" << mat.type();
            // 尝试转换为支持的格式
            cv::Mat convertedMat;
            if (mat.channels() == 3) {
                mat.convertTo(convertedMat, CV_8UC3);
                QImage qimg(convertedMat.data, convertedMat.cols, convertedMat.rows, 
                           convertedMat.step, QImage::Format_RGB888);
                return qimg.rgbSwapped();
            } else if (mat.channels() == 4) {
                mat.convertTo(convertedMat, CV_8UC4);
                QImage qimg(convertedMat.data, convertedMat.cols, convertedMat.rows, 
                           convertedMat.step, QImage::Format_ARGB32);
                return qimg.rgbSwapped();
            }
            return QImage();
        }
    }
}

cv::Mat MainWindow::QImagetoMat(const QImage &image) //将QImage转换为OpenCV Mat
{
    if (image.isNull()) 
    {
        return cv::Mat();
    }

    QImage swapped;
    switch (image.format()) // 根据QImage的格式转换为OpenCV Mat
    {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
            swapped = image.rgbSwapped();
            return cv::Mat(swapped.height(), swapped.width(), CV_8UC4, 
                          (void*)swapped.constBits(), swapped.bytesPerLine()).clone();
            
        case QImage::Format_RGB888:
            swapped = image.rgbSwapped();
            return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, 
                          (void*)swapped.constBits(), swapped.bytesPerLine()).clone();
            
        case QImage::Format_Grayscale8:
            return cv::Mat(image.height(), image.width(), CV_8UC1, 
                          (void*)image.constBits(), image.bytesPerLine()).clone();
            
        default:
            // 转换为RGB888格式后再处理
            QImage convertedImage = image.convertToFormat(QImage::Format_RGB888);
            swapped = convertedImage.rgbSwapped();
            return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, 
                          (void*)swapped.constBits(), swapped.bytesPerLine()).clone();
    }
}

QImage MainWindow::GrayScale(QImage image) //灰度化函数
{
    if (image.isNull()) 
    {
        return QImage();
    }

    cv::Mat mat = QImagetoMat(image); // 将QImage转换为OpenCV Mat
    
    if (mat.empty()) 
    {
        QMessageBox::warning(nullptr, "Error", "Failed to convert QImage to Mat.");
        return QImage();
    }

    cv::Mat grayMat;// 创建一个灰度图像的Mat对象

    // 根据输入图像的通道数选择合适的转换方式
    if (mat.channels() == 4) 
    {
        cv::cvtColor(mat, grayMat, cv::COLOR_BGRA2GRAY);// 4通道转灰度
    }
    else if (mat.channels() == 3) 
    {
        cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);// 3通道转灰度
    }
    else if (mat.channels() == 1) 
    {
        grayMat = mat.clone(); // 已经是灰度图像
    }
    else 
    {
        QMessageBox::warning(nullptr, "Error", "Unsupported image format for grayscale conversion.");
        return QImage();
    }

    QImage grayImage = MattoQImage(grayMat);// 将灰度图像的Mat转换为QImage

    if (grayImage.isNull()) 
    {
        QMessageBox::warning(nullptr, "Error", "Failed to convert grayscale Mat to QImage.");
        return QImage();
    }

    return grayImage;
}

QImage MainWindow::Sharpen(QImage image, int value) //图像锐化函数
{
    if (image.isNull()) 
    {
        return QImage();
    }

    if (value == 0) {
        return image; // 值为0时，返回原图
    }

    cv::Mat mat = QImagetoMat(image); // 将QImage转换为OpenCV Mat
    cv::Mat processedMat;

    // 修改：处理正负值范围
    if (value > 0) {
        // 正值：锐化效果
        float intensity = value * 0.1f; // 锐化强度
        float kernelData[9] = {
            0, -1, 0,
            -1, 5.0f + intensity, -1,
            0, -1, 0
        };
        
        cv::Mat kernel = cv::Mat(3, 3, CV_32F, kernelData);
        cv::filter2D(mat, processedMat, -1, kernel);
        
    } else {
        // 负值：高斯模糊效果
        int blurStrength = abs(value);
        int kernelSize = 3 + (blurStrength / 10) * 2; // 根据强度调整核大小
        if (kernelSize % 2 == 0) kernelSize++; // 确保为奇数
        
        double sigma = 0.5 + blurStrength * 0.05; // 标准差
        cv::GaussianBlur(mat, processedMat, cv::Size(kernelSize, kernelSize), sigma);
    }

    QImage sharpenedImage = MattoQImage(processedMat);
    return sharpenedImage;
}

QImage MainWindow::GammaTransform(QImage image, int value) //伽马变换函数
{
    if (image.isNull()) 
    {
        return QImage();
    }

    cv::Mat mat = QImagetoMat(image); // 将QImage转换为OpenCV Mat
    
    cv::Mat gammaMat;// 创建一个伽马变换后的Mat对象

    // 修改：处理正负值范围
    double gamma;
    if (value == 0) {
        return image; // 值为0时，返回原图
    } else if (value > 0) {
        gamma = 1.0 + value * 0.04; // 正值：1.0 到 3.0，增强对比度
    } else {
        gamma = 1.0 + value * 0.02; // 负值：0.0 到 1.0，降低对比度
    }

    mat.convertTo(gammaMat, CV_32F, 1.0 / 255.0);// 转换为浮点型
    cv::pow(gammaMat, gamma, gammaMat);// 应用伽马变换
    
    // 根据输入图像的通道数决定输出格式
    if (mat.channels() == 3) 
    {
        gammaMat.convertTo(gammaMat, CV_8UC3, 255.0);// 3通道
    } 
    else if (mat.channels() == 4) 
    {
        gammaMat.convertTo(gammaMat, CV_8UC4, 255.0);// 4通道
    } 
    else 
    {
        gammaMat.convertTo(gammaMat, CV_8UC1, 255.0);// 单通道
    }

    QImage gammaImage = MattoQImage(gammaMat);// 将伽马变换后的Mat转换为QImage
    
    return gammaImage;
}

QImage MainWindow::CannyEdgeDetection(QImage image, double lowThreshold, double highThreshold) //Canny边缘检测函数
{
    if (image.isNull()) 
    {
        return QImage();
    }

    cv::Mat mat = QImagetoMat(image); // 将QImage转换为OpenCV Mat
    
    cv::Mat edgesMat;// 创建一个边缘检测后的Mat对象

    // 修改：根据输入图像的通道数选择合适的转换方式
    if (mat.channels() == 4) 
    {
        cv::cvtColor(mat, edgesMat, cv::COLOR_BGRA2GRAY);// 4通道转灰度
    } 
    else if (mat.channels() == 3) 
    {
        cv::cvtColor(mat, edgesMat, cv::COLOR_BGR2GRAY);// 3通道转灰度
    } 
    else if (mat.channels() == 1) 

    {
        edgesMat = mat.clone(); // 已经是灰度图像
    } 
    else 
    {
        QMessageBox::warning(nullptr, "Error", "Unsupported image format for edge detection.");
        return QImage();
    }
    
    cv::Canny(edgesMat, edgesMat, lowThreshold, highThreshold);// 应用Canny边缘检测

    QImage edgesImage = MattoQImage(edgesMat);// 将边缘检测后的Mat转换为QImage
    
    return edgesImage;
}

QImage MainWindow::Stitching(QImage image1, QImage image2) //图像拼接函数
{
    if (image1.isNull() || image2.isNull()) 
    {
        return QImage();
    }

    cv::Mat mat1 = QImagetoMat(image1); // 将QImage转换为OpenCV Mat
    cv::Mat mat2 = QImagetoMat(image2); // 将QImage转换为OpenCV Mat
    
    // 确保图像格式为CV_8UC3（3通道BGR）
    cv::Mat processedMat1, processedMat2;
    
    // 处理第一张图像
    if (mat1.channels() == 4) 
    {
        cv::cvtColor(mat1, processedMat1, cv::COLOR_BGRA2BGR); // 4通道转3通道
    } 
    else if (mat1.channels() == 1) 
    {
        cv::cvtColor(mat1, processedMat1, cv::COLOR_GRAY2BGR); // 灰度转3通道
    } 
    else if (mat1.channels() == 3) 
    {
        processedMat1 = mat1.clone(); // 已经是3通道
    } 
    else 
    {
        QMessageBox::warning(nullptr, "拼接错误", "不支持的图像格式 - 图像1");
        return QImage();
    }
    
    // 处理第二张图像
    if (mat2.channels() == 4) 
    {
        cv::cvtColor(mat2, processedMat2, cv::COLOR_BGRA2BGR); // 4通道转3通道
    } 
    else if (mat2.channels() == 1) 
    {
        cv::cvtColor(mat2, processedMat2, cv::COLOR_GRAY2BGR); // 灰度转3通道
    } 
    else if (mat2.channels() == 3) 
    {
        processedMat2 = mat2.clone(); // 已经是3通道
    } 
    else 
    {
        QMessageBox::warning(nullptr, "拼接错误", "不支持的图像格式 - 图像2");
        return QImage();
    }
    
    // 确保数据类型为CV_8U
    if (processedMat1.type() != CV_8UC3) {
        processedMat1.convertTo(processedMat1, CV_8UC3);
    }
    if (processedMat2.type() != CV_8UC3) {
        processedMat2.convertTo(processedMat2, CV_8UC3);
    }
    
    cv::Mat stitchedMat; // 创建一个拼接后的Mat对象

    std::vector<cv::Mat> images = { processedMat1, processedMat2 }; // 使用处理后的图像
    
    // 创建拼接器并设置参数
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create();
    
    cv::Stitcher::Status status = stitcher->stitch(images, stitchedMat); // 执行图像拼接
    
    if (status != cv::Stitcher::OK) 
    {
        QString errorMsg;
        switch (status) // 检查拼接状态
        {
            case cv::Stitcher::ERR_NEED_MORE_IMGS:
                errorMsg = "需要更多图像进行拼接";
                break;
            case cv::Stitcher::ERR_HOMOGRAPHY_EST_FAIL:
                errorMsg = "单应性矩阵估计失败";
                break;
            case cv::Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL:
                errorMsg = "相机参数调整失败";
                break;
            default:
                errorMsg = "拼接失败，请检查图像是否适合拼接";
                break;
        }
        QMessageBox::warning(nullptr, "拼接错误", errorMsg);
        return QImage();
    }

    QImage stitchedImage = MattoQImage(stitchedMat); // 将拼接后的Mat转换为QImage
    stitchedImage = stitchedImage.rgbSwapped(); // 确保颜色通道顺序正确
    return stitchedImage;
}

QImage MainWindow::Fusion(QImage image1, QImage image2) //图像融合函数
{
    if (image1.isNull() || image2.isNull()) 
    {
        return QImage();
    }

    cv::Mat mat1 = QImagetoMat(image1); // 将QImage转换为OpenCV Mat
    cv::Mat mat2 = QImagetoMat(image2); // 将QImage转换为OpenCV Mat
    
    // 确保两个图像都是相同的格式（优先使用3通道）
    cv::Mat processedMat1, processedMat2;
    
    // 统一转换为3通道格式
    if (mat1.channels() == 4) 
    {
        cv::cvtColor(mat1, processedMat1, cv::COLOR_BGRA2BGR);
    } 
    else if (mat1.channels() == 1) 
    {
        cv::cvtColor(mat1, processedMat1, cv::COLOR_GRAY2BGR);
    } 
    else 
    {
        processedMat1 = mat1.clone();
    }
    
    if (mat2.channels() == 4) 
    {
        cv::cvtColor(mat2, processedMat2, cv::COLOR_BGRA2BGR);
    } 
    else if (mat2.channels() == 1) 
    {
        cv::cvtColor(mat2, processedMat2, cv::COLOR_GRAY2BGR);
    } 
    else 
    {
        processedMat2 = mat2.clone();
    }
    
    // 确保两个图像尺寸相同
    if (processedMat1.size() != processedMat2.size()) 
    {
        cv::resize(processedMat2, processedMat2, processedMat1.size()); // 将mat2调整到与mat1相同的尺寸
    }
    
    cv::Mat fusedMat; // 创建一个融合后的Mat对象
    cv::addWeighted(processedMat1, 0.5, processedMat2, 0.5, 0.0, fusedMat); // 执行图像融合

    QImage fusedImage = MattoQImage(fusedMat); // 将融合后的Mat转换为QImage
    fusedImage = fusedImage.rgbSwapped(); // 确保颜色通道顺序正确
    return fusedImage;
}

QImage MainWindow::TextureTransfer(QImage source, QImage target) //纹理迁移函数
{
    if (source.isNull() || target.isNull()) 
    {
        return QImage();
    }

    cv::Mat sourceMat = QImagetoMat(source); // 源图像（结构图像）
    cv::Mat targetMat = QImagetoMat(target); // 目标图像（纹理图像）
    
    if (sourceMat.empty() || targetMat.empty()) 
    {
        QMessageBox::warning(nullptr, "纹理迁移", "图像转换失败。");
        return QImage();
    }

    // 修正：确保两个图像都是3通道BGR格式，并保存原始图像信息
    cv::Mat processedSource, processedTarget;
    
    qDebug() << "原始源图像通道数：" << sourceMat.channels() << "，类型：" << sourceMat.type();
    qDebug() << "原始目标图像通道数：" << targetMat.channels() << "，类型：" << targetMat.type();
    
    // 处理源图像
    if (sourceMat.channels() == 4) 
    {
        cv::cvtColor(sourceMat, processedSource, cv::COLOR_BGRA2BGR);
    } 
    else if (sourceMat.channels() == 1) 
    {
        cv::cvtColor(sourceMat, processedSource, cv::COLOR_GRAY2BGR);
    } 
    else if (sourceMat.channels() == 3) 
    {
        processedSource = sourceMat.clone();
    } 
    else 
    {
        QMessageBox::warning(nullptr, "纹理迁移", "不支持的源图像格式。");
        return QImage();
    }
    
    // 处理目标纹理图像
    if (targetMat.channels() == 4) 
    {
        cv::cvtColor(targetMat, processedTarget, cv::COLOR_BGRA2BGR);
    } 
    else if (targetMat.channels() == 1) 
    {
        cv::cvtColor(targetMat, processedTarget, cv::COLOR_GRAY2BGR);
    } 
    else if (targetMat.channels() == 3) 
    {
        processedTarget = targetMat.clone();
    } 
    else 
    {
        QMessageBox::warning(nullptr, "纹理迁移", "不支持的目标图像格式。");
        return QImage();
    }

    // 确保数据类型正确
    if (processedSource.type() != CV_8UC3) {
        processedSource.convertTo(processedSource, CV_8UC3);
    }
    if (processedTarget.type() != CV_8UC3) {
        processedTarget.convertTo(processedTarget, CV_8UC3);
    }

    qDebug() << "处理后源图像通道数：" << processedSource.channels() << "，类型：" << processedSource.type();
    qDebug() << "处理后目标图像通道数：" << processedTarget.channels() << "，类型：" << processedTarget.type();

    // 定义纹理块大小
    const int blockSize = 8;
    
    // 计算源图像和目标纹理图像的亮度
    cv::Mat sourceGray, targetGray;
    cv::cvtColor(processedSource, sourceGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(processedTarget, targetGray, cv::COLOR_BGR2GRAY);
    
    // 创建结果图像 - 修正：使用源图像的尺寸，数据类型为CV_8UC3
    cv::Mat result = cv::Mat::zeros(processedSource.size(), CV_8UC3);
    
    // 预先提取目标纹理图像中的所有块及其平均亮度
    std::vector<cv::Mat> textureBlocks;
    std::vector<double> textureBlockBrightness;
    
    int textureRows = processedTarget.rows;
    int textureCols = processedTarget.cols;
    
    qDebug() << "开始提取纹理块，纹理图像尺寸：" << textureCols << "x" << textureRows;
    
    // 从纹理图像中提取所有可能的块
    for (int y = 0; y <= textureRows - blockSize; y += blockSize/2)
    {
        for (int x = 0; x <= textureCols - blockSize; x += blockSize/2)
        {
            cv::Rect blockRect(x, y, blockSize, blockSize);
            
            // 确保块不超出边界
            if (x + blockSize > textureCols) blockRect.width = textureCols - x;
            if (y + blockSize > textureRows) blockRect.height = textureRows - y;
            
            if (blockRect.width >= blockSize/2 && blockRect.height >= blockSize/2)
            {
                cv::Mat textureBlock = processedTarget(blockRect).clone();
                cv::Mat textureBlockGray = targetGray(blockRect);
                
                // 调整块大小到标准大小
                if (textureBlock.size() != cv::Size(blockSize, blockSize))
                {
                    cv::resize(textureBlock, textureBlock, cv::Size(blockSize, blockSize));
                    cv::resize(textureBlockGray, textureBlockGray, cv::Size(blockSize, blockSize));
                }
                
                // 确保纹理块数据类型正确
                if (textureBlock.type() != CV_8UC3) {
                    textureBlock.convertTo(textureBlock, CV_8UC3);
                }
                
                // 计算块的平均亮度
                cv::Scalar meanBrightness = cv::mean(textureBlockGray);
                
                textureBlocks.push_back(textureBlock);
                textureBlockBrightness.push_back(meanBrightness[0]);
            }
        }
    }
    
    qDebug() << "提取到纹理块数量：" << textureBlocks.size();
    
    if (textureBlocks.empty())
    {
        QMessageBox::warning(nullptr, "纹理迁移", "无法从纹理图像中提取有效块。");
        return QImage();
    }
    
    // 遍历源图像，用纹理块替换
    int sourceRows = processedSource.rows;
    int sourceCols = processedSource.cols;
    
    qDebug() << "开始处理源图像，尺寸：" << sourceCols << "x" << sourceRows;
    
    for (int y = 0; y < sourceRows; y += blockSize)
    {
        for (int x = 0; x < sourceCols; x += blockSize)
        {
            cv::Rect sourceBlockRect(x, y, blockSize, blockSize);
            
            // 调整边界块的大小
            if (x + blockSize > sourceCols) sourceBlockRect.width = sourceCols - x;
            if (y + blockSize > sourceRows) sourceBlockRect.height = sourceRows - y;
            
            // 计算源图像块的平均亮度
            cv::Mat sourceBlockGray = sourceGray(sourceBlockRect);
            cv::Scalar sourceMeanBrightness = cv::mean(sourceBlockGray);
            double sourceBrightness = sourceMeanBrightness[0];
            
            // 在纹理块中找到亮度最匹配的块
            int bestMatchIndex = 0;
            double minDifference = std::abs(textureBlockBrightness[0] - sourceBrightness);
            
            for (size_t i = 1; i < textureBlocks.size(); i++)
            {
                double difference = std::abs(textureBlockBrightness[i] - sourceBrightness);
                if (difference < minDifference)
                {
                    minDifference = difference;
                    bestMatchIndex = i;
                }
            }
            
            // 获取最佳匹配的纹理块
            cv::Mat bestTextureBlock = textureBlocks[bestMatchIndex].clone();
            
            // 如果块大小不匹配，调整纹理块大小
            if (bestTextureBlock.size() != sourceBlockRect.size())
            {
                cv::resize(bestTextureBlock, bestTextureBlock, sourceBlockRect.size());
            }
            
            // 确保纹理块数据类型和结果图像一致
            if (bestTextureBlock.type() != result.type()) {
                bestTextureBlock.convertTo(bestTextureBlock, result.type());
            }
            
            // 将纹理块复制到结果图像
            try {
                bestTextureBlock.copyTo(result(sourceBlockRect));
            } catch (const cv::Exception& e) {
                qDebug() << "复制块时出错：" << e.what();
                // 如果复制失败，直接赋值像素
                for (int dy = 0; dy < sourceBlockRect.height; dy++) {
                    for (int dx = 0; dx < sourceBlockRect.width; dx++) {
                        if (dy < bestTextureBlock.rows && dx < bestTextureBlock.cols) {
                            result.at<cv::Vec3b>(sourceBlockRect.y + dy, sourceBlockRect.x + dx) = 
                                bestTextureBlock.at<cv::Vec3b>(dy, dx);
                        }
                    }
                }
            }
        }
    }
    
    qDebug() << "纹理迁移处理完成，结果图像类型：" << result.type() << "，通道数：" << result.channels();
    
    // 平滑处理：使用双边滤波和加权平均
    cv::Mat smoothed;
    try {
        cv::bilateralFilter(result, smoothed, 5, 50, 50);
        cv::addWeighted(result, 0.8, smoothed, 0.2, 0, result);
    } catch (const cv::Exception& e) {
        qDebug() << "平滑处理失败：" << e.what();
        // 如果平滑处理失败，直接使用原结果
    }
    
    // 修正：转换回QImage时不再调用rgbSwapped()
    QImage transferredImage = MattoQImage(result);
    
    if (transferredImage.isNull()) 
    {
        QMessageBox::warning(nullptr, "纹理迁移", "转换结果图像失败。");
        return QImage();
    }
    
    qDebug() << "纹理迁移成功完成";
    return transferredImage;
}

QImage MainWindow::SkinSmoothing(QImage image, int level) //磨皮函数
{
    if (image.isNull()) 
    {
        return QImage();
    }

    // 人脸检测
    if (faces.empty()) 
    {
        QMessageBox::warning(nullptr, "磨皮", "未检测到人脸，无法进行磨皮处理。");
        return image;
    }

    cv::Mat mat = QImagetoMat(image);
    cv::Mat result = mat.clone();

    // 参数映射
    int bilateral_d = 9; // 邻域直径
    double bilateral_sigmaColor = 30 + level * 1.5; // 颜色空间标准差
    double bilateral_sigmaSpace = 30 + level * 1.5; // 坐标空间标准差
    int gaussian_ksize = 3 + (level / 10) * 2; // 高斯核大小，随level增大
    if (gaussian_ksize % 2 == 0) gaussian_ksize++; // 必须为奇数
    double alpha = 0.4 + 0.5 * (level / 100.0); // 融合权重，level越大磨皮越强

    for (const auto& face : faces)
    {
        // 提取人脸ROI
        cv::Rect faceRect = face & cv::Rect(0, 0, mat.cols, mat.rows);
        cv::Mat faceROI = mat(faceRect).clone();

        // 保证faceROI为CV_8UC3（BGR三通道）
        cv::Mat faceROI_BGR;
        if (faceROI.channels() == 4) {
            cv::cvtColor(faceROI, faceROI_BGR, cv::COLOR_BGRA2BGR);
        } else if (faceROI.channels() == 1) {
            cv::cvtColor(faceROI, faceROI_BGR, cv::COLOR_GRAY2BGR);
        } else {
            faceROI_BGR = faceROI;
        }

        // 双边滤波
        cv::Mat bilateral;
        cv::bilateralFilter(faceROI_BGR, bilateral, bilateral_d, bilateral_sigmaColor, bilateral_sigmaSpace);

        // 高斯模糊
        cv::Mat gaussian;
        cv::GaussianBlur(bilateral, gaussian, cv::Size(gaussian_ksize, gaussian_ksize), 0);

        // 融合（保留细节，增强平滑）
        cv::Mat smoothFace;
        cv::addWeighted(bilateral, 1.0 - alpha, gaussian, alpha, 0, smoothFace);

        // 将处理后的人脸区域拷贝回原图（需通道匹配）
        if (mat.channels() == 4) {
            // 先把result的人脸区域转为BGR，替换后再转回BGRA
            cv::Mat resultROI = result(faceRect);
            cv::Mat resultROI_BGR;
            cv::cvtColor(resultROI, resultROI_BGR, cv::COLOR_BGRA2BGR);
            smoothFace.copyTo(resultROI_BGR);
            cv::cvtColor(resultROI_BGR, resultROI, cv::COLOR_BGR2BGRA);
        } else if (mat.channels() == 3) {
            smoothFace.copyTo(result(faceRect));
        } else if (mat.channels() == 1) {
            // 灰度图，直接转回灰度
            cv::Mat smoothGray;
            cv::cvtColor(smoothFace, smoothGray, cv::COLOR_BGR2GRAY);
            smoothGray.copyTo(result(faceRect));
        }
    }

    QImage smoothedImage = MattoQImage(result);
    return smoothedImage;
}

QImage MainWindow::FaceSlimming(QImage image, int level) //瘦脸函数
{
    if (image.isNull()) {
        return image;
    }

    if (level == 0) {
        return image; // 值为0时，返回原图
    }

    // 1. QImage转cv::Mat并确保为3通道BGR
    cv::Mat src = QImagetoMat(image);
    if (src.empty()) {
        QMessageBox::warning(nullptr, "瘦脸", "图像转换失败。");
        return image;
    }

    // 保存原始通道数用于最后转换
    int originalChannels = src.channels();

    // 统一转换为3通道BGR
    if (src.channels() == 4) {
        cv::cvtColor(src, src, cv::COLOR_BGRA2BGR);
    } else if (src.channels() == 1) {
        cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);
    } else if (src.channels() != 3) {
        QMessageBox::warning(nullptr, "瘦脸", "不支持的图像格式。");
        return image;
    }

    // 2. 人脸检测
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(FACE_CASCADE_PATH.toStdString())) {
        QMessageBox::warning(nullptr, "瘦脸", "无法加载人脸检测模型。");
        return image;
    }

    std::vector<cv::Rect> faces_local;
    face_cascade.detectMultiScale(gray, faces_local, 1.1, 3, 0, cv::Size(80, 80));

    if (faces_local.empty()) {
        QMessageBox::warning(nullptr, "瘦脸", "未检测到人脸。");
        return image;
    }

    // 4. 对每个检测到的人脸进行瘦脸处理
    cv::Mat result = src.clone();
    
    for (size_t i = 0; i < faces_local.size(); ++i) {
        try {
            // 对人脸区域进行局部变形
            cv::Rect faceRect = faces_local[i];
            
            // 扩展人脸区域以包含更多上下文
            int expand = std::max(faceRect.width, faceRect.height) / 4;
            faceRect.x = std::max(0, faceRect.x - expand);
            faceRect.y = std::max(0, faceRect.y - expand);
            faceRect.width = std::min(src.cols - faceRect.x, faceRect.width + 2 * expand);
            faceRect.height = std::min(src.rows - faceRect.y, faceRect.height + 2 * expand);

            cv::Mat faceROI = result(faceRect).clone();
            cv::Mat deformedROI = faceROI.clone();

            // 使用径向变形算法
            cv::Point2f faceCenter(faceRect.width / 2.0f, faceRect.height / 2.0f);
            float maxRadius = std::min(faceRect.width, faceRect.height) / 2.0f;
            
            // 修改：根据level正负值调整变形强度和方向
            float deformStrength = abs(level) * 0.002f; // 使用绝对值计算强度
            bool isSlimming = (level > 0); // 正值瘦脸，负值胖脸

            for (int y = 0; y < faceROI.rows; ++y) {
                for (int x = 0; x < faceROI.cols; ++x) {
                    cv::Point2f currentPoint(x, y);
                    
                    // 计算到脸部中心的距离
                    float dx = currentPoint.x - faceCenter.x;
                    float dy = currentPoint.y - faceCenter.y;
                    float distance = std::sqrt(dx * dx + dy * dy);
                    
                    if (distance < maxRadius && distance > 0) {
                        float normalizedDistance = distance / maxRadius;
                        
                        float compressionFactor;
                        if (isSlimming) {
                            // 正值：向内收缩（瘦脸）
                            compressionFactor = 1.0f - deformStrength * (1.0f - normalizedDistance * normalizedDistance);
                            compressionFactor = std::max(0.5f, std::min(1.0f, compressionFactor));
                        } else {
                            // 负值：向外扩张（胖脸）
                            compressionFactor = 1.0f + deformStrength * (1.0f - normalizedDistance * normalizedDistance);
                            compressionFactor = std::max(1.0f, std::min(1.5f, compressionFactor));
                        }
                        
                        cv::Point2f newPoint;
                        newPoint.x = faceCenter.x + dx * compressionFactor;
                        newPoint.y = faceCenter.y + dy * compressionFactor;
                        
                        // 边界检查
                        int srcX = static_cast<int>(newPoint.x);
                        int srcY = static_cast<int>(newPoint.y);
                        
                        if (srcX >= 0 && srcX < faceROI.cols && 
                            srcY >= 0 && srcY < faceROI.rows) {
                            deformedROI.at<cv::Vec3b>(y, x) = faceROI.at<cv::Vec3b>(srcY, srcX);
                        }
                    }
                }
            }

            // 将变形后的人脸区域复制回结果图像
            deformedROI.copyTo(result(faceRect));

        } catch (const cv::Exception& e) {
            qDebug() << "OpenCV exception in face slimming:" << e.what();
            continue;
        }
    }

    // 7. 根据原始图像通道数转换回相应格式
    cv::Mat finalResult;
    if (originalChannels == 4) {
        cv::cvtColor(result, finalResult, cv::COLOR_BGR2BGRA);
    } else if (originalChannels == 1) {
        cv::cvtColor(result, finalResult, cv::COLOR_BGR2GRAY);
    } else {
        finalResult = result;
    }

    // 8. 转换回QImage
    return MattoQImage(finalResult);
}

void MainWindow::on_Action_Load_triggered() //打开图像文件槽函数
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开图像"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    
    if (!fileName.isEmpty()) 
    {
        currentImagePath = fileName;
        originalQImage = QImage(fileName); // 保存原始图像
        currentQImage = originalQImage;    // 当前图像设为原始图像
        currentImage = QImagetoMat(currentQImage);
        srcImagePaths.append(fileName);
        ImageIndex = srcImagePaths.size() - 1;
        
        if (currentQImage.isNull()) 
        {
            QMessageBox::warning(this, tr("打开图像"), tr("无法加载图像，请检查文件路径和格式。"));
            return;
        }
        ShowImage(currentQImage, ui->label_show); // 显示图像

        enableAllSliders(); // 启用所有滑动条
        enableAllRadioButtons(); // 启用所有单选按钮
        enableAllPushButtons(); // 启用所有按钮

        if(srcImagePaths.size() == 1)//是第一张，判断是否有人脸 - 使用绝对路径
        {
            cv::CascadeClassifier faceCascade;
            QString cascadePath = MainWindow::FACE_CASCADE_PATH;// 人脸检测分类器路径

            if (!faceCascade.load(cascadePath.toStdString())) 
            {
                QMessageBox::warning(this, tr("人脸检测"), 
                    tr("无法加载人脸检测模型文件：\n") + cascadePath + 
                    tr("\n请检查文件是否存在。"));
                disableAllFaceBeautySliders(); // 禁用所有美容滑动条
                return;
            }

            faceCascade.detectMultiScale(currentImage, faces);

            if (faces.empty()) 
            {
                QMessageBox::information(this, tr("人脸检测"), tr("未检测到人脸，美容功能将被禁用。"));
                disableAllFaceBeautySliders(); // 禁用所有美容滑动条
            }
            else
            {
                enableAllFaceBeautySliders(); // 启用美容滑动条
            }
        }
    }

    //按钮弹起
    ui->Action_Load->setChecked(false);
}

void MainWindow::on_Action_Save_triggered() //保存图像文件槽函数
{
    if(resultQImage.isNull()) 
    {
       QMessageBox::warning(this, tr("保存图像"), tr("没有可保存的图像。"));
       return;
    }
    // 弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存图像"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty()) 
    {
        if (resultQImage.save(fileName)) 
        {
            QMessageBox::information(this, tr("保存图像"), tr("图像已成功保存。"));
        } 
        else 
        {
            QMessageBox::warning(this, tr("保存图像"), tr("无法保存图像，请检查文件路径和权限。"));
        }
    }

    //按钮弹起
    ui->Action_Save->setChecked(false);
}

void MainWindow::on_IfGrayscale_clicked() //灰度化按钮点击槽函数
{
    if(isGrayscaleApplied) 
    {
        ui->IfGrayscale->setChecked(false); // 如果已经应用灰度化，取消选中状态
        isGrayscaleApplied = false; // 重置灰度化应用状态

        ShowImage(originalQImage, ui->label_show); // 显示原始图像
    }
    else 
    {
        ui->IfGrayscale->setChecked(true); // 如果未应用灰度化，设置为选中状态
        isGrayscaleApplied = true; // 设置灰度化应用状态为已应用

        // 互斥处理，取消其他效果
        if(isEdgeDetectApplied) 
        {
            ui->IfEdgeDetect->setChecked(false); // 如果边缘检测已应用，取消选中状态
            isEdgeDetectApplied = false; // 重置边缘检测应用状态
        }

        if (currentQImage.isNull()) 
        {
            QMessageBox::warning(this, tr("灰度化"), tr("没有可处理的图像。"));
            return;
        }

        resultQImage = GrayScale(currentQImage); // 调用灰度化函数
        if (resultQImage.isNull())
        {
            QMessageBox::warning(this, tr("灰度化"), tr("无法处理图像，请检查图像格式。"));
            return;
        }

        ShowImage(resultQImage, ui->label_show); // 显示处理后的图像
    }
}

void MainWindow::on_IfEdgeDetect_clicked() //Canny边缘检测按钮点击槽函数
{
    if(isEdgeDetectApplied) 
    {
        ui->IfEdgeDetect->setChecked(false); // 如果已经应用边缘检测，取消选中状态
        isEdgeDetectApplied = false; // 重置边缘检测应用状态

        ShowImage(originalQImage, ui->label_show); // 显示原始图像
    } 
    else 
    {
        ui->IfEdgeDetect->setChecked(true); // 如果未应用边缘检测，设置为选中状态
        isEdgeDetectApplied = true; // 设置边缘检测应用状态为已应用

        // 互斥处理，取消其他效果
        if(isGrayscaleApplied)
        {
            ui->IfGrayscale->setChecked(false); // 如果灰度化已应用，取消选中状态
            isGrayscaleApplied = false; // 重置灰度化应用状态
        }

         if (currentQImage.isNull()) 
        {
            QMessageBox::warning(this, tr("Canny边缘检测"), tr("没有可处理的图像。"));
            return;
        }

        double lowThreshold = 100.0; // 设置低阈值
        double highThreshold = 200.0; // 设置高阈值

        resultQImage = CannyEdgeDetection(currentQImage, lowThreshold, highThreshold); // 调用Canny边缘检测函数
        if (resultQImage.isNull()) 
        {
            QMessageBox::warning(this, tr("Canny边缘检测"), tr("无法处理图像，请检查图像格式。"));
            return;
        }

    ShowImage(resultQImage, ui->label_show); // 显示处理后的图像
    }
}

void MainWindow::on_GammaCorrectionSlider_valueChanged(int value) //滑动条值改变槽函数
{
    if (originalQImage.isNull()) 
    {
        return;
    }
    
    resultQImage = GammaTransform(originalQImage, value);// 直接传递value，不再进行转换

    if (resultQImage.isNull())
    {
        QMessageBox::warning(this, tr("伽马变换"), tr("无法处理图像，请检查图像格式。"));
        return;
    }

    ShowImage(resultQImage, ui->label_show); // 显示处理后的图像
}

void MainWindow::on_ImageSharpeningSlider_valueChanged(int value) //锐化滑动条值改变槽函数
{
    if (originalQImage.isNull()) 
    {
        QMessageBox::warning(this, tr("图像锐化"), tr("没有可处理的图像。"));
        return;
    }

    resultQImage = Sharpen(originalQImage, value); // 调用图像锐化函数
    if (resultQImage.isNull())
    {
        QMessageBox::warning(this, tr("图像锐化"), tr("无法处理图像，请检查图像格式。"));
        return;
    }

    ShowImage(resultQImage, ui->label_show); // 显示处理后的图像
}

void MainWindow::on_SkinSmoothingSlider_6_valueChanged(int value) //磨皮滑动条值改变槽函数
{
    if (currentQImage.isNull()) 
    {
        QMessageBox::warning(this, tr("磨皮"), tr("没有可处理的图像。"));
        return;
    }

    resultQImage = SkinSmoothing(currentQImage, value); // 调用磨皮函数
    if (resultQImage.isNull())
    {
        QMessageBox::warning(this, tr("磨皮"), tr("无法处理图像，请检查图像格式。"));
        return;
    }

    ShowImage(resultQImage, ui->label_show); // 显示处理后的图像
}

void MainWindow::on_FaceSlimmingSlider_7_valueChanged(int value) //瘦脸滑动条值改变槽函数
{
    if (currentQImage.isNull()) 
    {
        QMessageBox::warning(this, tr("瘦脸"), tr("没有可处理的图像。"));
        return;
    }

    resultQImage = FaceSlimming(currentQImage, value); // 调用瘦脸函数
    if (resultQImage.isNull())
    {
        QMessageBox::warning(this, tr("瘦脸"), tr("无法处理图像，请检查图像格式。"));
        return;
    }
    ShowImage(resultQImage, ui->label_show); // 显示处理后的图像
}

void MainWindow::on_ImageSitchingButton_clicked() //图像拼接按钮点击槽函数
{
    if (srcImagePaths.size() < 2) 
    {
        QMessageBox::warning(this, tr("图像拼接"), tr("至少需要两张图像进行拼接。"));
        QMessageBox::information(this, tr("图像拼接"), tr("请先打开两张或更多图像。"));
        on_Action_Load_triggered(); // 调用打开图像的槽函数
    }
    
    QImage image1 = QImage(srcImagePaths[0]);
    QImage image2 = QImage(srcImagePaths[1]);
    
    if (image1.isNull() || image2.isNull())
    {
        QMessageBox::warning(this, tr("图像拼接"), tr("无法加载图像，请检查图像路径。"));
        return;
    }

    resultQImage = Stitching(image1, image2); // 图像拼接函数
    if (resultQImage.isNull()) 
    {
        QMessageBox::warning(this, tr("图像拼接"), tr("拼接失败，请检查图像是否适合拼接。"));
        return;
    } 

    ShowImage(resultQImage, ui->label_show); // 显示拼接后的图像
}

void MainWindow::on_ImageFusionButton_clicked() //图像融合按钮点击槽函数
{
    if (srcImagePaths.size() < 2) 
    {
        QMessageBox::warning(this, tr("图像融合"), tr("至少需要两张图像进行融合。"));
        QMessageBox::information(this, tr("图像融合"), tr("请先打开两张或更多图像。"));
        on_Action_Load_triggered(); // 调用打开图像的槽函数
    }
    
    QImage image1 = QImage(srcImagePaths[0]);
    QImage image2 = QImage(srcImagePaths[1]);
    
    if (image1.isNull() || image2.isNull())
    {
        QMessageBox::warning(this, tr("图像融合"), tr("无法加载图像，请检查图像路径。"));
        return;
    }

    resultQImage = Fusion(image1, image2); // 图像融合函数
    if (resultQImage.isNull()) 
    {
        QMessageBox::warning(this, tr("图像融合"), tr("融合失败，请检查图像是否适合融合。"));
        return;
    }
    
    ShowImage(resultQImage, ui->label_show); // 显示融合后的图像
}

void MainWindow::on_ImageTextureTransferButton_clicked() //纹理迁移按钮点击槽函数
{
    if (srcImagePaths.size() < 2) 
    {
        QMessageBox::warning(this, tr("纹理迁移"), tr("至少需要两张图像进行纹理迁移。"));
        QMessageBox::information(this, tr("纹理迁移"), tr("请先打开两张或更多图像。"));
        on_Action_Load_triggered(); // 调用打开图像的槽函数
    }
    
    QImage source = QImage(srcImagePaths[0]);
    QImage target = QImage(srcImagePaths[1]);
    
    if (source.isNull() || target.isNull())
    {
        QMessageBox::warning(this, tr("纹理迁移"), tr("无法加载图像，请检查图像路径。"));
        return;
    }

    resultQImage = TextureTransfer(source, target); // 纹理迁移函数
    if (resultQImage.isNull())
    {
        QMessageBox::warning(this, tr("纹理迁移"), tr("纹理迁移失败，请检查图像是否适合迁移。"));
        return;
    }

    ShowImage(resultQImage, ui->label_show); // 显示纹理迁移后的图像
}
