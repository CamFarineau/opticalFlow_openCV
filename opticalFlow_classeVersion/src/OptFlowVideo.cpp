#include "OptFlowVideo.hpp"

OptFlowVideo::OptFlowVideo()
{
    this->filename = "";
    this->initFeatures = vector<Point2f>{(-1.0,-1.0)};
    this->maxFeatures = 20;
    this->qualityLevel = 0.01;
    this->minDistance = 10;
    this->blockSize = 3;
    this->useHarrisDetector = false;
    this->winSize = Size(15,15);
    this->maxLevelPyramids = 3;
}

OptFlowVideo::OptFlowVideo(cv::String filenameTemp)
{
    this->filename = filenameTemp;
    this->maxFeatures = 20;
    this->qualityLevel = 0.01;
    this->minDistance = 10;
    this->blockSize = 3;
    this->useHarrisDetector = false;
    this->winSize = Size(15,15);
    this->maxLevelPyramids = 3;
}

int OptFlowVideo::loadVideo()
{
    if (!this->video.isOpened())
    {
        this->video.open(this->filename);
        if (!this->video.isOpened()) {
            std::cerr << "Failed to open the video device, video file or image sequence!\n" << std::endl;
            return 1;
        }
    }

    return 0;
}

int OptFlowVideo::searchInitFeatures()
{ 
    if(loadVideo())
    {
        std::cerr << "No video loaded!\n" << std::endl;
        return 1;
    }

    Mat gray, frame;

    // initialisation
    this->video >> this->firstFrame;
    if( this->firstFrame.empty() )
    {
        std::cerr << "Error with the video, first frame empty\n" << std::endl;
        return 1;
    }
    // get the gray image
    cvtColor(this->firstFrame, gray, COLOR_BGR2GRAY);

    // search the features
    goodFeaturesToTrack(gray, this->initFeatures, this->maxFeatures, this->qualityLevel, this->minDistance, Mat(), this->blockSize, this->blockSize, this->useHarrisDetector, 0.04);

    return 0;
}

void OptFlowVideo::getImageWithInitFeatures(cv::String outputPath)
{
    if(this->initFeatures.empty())
    {
        if(searchInitFeatures())
        {
            std::cerr << "Error with the video -> Frame empty" << std::endl;
            return;
        }
    }

    Mat firstFrameTemp;
    this->firstFrame.copyTo(firstFrameTemp);

    int i = 0, k = 0;
    for( i = k = 0; i < this->initFeatures.size(); i++ )
    {
            this->initFeatures[k++] = this->initFeatures[i];
            circle( firstFrameTemp, this->initFeatures[i], 3, Scalar(0,255,0), -1, 8);
    }

    namedWindow( "initFeatures", 1 );
    imshow("initFeatures", firstFrameTemp);
    waitKey(0);

    imwrite( outputPath, firstFrameTemp );
}

void OptFlowVideo::getImageWithOpticalFlow(cv::String outputPath)
{
    if(this->initFeatures.empty())
    {
        if(searchInitFeatures())
        {
            std::cerr << "Error with the video -> Frame empty" << std::endl;
            return;
        }
    }

    Mat firstFrameTemp, frame, image, gray, prevGray;
    this->firstFrame.copyTo(firstFrameTemp);

    vector<Point2f> points[2];
    points[0] = this->initFeatures;
    int i = 0, k = 0;


    namedWindow( "Video with Features", 1 );

    while(1)
    {

        this->video >> frame;
        if( frame.empty() )
            break;
        
        frame.copyTo(image);
        cvtColor(image, gray, COLOR_BGR2GRAY);

        if(prevGray.empty())
            gray.copyTo(prevGray);

        vector<uchar> status;
        vector<float> err;
        calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, this->winSize, this->maxLevelPyramids, this->termcrit, this->useHarrisDetector, 0.001);

        for( i = k = 0; i < points[1].size(); i++ )
        {
            if( !status[i] )
                continue;
            points[1][k++] = points[1][i];
            circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
            line(firstFrameTemp, points[1][i],points[0][i], Scalar(255,0,0));
        }

        imshow("Video with Features", image);
        char c = (char)waitKey(10);
        if( c == 27 )
            break;

        std::swap(points[1], points[0]);
        cv::swap(prevGray, gray);
    }

    namedWindow( "First Frame with Optical Flow", 1 );
    imshow("First Frame with Optical Flow", firstFrameTemp);
    waitKey(0);

    imwrite( outputPath, firstFrameTemp );

}

void OptFlowVideo::getVectorVideo()
{

}

void OptFlowVideo::releaseVideo()
{
    this->video.release();
}