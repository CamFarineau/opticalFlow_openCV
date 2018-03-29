#include "opt_flow_process_directory_video.hpp"

OptFlowProcessDirectoryVideo::OptFlowProcessDirectoryVideo(){}

// Function to process all the videos that are all in the same folder
// Parameters: path -> path to the folder, show_output (optional): show the results while running
void OptFlowProcessDirectoryVideo::process_all_videos_folder(std::string path, bool show_output)
{
    // Get the path to the dir
    boost::filesystem::path someDir(path);
    boost::filesystem::directory_iterator end_iter;

    // Vector of path that will contains all the videos
    vector<boost::filesystem::path> result_set;

    // Go through all the files in the folder
    for( boost::filesystem::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
    {
        // Insert the path of each video into the vector of path
        result_set.push_back(dir_iter->path());
    }

    // Create an OptFlowVideo
    OptFlowVideo video_to_process;

    // For each video in the folder, process everything
    for(int i = 0; i < result_set.size(); i++)
    {
        // Set the correct filename of the video in question
        video_to_process.set_filename(result_set.at(i).string());
        // Get the image with the features
        video_to_process.write_image_with_init_features(show_output);
        // Get the image with the optical flow
        video_to_process.write_image_with_optical_flow(show_output);
        // Get the video with the vector representing the optical flow
        video_to_process.write_vector_video(show_output);
        // Release the video at the end of the processing
        video_to_process.release_video();
    }
}