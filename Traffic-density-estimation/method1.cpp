#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <pthread.h>
using namespace std;
using namespace cv;
#include <chrono>
#include <cmath>
using namespace std::chrono;
std::vector<Point2f> pts_src;
std::vector<Point2f> pts_des;
Mat frame2;
Mat frame2Final;
ofstream outputFile;
ifstream compareFile("originaloutput.txt");
float errorSquared = 0;
string compareLine;
Mat Transform(Mat image)
{
    Mat Result;

    Mat h = findHomography(pts_src, pts_des);

    warpPerspective(image, Result, h, image.size());
    return Result;
}

Mat cameraCorrection(Mat image)
{

    Mat wrap = Transform(image);
    cv::Rect roi;
    roi.x = 472;
    roi.y = 52;
    roi.width = 328;
    roi.height = 778;
    return wrap(roi);
}

void funct(Mat frameCopy, int counters)
{
    Mat imgDifference;
    Mat imgThresh;
    cv::cvtColor(frameCopy, frameCopy, cv::COLOR_BGR2GRAY);
    Mat frameFinal = cameraCorrection(frameCopy);

    absdiff(frameFinal, frame2Final, imgDifference);

    threshold(imgDifference, imgThresh, 45, 255.0, cv::THRESH_BINARY);

    int TotalNumberOfPixels = imgThresh.rows * imgThresh.cols;
    int ZeroPixels = TotalNumberOfPixels - countNonZero(imgThresh);

    cout << counters << ", " << 1 - (float)ZeroPixels / TotalNumberOfPixels << endl;
    errorSquared += (stof(compareLine) - 1 + (float)ZeroPixels / TotalNumberOfPixels) * (stof(compareLine) - 1 + (float)ZeroPixels / TotalNumberOfPixels);
    outputFile << counters << ", " << 1 - (float)ZeroPixels / TotalNumberOfPixels << endl;
}

int main(int argc, char *argv[])
{
    int frameSkip = atoi(argv[2]);
    auto start = high_resolution_clock::now();
    outputFile.open("output.txt");
    // ----------------------------------------------------------------------//
    // pts_des and pts_src for homography //

    pts_des.clear();
    pts_des.push_back(Point(472, 52));
    pts_des.push_back(Point(472, 830));
    pts_des.push_back(Point(800, 830));
    pts_des.push_back(Point(800, 52));

    pts_src.clear();
    pts_src.push_back(Point(983, 205));
    pts_src.push_back(Point(222, 1076));
    pts_src.push_back(Point(1538, 1055));
    pts_src.push_back(Point(1268, 205));

    // ---------------------------------------------------------------------//

    VideoCapture cap(argv[1]);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video file" << endl;
        cin.get(); // wait for any key press
        return -1;
    }

    frame2 = imread("finalSnap.png");

    if (!frame2.data)
    {
        cout << "Image not found or unable to open" << endl;
        return -1;
    }
    Mat frame2Copy = frame2.clone();
    cv::cvtColor(frame2Copy, frame2Copy, cv::COLOR_BGR2GRAY);
    frame2Final = cameraCorrection(frame2Copy);
    int counter = 0;
    float result = 0;
    bool bSuccess = true;
    Mat frame;
    while (true)
    {
        counter += frameSkip;
        for (int i = 0; i < frameSkip; i++)
        {
            bSuccess = cap.read(frame);
            if (!bSuccess)
            {
                break;
            }
            getline(compareFile, compareLine);
        }

        funct(frame, counter);
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            break;
        }
    }

    outputFile.close();
    compareFile.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << duration.count() << endl;
    cout << sqrt(errorSquared) << endl;
    return 0;
}