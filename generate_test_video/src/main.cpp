#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

int main( int argc, const char* argv[] )
{
	int height = 1000,width = 1000;
    Mat first_frame(height, width, CV_8UC3, Scalar(255,255,2550));
    Mat image_temp(height, width, CV_8UC3, Scalar(255,255,255));

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width ; j++)
        {
            if(i >= 350 && i <= 650 && j >= 250 && j <= 750)    
                first_frame.at<Vec3b>(i, j) = Vec3b(0,0,255);  
        }
    }

    // Create a VideoWriter with the correct parameters and outputPath
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    writer.open("/home/camille/work/opticalFlow_openCV/generate_test_video/data/rectangle_rotation_megaslow.avi", codec, 25, first_frame.size());
    // check if the VideoWriter is correctly opened
    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return 1;
    }


    int speed = 1;
    int nb_frames = 700 * speed;
    int init_i = 100, init_j = 200, end_i = 200, end_j = 200, rect_height = 100, rect_width = 150;

    for(int k = 0; k < nb_frames; k = k+speed)
    {
        first_frame.copyTo(image_temp);

        // Translation rectangle
        // for(int i = 0; i < height; i++)
        // {
        //     for(int j = 0; j < width ; j++)
        //     {
        //         if(i >= init_i + k && i <= init_i + rect_height + k && j >= init_j + k && j <= init_j + rect_width + k)    
        //             image_temp.at<Vec3b>(i, j) = Vec3b(0,0,255);  
        //     }
        // }

        // Rotation
        Mat mat_rotation = getRotationMatrix2D(Point(width/2,height/2),(double)k/8.f,1);
        Mat img_rotated;
        warpAffine( image_temp, img_rotated, mat_rotation, image_temp.size(), INTER_LINEAR, BORDER_REFLECT );

        // for(int i = 0; i < height; i++)
        // {
        //     for(int j = 0; j < width ; j++)
        //     {
        //         if(img_rotated.at<Vec3b>(i, j) == Vec3b(0,0,0))    
        //             img_rotated.at<Vec3b>(i, j) = Vec3b(255,255,255);  
        //     }
        // }

        // Encode the frame into the videofile stream
        writer.write(img_rotated);
        imshow("video",img_rotated);
        waitKey(10);
    }





    destroyAllWindows();
    return 0;
}