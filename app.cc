#include "app.h"
#include "frame.h"

bool CarvrApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }
    SetAppDisplayName(GetAppName());

    CarvrFrame* frame = new CarvrFrame("carvr");
    frame->Show(true);

    return true;
}
