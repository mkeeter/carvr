#include "app.h"
#include "frame.h"

////////////////////////////////////////////////////////////////////////////////

bool CarvrApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }
    SetAppDisplayName(GetAppName());

    frames.push_back(new CarvrFrame());

    return true;
}

////////////////////////////////////////////////////////////////////////////////

void CarvrApp::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    std::list<CarvrFrame*>::iterator itr;
    for (itr = frames.begin(); itr != frames.end(); ++itr) {
        (**itr).Close();
    }
}

////////////////////////////////////////////////////////////////////////////////

void CarvrApp::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* open_dialog = new wxFileDialog(
            frames.back(), "Choose a file", "", "",
            "*.png|*.jpg|*.jpeg|*.bmp",
            wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

    if (open_dialog->ShowModal() == wxID_OK) {
        const std::string filename = open_dialog->GetPath().ToStdString();
        if (frames.back()->ImageLoaded()) {
            frames.push_back(new CarvrFrame(filename));
        } else {
            frames.back()->LoadImage(filename);
        }
    }

    open_dialog->Destroy();
}

////////////////////////////////////////////////////////////////////////////////

void CarvrApp::RemoveFrame(CarvrFrame* frame)
{
    frames.remove(frame);
}
