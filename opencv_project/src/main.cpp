#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 读取
    Mat img = imread("resources/test_image.png");
    if (img.empty()) {
        cout << "Could not read the image" << endl;
        return 1;
    }

    // 灰度图，hsv图
    Mat gray, hsv;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    cvtColor(img, hsv, COLOR_BGR2HSV);

    // 均值滤波
    Mat meanBlur;
    blur(img, meanBlur, Size(13, 13));
    // 高斯滤波
    Mat gaussianBlur;
    GaussianBlur(img, gaussianBlur, Size(13, 13), 0);

    // 提取红色区域
    Scalar lowerred1(0,150,160),upperred1(255,255,255);
    //Scalar lowerred2(170,120,70),upperred2(180,255,255);
    Mat redMask1,redMask2,redMask;
    inRange(hsv, lowerred1, upperred1, redMask);
    //inRange(hsv, lowerred2, upperred2, redMask2);
    Mat redRegion;
    bitwise_and(img,img,redRegion,redMask);

    // 红色轮廓
    vector<vector<Point>> contours;
    findContours(redMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    cv::Mat imgContours=img.clone();
    //vector<Point> contour;
    for (size_t i = 0; i < contours.size(); i++) {
        Scalar color=Scalar(0,0,255);
        drawContours(imgContours,contours,(int)i,color,2);
    }
    cv::Mat contour(img.size(),img.type(),Scalar(255,255,255));
    for (size_t i = 0; i < contours.size(); i++) {
        Scalar color=Scalar(0,0,255);
        drawContours(contour,contours,(int)i,color,2);
    }
    Mat imgWithBoxes = img.clone();
    for (size_t i = 0; i < contours.size(); i++) {
        Rect boundingBox = boundingRect(contours[i]);
        double area = contourArea(contours[i]);
        rectangle(imgWithBoxes, boundingBox, Scalar(0, 255, 0), 2);
    }
    double area;
     for (size_t i = 0; i < contours.size(); i++) {
        area += contourArea(contours[i]);
    }
    cout << "Area of contour " << ": " << area << endl;
    // 提取高亮区域
    Mat highLightMask,highlight;
    threshold(gray, highLightMask, 100, 255, THRESH_BINARY);
    bitwise_and(img,img,highlight,highLightMask);

    //高亮灰度化
    Mat high_gray;
    cvtColor(highlight, high_gray, COLOR_BGR2GRAY);

    //膨胀，腐蚀
    Mat dilated,eroded;
    dilate(highlight, dilated, Mat::ones(3, 3, CV_8U));
    erode(highlight, eroded, Mat::ones(3, 3, CV_8U));

    // 漫水填充
    Mat floodFillImage = highlight.clone();
    vector<Point> points={Point(0,0),Point(1700,0),Point(0,2400),Point(1700,2400)};
    for (const auto& point : points) {
        Rect rect;
    floodFill(floodFillImage, point, Scalar(255,0,0), 0 , Scalar(0), Scalar(100), 4);
    }

    // 方圆文字
    Mat onefour=img.clone();
    rectangle(onefour, Point(50, 50), Point(200, 200), Scalar(0, 255, 0), 2);
    circle(onefour, Point(300, 300), 50, Scalar(255, 0, 0), 3);
    putText(onefour, "Hello", Point(400, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);

    // 旋转
    // 获取图像尺寸
    int width = img.cols;
    int height = img.rows;

    // 计算旋转矩阵
    double angle = 35.0; // 旋转角度
    Mat rotationMatrix = getRotationMatrix2D(Point2f(width / 2.0, height / 2.0), angle, 1.0);

    // 进行旋转
    Mat rotated;
    warpAffine(img, rotated, rotationMatrix, img.size());

    // 四分之一
    Rect roi(0, 0, img.cols / 2, img.rows / 2);
    Mat cropped(img, roi);

    // 保存
    imwrite("resources/gray.jpg", gray);
    imwrite("resources/hsv.jpg", hsv);
    imwrite("resources/meanBlur.jpg", meanBlur);
    imwrite("resources/gaussianBlur.jpg", gaussianBlur);
    imwrite("resources/redRegion.jpg", redRegion);
    imwrite("resources/highLight.jpg", highlight);
    imwrite("resources/floodFillImage.jpg", floodFillImage);
    imwrite("resources/drawnImage.jpg", onefour);
    imwrite("resources/otated.jpg", rotated);
    imwrite("resources/cropped.jpg", cropped);
    imwrite("resources/redcontours.jpg",imgContours);
    imwrite("resources/red_bounding_boxes.jpg", imgWithBoxes);
    imwrite("resources/highlight_gray.jpg",high_gray);
    imwrite("resources/dilated.jpg", dilated);
    imwrite("resources/eroded.jpg", eroded);
    imwrite("resources/contours.jpg", contour);

    return 0;
}