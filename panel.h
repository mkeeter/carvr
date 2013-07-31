#ifndef PANEL_H
#define PANEL_H

#include <wx/wx.h>

class ImagePanel : public wxPanel
{
public:
    ImagePanel(wxFrame* parent);
    void OnPaint(wxPaintEvent& event);
    void LoadImage(wxString filename);
};

#endif
