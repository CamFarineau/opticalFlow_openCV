#include "gui.hpp"
#include <iostream>

Gui::Gui(){}


void Gui::set_parameters()
{
    this->directory.video_to_process.set_max_features(std::stoi(this->max_features_box.caption()));
    this->directory.video_to_process.set_quality_level(std::atof(this->quality_level_box.caption().c_str()));
    this->directory.video_to_process.set_min_distance(std::stoi(this->min_distance_box.caption()));
    this->directory.video_to_process.set_block_size(std::stoi(this->block_size_box.caption()));
    this->directory.video_to_process.set_win_size(cv::Size(std::stoi(this->win_size_height_box.caption()),std::stoi(this->win_size_width_box.caption())));
    this->directory.video_to_process.set_max_level_pyramids(std::stoi(this->max_level_pyramids_box.caption()));
    this->directory.video_to_process.set_use_harris_detector(this->use_harris_detector_box.checked());


    this->directory.process_all_videos_folder(this->path_box.caption());
}

void Gui::init_gui()
 {  
    this->fm.size(size(500,500));
    this->fm.caption("Optical Flow Process Directory");

    this->path_label.caption("Path to folder");
    this->path_label.bgcolor(colors::azure);

    this->max_features_label.caption("Max features");
    this->max_features_label.bgcolor(colors::azure);

    this->quality_level_label.caption("Quality Level");
    this->quality_level_label.bgcolor(colors::azure);

    this->min_distance_label .caption("Min distance");
    this->min_distance_label.bgcolor(colors::azure);

    this->block_size_label .caption("Block size");
    this->block_size_label.bgcolor(colors::azure);

    this->win_size_height_label.caption("Win size: Height");
    this->win_size_height_label.bgcolor(colors::azure);

    this->win_size_width_label.caption("Win size: Width");
    this->win_size_width_label.bgcolor(colors::azure);
    
    this->max_level_pyramids_label.caption("Max Level Pyramids");
    this->max_level_pyramids_label.bgcolor(colors::azure);

    this->use_harris_detector_label.caption("Use Harris detector");
    this->use_harris_detector_label.bgcolor(colors::azure);

    

    this->path_box.tip_string("/home/..."    ).multi_lines(false);
    this->max_features_box.caption(std::to_string(this->directory.video_to_process.get_max_features())    );
    this->quality_level_box.caption(std::to_string(this->directory.video_to_process.get_quality_level())    );
    this->min_distance_box.caption(std::to_string(this->directory.video_to_process.get_min_distance())    );
    this->block_size_box.caption(std::to_string(this->directory.video_to_process.get_block_size())    );
    this->win_size_height_box.caption(std::to_string(this->directory.video_to_process.get_win_size().height ));
    this->win_size_width_box.caption(std::to_string(this->directory.video_to_process.get_win_size().width ));
    this->max_level_pyramids_box.caption(std::to_string(this->directory.video_to_process.get_max_level_pyramids())    );

    this->btn_start.caption("Start");
    this->btn_start.events().click(std::bind( &Gui::set_parameters, this));

    this->btn_quit.caption("Quit");
    this->btn_quit.events().click(API::exit_all);


    //The div-text
    this->layout.div("<><weight=40% vertical<><weight=70% vertical <vertical gap=10 labels arrange=[25,25,25,25,25,25,25,25,25]> <> <weight=25 gap=10 button_start> ><>><weight=40% vertical<><weight=70% vertical <vertical gap=10 textboxs arrange=[25,25,25,25,25,25,25,25,25]> <> <weight=25 gap=10 button_quit> ><>><>");
    this->layout.field("labels") << this->path_label << this->max_features_label << this->quality_level_label << this->min_distance_label << this ->block_size_label << this->win_size_height_label << this->win_size_width_label << this->max_level_pyramids_label << this->use_harris_detector_label;
    this->layout.field("textboxs") << this->path_box << this->max_features_box << this->quality_level_box << this->min_distance_box << this ->block_size_box << this->win_size_height_box << this->win_size_width_box << this->max_level_pyramids_box << this->use_harris_detector_box;
    this->layout.field("button_start") << this->btn_start;
    this->layout.field("button_quit") << this->btn_quit;
    this->layout.collocate();

    this->fm.show();
    exec();
 }