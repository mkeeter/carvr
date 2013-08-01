#ifndef FRAME_H
#define FRAME_H

#include <wx/wx.h>

// Forward declaration of ImagePanel
class ImagePanel;

class CarvrFrame : public wxFrame
{
public:
    CarvrFrame(const wxString& title);
    void OnQuit(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnMouseLDown(wxMouseEvent& event);
    void OnMouseLUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
private:
    ImagePanel* const panel;
    float scale;
    wxSize max_size;

    enum { NONE, HORIZONTAL, VERTICAL } drag_mode;
    wxPoint mouse_position;
};

#endif
