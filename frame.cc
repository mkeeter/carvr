#include "frame.h"
#include "panel.h"

CarvrFrame::CarvrFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
      panel(new ImagePanel(this))
{
    wxMenuBar* const menu_bar = new wxMenuBar;
    wxMenu* const file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("Quit\tCTRL-Q"));
    file_menu->Append(wxID_OPEN, _("Open\tCTRL-O"));
    menu_bar->Append(file_menu, _("File"));
    SetMenuBar(menu_bar);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnOpen, this, wxID_OPEN);

    //panel->Bind(wxEVT_MOTION, &CarvrFrame::OnMouseMove,  this);
    //panel->Bind(wxEVT_LEFT_DOWN, &CarvrFrame::OnMouseLDown,  this);
    //panel->SetCursor(wxCURSOR_CROSS);
}

void CarvrFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void CarvrFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* open_dialog = new wxFileDialog(
            this, "Choose a file", "", "",
            "*.png|*.jpg|*.jpeg|*.bmp");

    if (open_dialog->ShowModal() == wxID_OK) {
        const std::string filename = open_dialog->GetPath().ToStdString();
        cv::Mat image = cv::imread(filename);
        panel->LoadImage(image);
    }

    open_dialog->Destroy();
}

void CarvrFrame::OnMouseLDown(wxMouseEvent& event)
{
    /*
    panel->CaptureMouse();
    mouse_position = event.GetPosition();
    const wxSize panel_size = panel->GetSize();

    if (mouse_position.x >= panel_size.GetWidth() - DRAG_BORDER) {
        drag_mode = HORIZONTAL;
    }
    else if (mouse_position.y >= panel_size.GetHeight() - DRAG_BORDER) {
        drag_mode = VERTICAL;
    } else {
        drag_mode = NONE;
    }
    */
}

void CarvrFrame::OnMouseMove(wxMouseEvent& event)
{
    /*
    if (!event.LeftIsDown())    drag_mode = NONE;
    std::cout << drag_mode << " " << std::flush;

    wxSize size = GetSize();
    const wxPoint mp = event.GetPosition();

    if (drag_mode == HORIZONTAL) {
        int new_width = size.GetWidth() + mp.x - mouse_position.x;
        new_width = std::min(new_width, max_size.GetWidth());
        new_width = std::max(new_width, DRAG_BORDER);
        size.SetWidth(new_width);
    } else if (drag_mode == VERTICAL) {
        int new_height = size.GetHeight() + mp.y - mouse_position.y;
        new_height = std::min(new_height, max_size.GetHeight());
        new_height = std::max(new_height, DRAG_BORDER);
        size.SetHeight(new_height);
    }

    if (drag_mode == NONE) {
        const wxSize panel_size = panel->GetSize();
        if (mp.x >= panel_size.GetWidth() - DRAG_BORDER ||
            mp.y >= panel_size.GetHeight() - DRAG_BORDER)
        {
            panel->SetCursor(wxCURSOR_HAND);
        } else {
            panel->SetCursor(wxCURSOR_CROSS);
        }
    } else {
        SetSize(size);
    }

    mouse_position = mp;
    */
}
