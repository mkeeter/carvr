#include "frame.h"
#include "panel.h"

CarvrFrame::CarvrFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
      panel(new ImagePanel(this)), drag_mode(NONE)
{
    wxMenuBar* const menu_bar = new wxMenuBar;
    wxMenu* const file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("Quit\tCTRL-Q"));
    file_menu->Append(wxID_OPEN, _("Open\tCTRL-O"));
    menu_bar->Append(file_menu, _("File"));
    SetMenuBar(menu_bar);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnOpen, this, wxID_OPEN);
    panel->Bind(wxEVT_LEFT_DOWN, &CarvrFrame::OnMouseLDown, this);
    panel->Bind(wxEVT_LEFT_UP,   &CarvrFrame::OnMouseLUp,   this);
    panel->Bind(wxEVT_MOTION,    &CarvrFrame::OnMouseMove,  this);

    max_size = GetSize();
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

        if (image.cols > image.rows && image.cols > 640) {
            SetSize(wxDefaultCoord, wxDefaultCoord,
                    640, 640*image.rows/image.cols);
            scale = image.cols / 640.0f;
        } else if (image.rows > 640) {
            SetSize(wxDefaultCoord, wxDefaultCoord,
                    640*image.cols/image.rows, 640);
            scale = image.rows / 640.0f;
        } else {
            SetSize(wxDefaultCoord, wxDefaultCoord, image.cols, image.rows);
            scale = 1;
        }
        max_size = GetSize();
    }

    open_dialog->Destroy();
}

void CarvrFrame::OnMouseLDown(wxMouseEvent& event)
{
    mouse_position = event.GetPosition();
    const wxSize size = panel->GetSize();

    if (mouse_position.x >= size.GetWidth() - 20) {
        drag_mode = HORIZONTAL;
    }
    else if (mouse_position.y >= size.GetHeight() - 20) {
        drag_mode = VERTICAL;
    }
}

void CarvrFrame::OnMouseLUp(wxMouseEvent& WXUNUSED(event))
{
    drag_mode = NONE;
}

void CarvrFrame::OnMouseMove(wxMouseEvent& event)
{
    wxSize size = GetSize();
    const wxPoint new_mouse_position = event.GetPosition();

    if (drag_mode == HORIZONTAL) {
        size.SetWidth(
                std::min(max_size.GetWidth(),
                    size.GetWidth() +
                    new_mouse_position.x - mouse_position.x));
    } else if (drag_mode == VERTICAL) {
        size.SetHeight(
                std::min(max_size.GetHeight(),
                    size.GetHeight() +
                    new_mouse_position.y - mouse_position.y));
    }
    if (drag_mode != NONE)  SetSize(size);

    mouse_position = new_mouse_position;
}
