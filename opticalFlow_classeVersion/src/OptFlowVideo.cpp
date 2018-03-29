#include "OptFlowVideo.hpp"

// Constructors initialize all the parameters in order to calculate the optical flow
// Each parameters can be set separately by the user
OptFlowVideo::OptFlowVideo()
{
    this->filename_ = "";
    this->change_name_ = false;
    this->max_features_ = 100;
    this->quality_level_ = 0.01;
    this->min_distance_ = 10;
    this->block_size_ = 3;
    this->use_harris_detector_ = false;
    this->win_size_ = Size(15,15);
    this->max_level_pyramids_ = 3;
    this->term_crit_ = TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,10,0.03);
}

OptFlowVideo::OptFlowVideo(string filename_temp)
{
    set_filename(filename_temp);
    this->change_name_ = true;
    this->max_features_ = 100;
    this->quality_level_ = 0.01;
    this->min_distance_ = 10;
    this->block_size_ = 3;
    this->use_harris_detector_ = false;
    this->win_size_ = Size(15,15);
    this->max_level_pyramids_ = 3;
    this->term_crit_ = TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,10,0.03);
}

void OptFlowVideo::set_filename(string filename_temp)
{
    boost::filesystem::path video_path(filename_temp);
    boost::filesystem::path current_path(video_path);
    current_path.remove_filename();
    this->path_to_data_folder_ = current_path.string() + "/" + video_path.stem().string();

    boost::filesystem::create_directory(this->path_to_data_folder_);

    this->path_to_data_folder_ += "/";

    this->filename_ = filename_temp;
    this->change_name_ = true;
    
}

/* Function to load video (private)
*  Will load the video only if it's not opened or if it's a new video
*  Return: 0 if successful, else 1 (with error message)
*/
int OptFlowVideo::load_video()
{
    if(!this->video_.isOpened() || this->change_name_)
    {
        this->video_.open(this->filename_);
        this->change_name_ = false;
        // Verify if the video is correctly loaded
        if (!this->video_.isOpened()) {
            std::cerr << "Failed to open the video device, video file or image sequence!\n" << std::endl;
            return 1;
        }
    }

    return 0;
}

/* Function to search_init_features_ (private)
*  Will search the good features in the video with the openCV function goodFeaturesToTrack
*  Return: 0 if successful, else 1 (with error message)
*/
int OptFlowVideo::search_init_features()
{ 
    // Verify if the video is loaded
    if(load_video())
    {
        std::cerr << "No video loaded!\n" << std::endl;
        return 1;
    }

    // 2 mat to represent image / frame of the video
    Mat gray, frame;

    // initialisation : save the first frame of the video
    this->video_ >> this->first_frame_;
    // If the first frame is empty -> return 1 with error message
    if( this->first_frame_.empty() )
    {
        std::cerr << "Error with the video, first frame empty\n" << std::endl;
        return 1;
    }
    // Get the gray image
    cvtColor(this->first_frame_, gray, COLOR_BGR2GRAY);

    // Search the features
    goodFeaturesToTrack(gray, this->init_features_, this->max_features_, this->quality_level_, this->min_distance_, Mat(), this->block_size_, this->block_size_, this->use_harris_detector_, 0.04);

    return 0;
}

/* Function to get the first frame of the video with the location of the features (public)
*  Will create an image file at the location chosen by the user that contains the good features
*  Return:
*/
void OptFlowVideo::write_image_with_init_features(bool show_output)
{
    // Verify if the video is loaded (or changed) or if the features has been already found
    if(this->init_features_.empty() || this->change_name_)
    {
        // If there is an issue then search the features
        if(search_init_features())
        {
            std::cerr << "Error with the video -> Frame empty" << std::endl;
            return;
        }
    }

    // Get the first frame of the video
    Mat first_frame_temp;
    this->first_frame_.copyTo(first_frame_temp);

    // Add circle representing the features on the first frame of the video
    int i = 0, k = 0;
    for( i = k = 0; i < this->init_features_.size(); i++ )
    {
            this->init_features_[k++] = this->init_features_[i];
            circle( first_frame_temp, this->init_features_[i], 3, Scalar(0,255,0), -1, 8);
    }

    // If the user want to open the see the output at the execution time
    if(show_output)
    {
        namedWindow( "initFeatures", 1 );
        imshow("initFeatures", first_frame_temp);
        waitKey(1000);
    }

    // Write in the correct file the image
    imwrite( this->path_to_data_folder_ + "initFeatures.jpg", first_frame_temp );
    std::cout<<"First frame with init features has been written at : "<<this->path_to_data_folder_<<"initFeatures.jpg"<<std::endl<<std::endl;
}

/* Function to get the first frame of the video with the optical flow of each features (public)
*  Will create an image file at the location chosen by the user that contains the entire optical flow of each features
*  Return:
*/
void OptFlowVideo::write_image_with_optical_flow(bool show_output)
{
    // Verify if the video is loaded (or changed) or if the features has been already found
    if(this->init_features_.empty() || this->change_name_)
    {
        // If there is an issue then search the features
        if(search_init_features())
        {
            std::cerr << "Error with the video -> Frame empty" << std::endl;
            return;
        }
    }

    // Needed matrix
    Mat first_frame_temp, frame, image, gray, prev_gray;

    // Get the first frame of the video
    this->first_frame_.copyTo(first_frame_temp);

    // tab of vector of Points2f : one for the previous location of the features, one for the newest
    vector<Point2f> points[2];

    // Get the init features
    points[0] = this->init_features_;

    int i = 0, k = 0;

    if(show_output)
        namedWindow( "Video with Features", 1 );

    // This while loop will be break at the end of the video
    while(1)
    {
        // Get the next frame
        this->video_ >> frame;
        // If empty that means this is the end of the video then break the loop
        if( frame.empty() )
            break;
        
        // Get the gray image
        frame.copyTo(image);
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // Initialisation for the gray images
        if(prev_gray.empty())
            gray.copyTo(prev_gray);

        // Compute the optical flow for each features
        // Put in points[1] the new locations of each features
        vector<uchar> status;
        vector<float> err;
        calcOpticalFlowPyrLK(prev_gray, gray, points[0], points[1], status, err, this->win_size_, this->max_level_pyramids_, this->term_crit_, this->use_harris_detector_, 0.001);

        // For loop in order to draw the optical flow and features on the images
        for( i = k = 0; i < points[1].size(); i++ )
        {
            if( !status[i] )
                continue;
            // Draw a circle for the featurs
            circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
            // Draw a line for the optical flow
            line(first_frame_temp, points[1][i],points[0][i], Scalar(255,0,0));
        }

        // If the user wants to see the output (position of the features at each frame) while running
        if(show_output)
        {
            imshow("Video with Features", image);
            char c = (char)waitKey(10);
            if( c == 27 )
                break;
        }

        // New locations of the features become the old location
        std::swap(points[1], points[0]);
        // Same thing for the frames
        cv::swap(prev_gray, gray);
    }

    // If the user wants to see the output (first frame with optical flow for each feature) while running
    if(show_output)
    {
        namedWindow( "First Frame with Optical Flow", 1 );
        imshow("First Frame with Optical Flow", first_frame_temp);
        waitKey(1000);
    }

    // Write the first frame with optical flow for each features in the correct file
    imwrite( this->path_to_data_folder_ + "optFlow.jpg", first_frame_temp );

    // Reload the video (it's now at the end of the video so it need to be reloaded if the user wants to use another function right after this one)
    this->video_.open(this->filename_);
    if (!this->video_.isOpened()) {
        std::cerr << "Failed to open the video device, video file or image sequence!\n" << std::endl;
        return;
    }

    std::cout<<"First frame with optical flow (LK) has been written at : "<<this->path_to_data_folder_<<"optFlow.jpg"<<std::endl<<std::endl;
}

/* Function to get the video with the vectors reprensenting the optical flow of each features at each frame (public)
*  Will create a video file at the location chosen by the user
*  Return:
*/
void OptFlowVideo::write_vector_video(bool show_output, int fps)
{
    // Verify if the video is loaded (or changed) or if the features has been already found
    if(this->init_features_.empty() || this->change_name_)
    {
        // If there is an issue then search the features
        if(search_init_features())
        {
            std::cerr << "Error with the video -> Frame empty" << std::endl;
            return;
        }
    }

    // Needed matrix
    Mat first_frame_temp, frame, image, gray, prev_gray;
    // Get the first frame of the image
    this->first_frame_.copyTo(first_frame_temp);

    // Tab of vector of Points2f : one for the previous location of the features, one for the newest
    vector<Point2f> points[2];
    // Points representing the vector between the new location of a feature and the old location
    Point2f vector_flow;
    // Get the location of the init features
    points[0] = this->init_features_;

    int i = 0, k = 0;

    if(show_output)
        namedWindow("Video with vectors of Optical Flow");

    // Create a VideoWriter with the correct parameters and outputPath
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    writer.open(this->path_to_data_folder_ + "optFlowVectors.avi", codec, fps, first_frame_temp.size());
    // check if the VideoWriter is correctly opened
    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return;
    }

    // This while loop will be break at the end of the video
    while(1)
    {
        // Get the new frame of the video
        this->video_ >> frame;
        // If the frame is empty then this is the end of the video
        if( frame.empty() )
            break;
        
        // Get the gray scale image
        frame.copyTo(image);
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // Initialisation of the gray scales images
        if(prev_gray.empty())
            gray.copyTo(prev_gray);

        // Compute the optical flow for each feature
        vector<uchar> status;
        vector<float> err;
        calcOpticalFlowPyrLK(prev_gray, gray, points[0], points[1], status, err, this->win_size_, this->max_level_pyramids_, this->term_crit_, this->use_harris_detector_, 0.001);

        // For loop to draw the vectors on each frame
        for( i = k = 0; i < points[1].size(); i++ )
        {
            if( !status[i] )
                continue;
            // Compute the vector of the optical flow between two frames
            vector_flow = (points[1][i] - points[0][i])*20.0;
            // Draw arrowed line the correct frame
            arrowedLine(image, points[0][i],points[0][i] + vector_flow, Scalar(255,0,0),2);
        }

        // Encode the frame into the videofile stream
        writer.write(image);

        // Show the output at runtime if need by the user
        if(show_output)
        {
            imshow("Video with vectors of Optical Flow", image);
            if (waitKey(5) >= 0)
                break;
        }
        // New locations of features become the old one
        std::swap(points[1], points[0]);
        // Same thing for the gray frame
        cv::swap(prev_gray, gray);
    }

    // Reload the video (the stream of the video is at the end so it's need to be reloaded if the user wants to use another function right after)
    this->video_.open(this->filename_);
    if (!this->video_.isOpened()) {
        std::cerr << "Failed to open the video device, video file or image sequence!\n" << std::endl;
        return;
    }

    std::cout<<"Video with vector representing optical flow has been written at : "<<this->path_to_data_folder_<<"optFlowVectors.avi"<<std::endl<<std::endl;
}

/* Function to release the video if it's no longer needed. The video is also released by the destructor of VideoCapture (public)
*  Return:
*/
void OptFlowVideo::release_video()
{
    this->video_.release();
    this->init_features_.erase (this->init_features_.begin(),this->init_features_.end());
}

