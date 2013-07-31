#include <wx/wx.h>

// Forward declaration of ImagePanel
class ImagePanel;

class CarvrFrame : public wxFrame
{
public:
    CarvrFrame(const wxString& title);
    void OnQuit(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
private:
    ImagePanel* const panel;
};
