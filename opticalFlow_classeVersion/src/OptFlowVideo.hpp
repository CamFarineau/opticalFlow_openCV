#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace cv;


class OptFlowVideo{
    public:

        // Parametized constructor can be created but initialize all parameters with the set functions would be better
        OptFlowVideo();
        OptFlowVideo(cv::String filenameTemp);

        // Function to load the video
        // Return 0 if successful, 1 if not
        int loadVideo();

        void getImageWithInitFeatures(cv::String outputPath);
        void getImageWithOpticalFlow(cv::String outputPath);
        void getVectorVideo();
        void releaseVideo();

        void setFilename(string filenameTemp){filename = filenameTemp;};
        string getFilename(){return filename;};

        void setMaxFeatures(int maxFeaturesTemp){maxFeatures = maxFeaturesTemp;};
        int getMaxFeatures(){return maxFeatures;};

        void setQualityLevel(double qualityLevelTemp){qualityLevel = qualityLevelTemp;};
        double getQualityLevel(){return qualityLevel;};

        void setMinDistance(double minDistanceTemp){minDistance = minDistanceTemp;};
        double getMinDistance(){return minDistance;};

        void setBlockSize(int blockSizeTemp){blockSize = blockSizeTemp;};
        int getBlockSize(){return blockSize;};

        void setUseHarrisDetector(bool useHarrisDetectorTemp){useHarrisDetector = useHarrisDetectorTemp;};
        bool getUseHarrisDetector(){return useHarrisDetector;};

        void setWinSize(Size winSizeTemp){winSize = winSizeTemp;};
        Size getWinSize(){return winSize;};

        void setMaxLevelPyramids(int maxLevelPyramidsTemp){maxLevelPyramids = maxLevelPyramidsTemp;};
        int getMaxLevelPyramids(){return maxLevelPyramids;};

    private:
        //Video
        cv::String filename;
        VideoCapture video;
        Mat firstFrame;

        //Features
        vector<Point2f> initFeatures;

        //Features parameters
        int maxFeatures;
        double qualityLevel;
        double minDistance;
        int blockSize;
        bool useHarrisDetector;

        //OpticalFlow with LK alg parameters
        TermCriteria termcrit;
        Size winSize;
        int maxLevelPyramids;

        // Get the initial features
        // All the parameters need to be initialized
        int searchInitFeatures();
};