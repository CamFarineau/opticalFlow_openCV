#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace cv;



int main(int argc, char** argv)
{
    /* initialize random seed: */
  srand (time(NULL));
    
    cv::VideoCapture vid("/home/camille/work/opticalFlow_test/opticalFlow_openCV/data/slow.mp4");
    //vid.open("/home/camille/work/opticalFlow_test/opticalFlow_openCV/data/flow.mp4");
    if (!vid.isOpened()) {
        std::cerr << "Failed to open the video device, video file or image sequence!\n" << std::endl;
        return 1;
    }

    //Parameters for the calcOpticalFlowPyrLK function
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,10,0.03);
    Size  winSize(15,15);
    const int max = 20;

    // Parameter for the goodFeaturesToTrack function
    int maxLevel = 5;

    Mat gray, prevGray, image, frame, mask;
    vector<Point2f> points[2];

    // initialisation
    vid >> frame;
    if( frame.empty() )
            return 1;
    frame.copyTo(image);
    image.copyTo(mask);
    cvtColor(image, gray, COLOR_BGR2GRAY);
    gray.copyTo(prevGray);

    // Find corners in the first frame
    int i = 0, k = 0;

    // Others parameters are hard-coded in the call
    goodFeaturesToTrack(gray, points[1], max, 0.01, 10, Mat(), 3, 3, 0, 0.04);

    // TO GET THE IMAGE OF THE DETECTED FEATURES
    // for( i = k = 0; i < points[1].size(); i++ )
    // {
    //         points[1][k++] = points[1][i];
    //         //std::cout<<blue_color.at(i)<<std::endl;
    //         circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
    //         //points[1].resize(k);
    // }

    // namedWindow( "test", 1 );
    // imwrite( "/home/camille/work/opticalFlow_test/opticalFlow_openCV/data/featuresSelection/qualityLevel09.jpg", image );
    

    std::swap(points[1], points[0]);
    int cpt = 0, maxFrame = 200;

    vector<int> blue_color;
    for(int j = 0; j < max; j++)
    {
        int temp_blue = rand() % 100 + 1;
        
        blue_color.push_back(temp_blue);
    }

    while(cpt < maxFrame)
    {

        vid >> frame;
        if( frame.empty() )
            break;
        
        frame.copyTo(image);
        cvtColor(image, gray, COLOR_BGR2GRAY);

        if(prevGray.empty())
            gray.copyTo(prevGray);

        vector<uchar> status;
        vector<float> err;
        calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, maxLevel, termcrit, 0, 0.001);

        
        // std::cout<<"nb points: "<<points[1].size()<<std::endl;
        // std::cout<<"points: "<<points[1]<<std::endl;
        for( i = k = 0; i < points[1].size(); i++ )
        {
            if( !status[i] )
                continue;
            points[1][k++] = points[1][i];
            //std::cout<<blue_color.at(i)<<std::endl;
            circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
            line(mask, points[1][i],points[0][i], Scalar(255,0,0));
            //points[1].resize(k);
        }
        //std::cout<<"Points[1]: "<<points[1]<<std::endl;
        imshow("test", image);
        char c = (char)waitKey(10);
        if( c == 27 )
            break;

        
        //std::cout<<"nb points: "<<points[1].size()<<std::endl;
        std::swap(points[1], points[0]);
        cv::swap(prevGray, gray);

        cpt++;

    }
    namedWindow( "flow", 1 );
    imshow("flow", mask);
    waitKey(10);


    imwrite( "/home/camille/work/opticalFlow_test/opticalFlow_openCV/data/termCritEpsilon9.jpg", mask );


    vid.release();
    return 0;
}