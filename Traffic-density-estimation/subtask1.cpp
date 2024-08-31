#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat image, imgGray, Result;
std::vector<Point2f> pts_src;
std::vector<Point2f> pts_des;

void Transform()
{
    pts_des.clear();
    pts_des.push_back(Point(472, 52));
    pts_des.push_back(Point(472, 830));
    pts_des.push_back(Point(800, 830));
    pts_des.push_back(Point(800, 52));

    Mat h = findHomography(pts_src, pts_des);

    warpPerspective(imgGray, Result, h, image.size());
}

void onMouse(int mouseClick, int x, int y, int flags, void *)
{
    switch (mouseClick)
    {
    case EVENT_RBUTTONDOWN:
    {
        if (pts_src.size() < 4)
        {
            pts_src.push_back(Point(x, y));
        }
    }
    }
    if (pts_src.size() == 4)
    {
        Transform();
        cv::Rect roi;
        roi.x = 472;
        roi.y = 52;
        roi.width = 328;
        roi.height = 778;
        cv::Mat crop = Result(roi);
        namedWindow("perspective image", WINDOW_NORMAL);
        imshow("perspective image", Result);
        namedWindow("final image", WINDOW_AUTOSIZE);
        imshow("final image", crop);
        imwrite("Output.jpg", crop);
        setMouseCallback("Gray image", NULL, NULL);
    }
}

int main(int argc, char **argv)
{
    image = imread(argv[1], cv::IMREAD_COLOR);

    if (!image.data)
    {
        cout << "Image not found or unable to open" << endl;
        return -1;
    }
    // namedWindow("Orignal image", WINDOW_NORMAL);
    // imshow("Orignal image", image);
    cv::cvtColor(image, imgGray, cv::COLOR_BGR2GRAY);
    namedWindow("Gray image", WINDOW_NORMAL);
    imshow("Gray image", imgGray);
    setMouseCallback("Gray image", onMouse, NULL);
    waitKey(0);
    return 0;
}
