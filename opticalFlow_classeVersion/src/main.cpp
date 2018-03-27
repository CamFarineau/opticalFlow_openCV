#include <iostream>
#include <stdio.h>
#include "OptFlowVideo.hpp"

using namespace std;



int main(int argc, char** argv)
{
    OptFlowVideo testVideo("/home/camille/work/opticalFlow_test/opticalFlow_classeVersion/data/slow.mp4");
    testVideo.getImageWithInitFeatures("/home/camille/work/opticalFlow_test/opticalFlow_classeVersion/data/initFeatures.jpg");
    testVideo.getImageWithOpticalFlow("/home/camille/work/opticalFlow_test/opticalFlow_classeVersion/data/optFlow.jpg");
    testVideo.releaseVideo();

    return 0;
}