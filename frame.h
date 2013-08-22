#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <wx/wx.h>

// Forward declaration of ImagePanel
class ImagePanel;

class CarvrFrame : public wxFrame
{
public:
    CarvrFrame();
    ~CarvrFrame();
    void LoadImage(std::string filename);
    bool ImageLoaded() const;
private:
    void OnClose(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);

    ImagePanel* const panel;
};

#endif
