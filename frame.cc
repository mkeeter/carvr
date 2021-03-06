#include <wx/aboutdlg.h>

#include "app.h"
#include "frame.h"
#include "image.h"
#include "panel.h"

////////////////////////////////////////////////////////////////////////////////

CarvrFrame::CarvrFrame(std::string filename)
    : wxFrame(NULL, wxID_ANY, "carvr", wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
      panel(new ImagePanel(this))
{
    wxMenuBar* const menu_bar = new wxMenuBar;
    wxMenu* const file_menu = new wxMenu;
    file_menu->Append(wxID_OPEN, _("Open\tCTRL-O"));
    file_menu->Append(wxID_SAVE, _("Save\tCTRL-S"));
    file_menu->Append(wxID_CLOSE, _("Close\tCTRL-W"));
    file_menu->Append(wxID_EXIT, _("Quit\tCTRL-Q"));
    file_menu->Append(wxID_ABOUT, _("About"));
    menu_bar->Append(file_menu, _("File"));

    wxMenu* const edit_menu = new wxMenu;
    edit_menu->Append(wxID_UNDO, _("Undo\tCTRL-Z"))->Enable(false);
    menu_bar->Append(edit_menu, _("Edit"));

    SetMenuBar(menu_bar);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrApp::OnOpen,
         static_cast<CarvrApp*>(wxTheApp), wxID_OPEN);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnSave, this, wxID_SAVE);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnClose, this, wxID_CLOSE);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrApp::OnQuit,
         static_cast<CarvrApp*>(wxTheApp), wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnAbout, this, wxID_ABOUT);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CarvrFrame::OnUndo, this, wxID_UNDO);
    Fit();

    if (!filename.empty()) {
        LoadImage(filename);
    }
    Show();
}

////////////////////////////////////////////////////////////////////////////////

// Upon destruction, remove this frame from the master list
CarvrFrame::~CarvrFrame()
{
    static_cast<CarvrApp*>(wxTheApp)->RemoveFrame(this);
}

////////////////////////////////////////////////////////////////////////////////

void CarvrFrame::LoadImage(std::string filename)
{
    panel->LoadImage(filename);
}

////////////////////////////////////////////////////////////////////////////////

const Image* CarvrFrame::GetImage() const
{
    return panel->GetImage();
}

////////////////////////////////////////////////////////////////////////////////

void CarvrFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

////////////////////////////////////////////////////////////////////////////////

void CarvrFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* save_dialog = new wxFileDialog(
            this, "Save image as", "", "",
            _("\
PNG image (*.png)|*.png|\
JPG image (*.jpg)|*.jpg|\
Bitmap image (*.bmp)|*.bmp"),
            wxFD_SAVE);

    if (save_dialog->ShowModal() == wxID_OK) {
        panel->SaveImage(save_dialog->GetPath().ToStdString());
    }

    save_dialog->Destroy();
}

////////////////////////////////////////////////////////////////////////////////

void CarvrFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetName(_("carvr"));
    info.SetVersion(_("0.1"));
    info.SetDescription(_("Seam carving made simple"));
    info.SetCopyright(_("(C) Matthew Keeter, 2013"));

    wxAboutBox(info);
}

////////////////////////////////////////////////////////////////////////////////

void CarvrFrame::OnUndo(wxCommandEvent& WXUNUSED(event))
{
    panel->Undo();
}
