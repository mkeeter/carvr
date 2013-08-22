#ifndef APP_H
#define APP_H

#include <list>
#include "wx/wx.h"

class CarvrFrame;

class CarvrApp : public wxApp
{
public:
    virtual bool OnInit();
    void OnQuit(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void RemoveFrame(CarvrFrame* frame);
private:
    std::list<CarvrFrame*> frames;
};

#endif
