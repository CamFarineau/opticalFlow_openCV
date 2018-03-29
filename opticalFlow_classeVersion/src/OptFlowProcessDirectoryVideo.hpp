#include <boost/filesystem.hpp>
#include "OptFlowVideo.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace boost::filesystem;

class ProcessDirectoryVideo{
    
    public:
    ProcessDirectoryVideo();
    void process_all_videos_folder(std::string path, bool show_output = false);
};