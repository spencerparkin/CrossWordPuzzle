#pragma once

#include <wx/app.h>
#include "CrossWord/Puzzle.h"
#include "CrossWord/WordIndex.h"
#include "CrossWord/WordSource.h"

class Frame;

class App : public wxApp
{
public:
	App();
	virtual ~App();

	virtual bool OnInit() override;
	virtual int OnExit() override;

	CrossWordEngine::Puzzle puzzle;
	CrossWordEngine::WordIndex wordIndex;
	CrossWordEngine::WordFileSource wordSource;

private:
	Frame* frame;
};

wxDECLARE_APP(App);