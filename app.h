#ifndef APP_H
#define APP_H

#include "wx/wx.h"

class CarvrFrame;

class CarvrApp : public wxApp
{
public:
    virtual bool OnInit();
    void OnQuit(wxCommandEvent& event);
private:
    CarvrFrame* frame;
};

#endif
