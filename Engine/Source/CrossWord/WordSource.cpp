#include "CrossWord/WordSource.h"
#include "CrossWord/Word.h"
#include <fstream>

using namespace CrossWordEngine;

//------------------------------- WordSource -------------------------------

WordSource::WordSource()
{
}

/*virtual*/ WordSource::~WordSource()
{
}

//------------------------------- WordFileSource -------------------------------

WordFileSource::WordFileSource()
{
}

/*virtual*/ WordFileSource::~WordFileSource()
{
}

bool WordFileSource::Load(const std::string& wordFilePath)
{
	std::ifstream fileStream;
	fileStream.open(wordFilePath, std::ios::in);
	if (!fileStream.is_open())
		return false;

	this->wordArray.clear();

	std::string wordText;
	while (std::getline(fileStream, wordText))
	{
		std::shared_ptr<Word> word = std::make_shared<Word>(wordText);
		this->wordArray.push_back(word);
	}

	return true;
}

/*virtual*/ unsigned int WordFileSource::GetNumWords()
{
	return (unsigned int)this->wordArray.size();
}

/*virtual*/ std::shared_ptr<Word> WordFileSource::GetWord(unsigned int i)
{
	if (i >= (unsigned int)this->wordArray.size())
		return nullptr;

	return this->wordArray[i];
}