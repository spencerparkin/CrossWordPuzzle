#include "Frame.h"
#include "App.h"
#include "CrossWord/Random.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/busycursor.h>
#include <wx/progdlg.h>

Frame::Frame() : wxFrame(nullptr, wxID_ANY, "Crossword Puzzle", wxDefaultPosition, wxSize(1500, 1200))
{
	wxMenu* puzzleMenu = new wxMenu();

	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_GeneratePuzzle, "Generate Puzzle", "Randomly generate a new cross-word puzzle."));
	puzzleMenu->AppendSeparator();
	puzzleMenu->Append(new wxMenuItem(puzzleMenu, ID_Exit, "Exit", "Go do something else with your life."));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(puzzleMenu, "Puzzle");
	this->SetMenuBar(menuBar);

	this->SetStatusBar(new wxStatusBar(this));

	this->Bind(wxEVT_MENU, &Frame::OnExit, this, ID_Exit);
	this->Bind(wxEVT_MENU, &Frame::OnGeneratePuzzle, this, ID_GeneratePuzzle);
}

/*virtual*/ Frame::~Frame()
{
}

void Frame::OnGeneratePuzzle(wxCommandEvent& event)
{
	wxBusyCursor busyCursor;

	CrossWordEngine::WordFileSource* wordSource = &wxGetApp().wordSource;

	if (wordSource->GetNumWords() == 0)
	{
		if (!wordSource->Load("D:\\git_repos\\CrossWordPuzzle\\Words.txt"))
		{
			wxMessageBox("Failed to load word source!", "Error!", wxICON_ERROR | wxOK, this);
			return;
		}
	}

	CrossWordEngine::WordIndex* wordIndex = &wxGetApp().wordIndex;

	if (wordIndex->GetNumBuckets() == 0)
	{
		wxGenericProgressDialog progressDialog("Working...", "Generating word index...", 1000, this, wxPD_AUTO_HIDE | wxPD_CAN_ABORT | wxPD_APP_MODAL);
		progressDialog.Show();

		auto progressLambda = [&progressDialog](int i, int total) -> bool
			{
				if (i % 500 != 0)
					return false;
			
				int range = progressDialog.GetRange();
				int value = int((double(i) / double(total)) * double(range));
				return !progressDialog.Update(value);
			};

		if (!wordIndex->Generate(wordSource, progressLambda))
		{
			wxMessageBox("Failed to generate word index!", "Error!", wxICON_ERROR | wxOK, this);
			return;
		}
	}

	CrossWordEngine::Puzzle* puzzle = &wxGetApp().puzzle;

	CrossWordEngine::Random random;
	random.SetSeed(12345);

	// STPTODO: Maybe make dialog querying user for dimensions of puzzle?
	int numRows = 10;
	int numCols = 10;
	if (!puzzle->Generate(numRows, numCols, wordIndex, &random))
	{
		wxMessageBox("Failed to generate puzzle!", "Error!", wxICON_ERROR | wxOK, this);
		return;
	}
}

void Frame::OnExit(wxCommandEvent& event)
{
	this->Close(true);
}