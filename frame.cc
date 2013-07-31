#include "frame.h"

CarvrFrame::CarvrFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    wxMenuBar* menu_bar = new wxMenuBar;
    wxMenu* file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT, _("Quit\tCTRL-Q"));
    file_menu->Append(wxID_OPEN, _("Open\tCTRL-O"));
    menu_bar->Append(file_menu, _("File"));
    SetMenuBar(menu_bar);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnOpen, this, wxID_OPEN);
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
        std::cout << open_dialog->GetPath() << std::endl;
    }

    open_dialog->Destroy();
}
