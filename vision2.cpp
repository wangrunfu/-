#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void circle(cv::Mat image,cv::Mat gray,cv::Mat edges)
{
    // 找到轮廓
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, edges.rows / 2.5, 100, 30, 5, 130);


    float minDiameter = 0;
    float radius = 0;

    // 找到最小圆的半径（以像素为单位）
    for (size_t i = 0; i < circles.size(); i++) {
        radius = circles[i][2];
        if (i == 0)
        {
            minDiameter = radius;
        }
        minDiameter = std::min(minDiameter, radius);
    }
    std::cout << "min: " << minDiameter << endl;

    // 找到最大圆的半径（以像素为单位）
    float maxRadiusPixels = 0;
    for (size_t i = 0; i < circles.size(); i++) {
        float radius = circles[i][2];
        maxRadiusPixels = std::max(maxRadiusPixels, radius);
    }
    std::cout << "max: " << maxRadiusPixels << endl;

    // 计算像素到厘米的比例
    float ratio = 0;
    ratio = 1.25 / minDiameter;
    float maxDiameterCm = 0;
    // 计算最大圆的实际直径
    maxDiameterCm = (maxRadiusPixels * ratio) * 2;

    // 输出最大圆的直径
    std::cout << "最大圆的直径为: " << maxDiameterCm << "厘米" << std::endl;

    // 绘制检测到的圆
    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(image, center, radius, cv::Scalar(0, 255, 0), 3, 8, 0);
    }
}


int main() {
    // 加载图片
    Mat image = imread("C:/Users/888/Desktop/book.jpg", IMREAD_COLOR);
    if (image.empty()) {
        cout << "无法加载图片" << endl;
        return -1;
    }

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // 使用高斯模糊减少噪声
    GaussianBlur(gray, gray, Size(3, 7), 2, 2);

    // 边缘检测
    Mat edges;
    Canny(gray, edges, 20, 50);

    //估算圆的直径
    circle(image, gray, edges);

    //矩形检测
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    // 遍历轮廓
    double maxArea = 0.0;
    int bookContourIndex = -1;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            bookContourIndex = static_cast<int>(i);
        }
    }

    if (bookContourIndex != -1) {
        cv::drawContours(image, contours, bookContourIndex, cv::Scalar(0, 255, 0), 2);
    }


    // 创建一个新的Mat对象来存储缩小后的图片
    Mat resizedImage;

    // 缩小图片到原来的1/4大小
    resize(image, resizedImage, Size(image.cols / 2, image.rows / 2), 0, 0, INTER_LINEAR);
    // 显示结果
    //imshow("Result", resizedImage);
    resize(edges, edges, cv::Size(edges.cols / 2, edges.rows / 2));
    //imshow("r", edges);
    waitKey(0);

    return 0;
}


