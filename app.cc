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
            wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR | wxFD_MULTIPLE);

    if (open_dialog->ShowModal() == wxID_OK) {
        wxArrayString filenames;
        open_dialog->GetPaths(filenames);

        for (size_t f=0; f < filenames.GetCount(); ++f) {
            const std::string filename = filenames[f].ToStdString();
            if (frames.back()->ImageLoaded()) {
                frames.push_back(new CarvrFrame(filename));
            } else {
                frames.back()->LoadImage(filename);
            }
        }
    }

    open_dialog->Destroy();
}

////////////////////////////////////////////////////////////////////////////////

void CarvrApp::RemoveFrame(CarvrFrame* frame)
{
    frames.remove(frame);
}
