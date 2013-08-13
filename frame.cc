#include <wx/aboutdlg.h>

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
    file_menu->Append(wxID_ABOUT, _("About"));
    file_menu->Append(wxID_SAVE, _("Save\tCTRL-S"));
    menu_bar->Append(file_menu, _("File"));
    SetMenuBar(menu_bar);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnSave, this, wxID_SAVE);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnAbout, this, wxID_ABOUT);
    Fit();
}

void CarvrFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void CarvrFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* open_dialog = new wxFileDialog(
            this, "Choose a file", "", "",
            "*.png|*.jpg|*.jpeg|*.bmp",
            wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

    if (open_dialog->ShowModal() == wxID_OK) {
        const std::string filename = open_dialog->GetPath().ToStdString();
        panel->LoadImage(filename);
    }

    open_dialog->Destroy();
}

void CarvrFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* save_dialog = new wxFileDialog(
            this, "Save image as", "", "", "*",
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (save_dialog->ShowModal() == wxID_OK) {
        const std::string filename = save_dialog->GetPath().ToStdString();
        try {
            panel->SaveImage(filename);
        } catch (cv::Exception& e) {
            wxMessageDialog* d;
            if (strstr(e.what(), "specified extension")) {
                d = new wxMessageDialog(
                        this, "Acceptable values are .jpg, .png, .bmp",
                        "Error: Invalid extension");
            } else {
                d = new wxMessageDialog(
                        this, e.what(), "OpenCV error:");
            }
            d->ShowModal();
            d->Destroy();
        }
    }

    save_dialog->Destroy();
}

void CarvrFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetName(_("carvr"));
    info.SetVersion(_("0.1"));
    info.SetDescription(_("Seam carving made simple"));
    info.SetCopyright(_("(C) Matthew Keeter, 2013"));

    wxAboutBox(info);
}
