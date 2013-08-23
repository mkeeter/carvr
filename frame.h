#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <wx/wx.h>

// Forward declarations
class ImagePanel;
class Image;

class CarvrFrame : public wxFrame
{
public:
    CarvrFrame(std::string filename="");
    ~CarvrFrame();
    const Image* GetImage() const;
    void LoadImage(std::string filename);
private:
    void OnClose(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);

    ImagePanel* const panel;
};

#endif
