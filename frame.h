#include <wx/wx.h>

class CarvrFrame : public wxFrame
{
public:
    CarvrFrame(const wxString& title);
    void OnQuit(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
};
