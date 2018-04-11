#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

int main( int argc, const char* argv[] )
{
	int height = 500,width = 800;
    Mat first_frame(height, width, CV_8UC3, Scalar(255,255,255));
    Mat image_temp(height, width, CV_8UC3, Scalar(255,255,255));


    // Create a VideoWriter with the correct parameters and outputPath
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    writer.open("/home/camille/work/opticalFlow_openCV/generate_test_video/data/lines_straf_down_slow.avi", codec, 25, first_frame.size());
    // check if the VideoWriter is correctly opened
    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return 1;
    }
    int speed = 1;
    int nb_frames = 100 * speed;
    int init_i = 0, init_j = 0, end_i = 200, end_j = 200, rect_height = 100, rect_width = 50;

    for(int k = 0; k < nb_frames; k = k+speed)
    {
        first_frame.copyTo(image_temp);

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width ; j++)
            {
                if((i >= init_i + k && i <= init_i + rect_width + k) ||
                    (i >= init_i + rect_width*2 + k && i <= init_i + rect_width*3 + k) ||
                    (i >= init_i + rect_width*4 + k && i <= init_i + rect_width*5 + k) ||
                    (i >= init_i + rect_width*6 + k && i <= init_i + rect_width*7 + k) ||
                    (i >= init_i + rect_width*8 + k && i <= init_i + rect_width*9 + k) ||
                    (i >= init_i + rect_width*10 + k && i <= init_i + rect_width*11 + k) ||
                    (i >= init_i + rect_width*12 + k && i <= init_i + rect_width*13 + k))    
                    image_temp.at<Vec3b>(i, j) = Vec3b(0,0,255);  
            }
        }
        // Encode the frame into the videofile stream
        writer.write(image_temp);
        imshow("video",image_temp);
        waitKey(10);
    }





    destroyAllWindows();
    return 0;
}