#include <boost/filesystem.hpp>
#include "OptFlowVideo.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace boost::filesystem;

/* Class in order to process (get the optical flow) a bunch of videos that are all in the same folder
*/
class OptFlowProcessDirectoryVideo{
    
    public:
    OptFlowProcessDirectoryVideo();
    void process_all_videos_folder(std::string path, bool show_output = false);
};