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
    panel->Bind(wxEVT_LEFT_DOWN, &CarvrFrame::OnMouseLDown, this);
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
    }

    open_dialog->Destroy();
}

void CarvrFrame::OnMouseLDown(wxMouseEvent& WXUNUSED(event))
{
    std::cout << "left button clicked" << std::endl;
}
