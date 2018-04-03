#include <iostream>
#include <stdio.h>
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include "gui.hpp"

using namespace std;

int main(int argc, char** argv)
{
    // Version to test one video at the time
    //OptFlowVideo testVideo("/home/camille/work/opticalFlow_openCV/opticalFlow_classeVersion/data/moscow.mp4");
    // testVideo.set_quality_level(0.1);
    // testVideo.set_block_size(7);
    // testVideo.set_max_features(100);
    // testVideo.set_max_level_pyramids(2);
    // testVideo.set_min_distance(7);
    // testVideo.set_win_size(Size(15,15));
    // testVideo.write_image_with_init_features(true);
    // testVideo.write_image_with_optical_flow(true);
    //testVideo.write_vector_video();

    // testVideo.releaseVideo();


    // Version to test some videos in the same folder

    Gui gui;
    gui.init_gui();
    gui.execute();

    return 0;
}