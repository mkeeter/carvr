#include <wx/wx.h>

class ImagePanel : public wxPanel
{
public:
    ImagePanel(wxFrame* parent);
    void OnPaint(wxPaintEvent& event);
};
