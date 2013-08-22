#include "app.h"
#include "frame.h"

bool CarvrApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }
    SetAppDisplayName(GetAppName());

    frame = new CarvrFrame();
    frame->Show(true);

    return true;
}

void CarvrApp::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    frame->Close();
}
