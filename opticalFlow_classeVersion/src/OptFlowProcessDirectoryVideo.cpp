#include "ProcessDirectoryVideo.hpp"

OptFlowProcessDirectoryVideo::OptFlowProcessDirectoryVideo(){}

void OptFlowProcessDirectoryVideo::process_all_videos_folder(std::string path, bool show_output)
{

    boost::filesystem::path someDir(path);
    boost::filesystem::directory_iterator end_iter;

    vector<boost::filesystem::path> result_set;

    for( boost::filesystem::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
    {
            result_set.push_back(dir_iter->path());

    }

    OptFlowVideo video_to_process;

    for(int i = 0; i < result_set.size(); i++)
    {
        video_to_process.set_filename(result_set.at(i).string());
        video_to_process.write_image_with_init_features(show_output);
        video_to_process.write_image_with_optical_flow(show_output);
        video_to_process.write_vector_video(show_output);
        video_to_process.release_video();
    }
}