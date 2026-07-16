#pragma once

#include <wx/frame.h>

enum
{
	ID_Exit = wxID_HIGHEST + 1,
	ID_GeneratePuzzle
};

class Frame : public wxFrame
{
public:
	Frame();
	virtual ~Frame();

private:
	void OnGeneratePuzzle(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
};