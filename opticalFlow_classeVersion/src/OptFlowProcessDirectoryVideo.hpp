#include <boost/filesystem.hpp>
#include "OptFlowVideo.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace boost::filesystem;

/* Class to process videos (get opt flow) that are all in the same folder
*/
class OptFlowProcessDirectoryVideo{
    
    public:
    OptFlowProcessDirectoryVideo();
    // Function to process all the video that are contain in the same folder
    void process_all_videos_folder(std::string path, bool show_output = false);
};