#include <iostream>
#include <stdio.h>
#include "opt_flow_process_directory_video.hpp"

using namespace std;

int main(int argc, char** argv)
{
    // OLD VERSION
    // OptFlowVideo testVideo;

    // testVideo.setQualityLevel(0.1);
    // testVideo.setBlockSize(7);
    // testVideo.setMaxFeatures(100);
    // testVideo.setMaxLevelPyramids(2);
    // testVideo.setMinDistance(7);
    // testVideo.setWinSize(Size(15,15));
    // testVideo.processAllVideosInFolder("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data", true);
    // testVideo.getImageWithInitFeatures("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/testFolder/initFeatures.jpg",true);
    // testVideo.getImageWithOpticalFlow("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/testFolder/optFlow.jpg",true);
    // testVideo.getVectorVideo("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/testFolder/testVideoVectors.avi",true);

    // testVideo.releaseVideo();
    // testVideo.setFilename("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/slow.mp4");
    // testVideo.setQualityLevel(0.8);

    // testVideo.getImageWithInitFeatures("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/testFolder2/initFeatures2.jpg",true);
    // testVideo.getImageWithOpticalFlow("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/testFolder2/optFlow2.jpg",true);
    // testVideo.getVectorVideo("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/testFolder2/testVideoVectors2.avi",true,5);

    // testVideo.releaseVideo();

    OptFlowProcessDirectoryVideo directory_to_process;
    directory_to_process.process_all_videos_folder("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data");

    return 1;
}