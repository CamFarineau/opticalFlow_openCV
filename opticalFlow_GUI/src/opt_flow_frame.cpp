#include "opt_flow_frame.hpp"

// Constructors initialize all the parameters in order to calculate the optical flow
// Each parameters can be set separately by the user
OptFlowFrame::OptFlowFrame()
{
    this->directory_path_ = "";
    this->change_directory_ = false;
    this->max_features_ = 100;
    this->quality_level_ = 0.01;
    this->min_distance_ = 10;
    this->block_size_ = 3;
    this->use_harris_detector_ = false;
    this->win_size_ = Size(15,15);
    this->max_level_pyramids_ = 3;
    this->term_crit_ = TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,10,0.03);
}

OptFlowFrame::OptFlowFrame(string directory_path_temp)
{
    set_directory_path(directory_path_temp);
    this->change_directory_ = true;
    this->max_features_ = 100;
    this->quality_level_ = 0.01;
    this->min_distance_ = 10;
    this->block_size_ = 3;
    this->use_harris_detector_ = false;
    this->win_size_ = Size(15,15);
    this->max_level_pyramids_ = 3;
    this->term_crit_ = TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,10,0.03);
}

// bool is_not_digit(char c)
// {
//     return !std::isdigit(c);
// }

// bool numeric_string_compare(const std::string& s1, const std::string& s2)
// {
//     // handle empty strings...

//     std::string::const_iterator it1 = s1.begin(), it2 = s2.begin();

//     if (std::isdigit(s1[0]) && std::isdigit(s2[0])) {
//         int n1, n2;
//         std::stringstream ss(s1);
//         ss >> n1;
//         ss.clear();
//         ss.str(s2);
//         ss >> n2;

//         if (n1 != n2) return n1 < n2;

//         it1 = std::find_if(s1.begin(), s1.end(), is_not_digit);
//         it2 = std::find_if(s2.begin(), s2.end(), is_not_digit);
//     }

//     return std::lexicographical_compare(it1, s1.end(), it2, s2.end());
// }

void OptFlowFrame::set_directory_path(string directory_path_temp)
{
    this->directory_path_ = directory_path_temp;
    this->change_directory_ = true;
}

/* Function to load all the frames (private)
*  Will load the frames only if it's not opened or if it's a new set of frames
*  Return: 0 if successful, else 1 (with error message)
*/
int OptFlowFrame::load_frames()
{
    if(this->frames_.empty() || this->change_directory_)
    {
        // Get the path to the dir
        boost::filesystem::path frames_dir(this->directory_path_);

        if(exists(frames_dir))
        {
            boost::filesystem::directory_iterator end_iter;
            // Vector of path that will contains all path to the of frames. Necessary because it will get the frame not sorted
            vector<boost::filesystem::path> frames_set;
            Mat image_temp;
            // Go through all the files in the folder
            for( boost::filesystem::directory_iterator dir_iter(frames_dir) ; dir_iter != end_iter ; ++dir_iter)
            {
                // Insert the path of each frames into the vector of path
                frames_set.push_back(dir_iter->path());
            }
            std::sort(frames_set.begin(), frames_set.end());

            for(int i; i < frames_set.size(); i++ )
            {
                image_temp = imread(frames_set.at(i).string(), CV_LOAD_IMAGE_COLOR);   // Read the file
                std::cout<<"Path: "<<frames_set.at(i).string()<<std::endl;
                if(! image_temp.data )  // Check for invalid input
                {
                    cout <<  "Could not open or find the image: " << frames_set.at(i).string() << std::endl ;
                    return 1;
                }
                else
                {
                    this->frames_.push_back(image_temp);
                }
            }

            if(frames_.empty())
            {
                std::cout<<"**********************************************************************"<<std::endl;
                std::cout<<"Directory is empty !"<<std::endl;
                std::cout<<"**********************************************************************"<<std::endl;
                return 1;
            }
            else
            {
                this->filename_ = frames_dir.stem().string();
                boost::filesystem::create_directory(this->directory_path_ + "/results");
            }
            this->change_directory_ = false;
        }
        

    }

    return 0;
}

/* Function to search_init_features_ (private)
*  Will search the good features in the first frame with the openCV function goodFeaturesToTrack
*  Return: 0 if successful, else 1 (with error message)
*/
int OptFlowFrame::search_init_features()
{ 
    // Verify if the video is loaded
    if(load_frames())
    {
        std::cerr << "No frame loaded!\n" << std::endl;
        return 1;
    }

    // 2 mat to represent image / frame of the video
    Mat gray;

    // Get the gray image
    cvtColor(this->frames_.at(0) , gray, COLOR_BGR2GRAY);

    // Search the features
    goodFeaturesToTrack(gray, this->init_features_, this->max_features_, this->quality_level_, this->min_distance_, Mat(), this->block_size_, this->block_size_, this->use_harris_detector_, 0.04);

    return 0;
}

/* Function to write the first frame with the location of the features (public)
*  Will create an image file at the location chosen by the user that contains the good features
*  Return:
*/
void OptFlowFrame::write_first_frame_with_init_features(bool show_output)
{
    // Verify if the frames are loaded (or changed) or if the features has been already found
    if(this->init_features_.empty() || this->change_directory_)
    {
        // If there is an issue then search the features
        if(search_init_features())
        {
            std::cerr << "Error with the frames -> No frames to process" << std::endl;
            return;
        }
    }

    // Get the first frame of the video
    Mat first_frame_temp;
    this->frames_.at(0).copyTo(first_frame_temp);

    // Add circle representing the features on the first frame of the video
    int i = 0;
    for( i = 0; i < this->init_features_.size(); i++ )
    {
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
    imwrite(this->directory_path_ + "/results/" + this->filename_ + "_init_features.jpg", first_frame_temp );
    std::cout<<"First frame with init features has been written at : "<<this->directory_path_ + "/" + this->filename_ + "_init_features.jpg"<<std::endl<<std::endl;
}

/* Function to write the first frame with the optical flow of each features (public)
*  Will create an image file at the location chosen by the user that contains the entire optical flow of each features
*  Return:
*/
void OptFlowFrame::write_first_frame_with_optical_flow(bool show_output)
{
    // Verify if the frames are loaded (or changed) or if the features has been already found
    if(this->init_features_.empty() || this->change_directory_)
    {
        // If there is an issue then search the features
        if(search_init_features())
        {
            std::cerr << "Error with the frames -> No frames to process" << std::endl;
            return;
        }
    }

    // Needed matrix
    Mat first_frame_temp, frame, image, gray, prev_gray;

    // Get the first frame of the video
    this->frames_.at(0).copyTo(first_frame_temp);

    // tab of vector of Points2f : one for the previous location of the features, one for the newest
    vector<Point2f> points[2];

    // Get the init features
    points[0] = this->init_features_;

    int i = 0;

    if(show_output)
        namedWindow( "Video with Features", 1 );

    // This for loop will be break until the last frame
    for(int j = 0; j < this->frames_.size(); j++)
    {
        // Get the next frame
        this->frames_.at(j).copyTo(image);
        // If empty that means this is the end of the video then break the loop
        if( image.empty() )
            break;
        
        // Get the gray image
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
        for( i = 0; i < points[1].size(); i++ )
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
    imwrite(this->directory_path_ + "/results/" + this->filename_ + "_opt_flow.jpg", first_frame_temp );

    std::cout<<"First frame with optical flow (LK) has been written at : "<< this->directory_path_ + "/results/" + this->filename_ + "_opt_flow.jpg"<<std::endl<<std::endl;
}

/* Function to get the video recomposed with the vectors reprensenting the optical flow of each features at each frame (public)
*  Will create a video file from the frames at the location chosen by the user
*  Return:
*/
void OptFlowFrame::write_vector_video(bool write_json_vector, bool show_output, int fps)
{
    // Verify if the frames are loaded (or changed) or if the features has been already found
    if(this->init_features_.empty() || this->change_directory_)
    {
        // If there is an issue then search the features
        if(search_init_features())
        {
            std::cerr << "Error with the frames -> No frames to process" << std::endl;
            return;
        }
    }

    // Needed matrix
    Mat first_frame_temp, frame, image, gray, prev_gray;
    // Get the first frame of the image
    this->frames_.at(0).copyTo(first_frame_temp);

    // Tab of vector of Points2f : one for the previous location of the features, one for the newest
    vector<Point2f> points[2];
    // Points representing the vector between the new location of a feature and the old location
    vector<vector<float>> vector_flow(2,vector<float>(this->max_features_,0.0f));
    vector<vector<float>> vector_polar(2,vector<float>(this->max_features_,0.0f));
    // Get the location of the init features
    points[0] = this->init_features_;

    int i = 0;

    if(show_output)
        namedWindow("Video with vectors of Optical Flow");

    // Create a VideoWriter with the correct parameters and outputPath
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    writer.open(this->directory_path_ + "/results/" + this->filename_ + "_opt_flow_vectors.avi", codec, fps, first_frame_temp.size());
    // check if the VideoWriter is correctly opened
    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return;
    }

    // Create a property_tree that will contains the vectors for each frames
    boost::property_tree::ptree pt, pt_general;
    // Counter to know which frame it's processing
    int cpt = 1;
    // This for loop will be break until the last frame
    for(int j = 0; j < this->frames_.size(); j++)
    {
        // Get the new frame of the video
        this->frames_.at(j).copyTo(image);
        // If the frame is empty then this is the end of the video
        if( image.empty() )
            break;
        
        // Get the gray scale image
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // Initialisation of the gray scales images
        if(prev_gray.empty())
            gray.copyTo(prev_gray);

        // Compute the optical flow for each feature
        vector<uchar> status;
        vector<float> err;
        calcOpticalFlowPyrLK(prev_gray, gray, points[0], points[1], status, err, this->win_size_, this->max_level_pyramids_, this->term_crit_, this->use_harris_detector_, 0.001);

        // For loop to draw the vectors on each frame
        for( i = 0; i < points[1].size(); i++ )
        {
            if( !status[i] )
                continue;
            // Compute the vector of the optical flow between two frames
            vector_flow.at(0).at(i) = (points[1][i].x - points[0][i].x);
            vector_flow.at(1).at(i) = (points[1][i].y - points[0][i].y);
            // Draw arrowed line the correct frame
            arrowedLine(image, points[0][i],points[0][i] + Point2f(vector_flow[0][i],vector_flow[1][i])*20.0, Scalar(255,0,0),2);
        }

        // If the user wants the json file
        if(write_json_vector)
        {
            cartToPolar(vector_flow.at(0),vector_flow.at(1),vector_polar.at(0),vector_polar.at(1),false);
            // Create a tree for each frame and vector
            boost::property_tree::ptree frame,vectors;
            // Add for each feature it's vector for this particular frame
            for( i = 0; i < points[1].size(); i++ )
            {
                vectors.put("feature " + std::to_string(i+1),Point2f(vector_polar.at(0).at(i),vector_polar.at(1).at(i)));
            }
            // Put the correct frame number
            frame.put("number", cpt);
            // Add the vector has a child
            frame.add_child("vectors", vectors);
            // Add this frame tree to the general tree
            pt_general.push_back(std::make_pair("", frame));
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

        cpt++;
    }

    if(write_json_vector)
    {
        // Add the general tree to the main tree as a child
        pt.add_child("Frame", pt_general);
        // Write the JSON at the correct location
        boost::property_tree::write_json(this->directory_path_ + "/results/" + this->filename_ + "_vectors.json", pt);
        std::cout<<"JSON file of vectors of each features at each frames can be found at: "<<this->directory_path_ + "/results/" + this->filename_ + "_vectors.json"<<std::endl<<std::endl;
    }

    std::cout<<"Video with vector representing optical flow has been written at : "<<this->directory_path_ + "/results/" + this->filename_ + "_opt_flow_vectors.avi"<<std::endl<<std::endl;
}

/* Function to release the vector of frames if it's no longer needed.(public)
*  It will only remove all elements from the vector
*  Return:
*/
void OptFlowFrame::release_vector_frames()
{
    this->frames_.erase(this->frames_.begin(),this->frames_.end());
    this->init_features_.erase (this->init_features_.begin(),this->init_features_.end());
}
