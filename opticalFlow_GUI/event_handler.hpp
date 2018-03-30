#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include "opt_flow_process_directory_video.hpp"


class EventHandler{
    public:
    EventHandler();
    void set_parameters(const nana::arg_textbox& ei);

    private:
    OptFlowProcessDirectoryVideo dir;


};

#endif