#include "CrossWord/Puzzle.h"
#include "CrossWord/Random.h"
#include "CrossWord/WordIndex.h"

using namespace CrossWordEngine;

Puzzle::Puzzle()
{
	this->matrix = nullptr;
	this->numRows = 0;
	this->numCols = 0;
}

/*virtual*/ Puzzle::~Puzzle()
{
	this->SetSize(0, 0);
}

void Puzzle::SetSize(int numRows, int numCols)
{
	if (this->matrix)
	{
		for (int row = 0; row < this->numRows; row++)
			delete[] this->matrix[row];

		delete[] this->matrix;
		this->matrix = nullptr;
	}

	this->numRows = numRows;
	this->numCols = numCols;

	if (this->numRows > 0)
	{
		this->matrix = new Element*[this->numRows];

		for (int row = 0; row < this->numRows; row++)
		{
			this->matrix[row] = new Element[this->numCols];

			for (int col = 0; col < this->numCols; col++)
			{
				this->matrix[row][col].refCount = 0;
			}
		}
	}
}

void Puzzle::Clear()
{
	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			Element* element = &this->matrix[row][col];
			element->letter.reset();
			element->refCount = 0;
		}
	}
}

bool Puzzle::Generate(WordIndex* wordIndex, Random* random)
{
	this->Clear();

	// STPTODO: The idea is to work by randomly stamping words and analyzing the matrix
	//          between each stamp.  Each iteration, we try to stamp in order to solve
	//          any invalid words found in the matrix.  Some stopping condition is needed.
	//          If we reach an impass, then we can unstamp any word we've already stamped.
	//          We don't need to backtrack by reversing every stamp we've done in reverse
	//          order.  Rather, we can unstamp any word in our stamp history to try to get
	//          us beyond the impass.  This algorithm doesn't feel like it's guarenteed to
	//          work, but that's why I think this is an interesting problem -- I'm not sure
	//          how to solve it.

	return true;
}

void Puzzle::AnalyzeMatrix(WordIndex* wordIndex, std::vector<WordStamp>& invalidWordsArray, std::vector<WordStamp>& validWordsArray)
{
	invalidWordsArray.clear();
	validWordsArray.clear();

	for (int row = 0; row < this->numRows; row++)
	{
		for (int col = 0; col < this->numCols; col++)
		{
			std::vector<WordSpan> wordSpanArray;

			Location location{ row, col };
			if (!this->IsStartOfWordLocation(location, wordSpanArray))
				continue;

			for (const WordSpan& wordSpan : wordSpanArray)
			{
				WordStamp wordStamp;
				wordStamp.span = wordSpan;
				this->UnstampWord(wordStamp, false);

				if (wordIndex->IsValidWord(wordStamp.text))
					validWordsArray.push_back(wordStamp);
				else
					invalidWordsArray.push_back(wordStamp);
			}
		}
	}
}

bool Puzzle::IsLocationValid(const Location& location) const
{
	if (location.row < 0 || location.row >= this->numRows)
		return false;

	if (location.col < 0 || location.col >= this->numCols)
		return false;

	return true;
}

bool Puzzle::HasCharacterAtLocation(const Location& location) const
{
	if (!this->IsLocationValid(location))
		return false;

	return this->matrix[location.row][location.col].letter.has_value();
}

bool Puzzle::IsStartOfWordLocation(const Location& location, std::vector<WordSpan>& wordSpanArray) const
{
	wordSpanArray.clear();

	if (!this->HasCharacterAtLocation(location))
		return false;

	if (location.row == 0 || !this->matrix[location.row - 1][location.col].letter.has_value())
	{
		WordSpan wordSpan;
		wordSpan.location = location;
		wordSpan.orientation = Orientation::DOWN;
		wordSpan.length = 0;
		for (int i = location.row; i < this->numRows; i++)
			if (this->matrix[location.row + i][location.col].letter.has_value())
				wordSpan.length++;

		wordSpanArray.push_back(wordSpan);
	}

	if (location.col == 0 || !this->matrix[location.row][location.col - 1].letter.has_value())
	{
		WordSpan wordSpan;
		wordSpan.location = location;
		wordSpan.orientation = Orientation::ACROSS;
		wordSpan.length = 0;
		for (int i = location.col; i < this->numCols; i++)
			if (this->matrix[location.row][location.col + i].letter.has_value())
				wordSpan.length++;
	}

	return wordSpanArray.size() > 0;
}

bool Puzzle::StampWord(const WordStamp& wordStamp)
{
	int i = 0;

	return this->ForElementsOf(wordStamp.span, [&wordStamp, &i](Element* element) -> bool
		{
			if (i >= wordStamp.text.length())
				return false;

			char letter = wordStamp.text.c_str()[i++];

			if (!element->letter.has_value())
			{
				if (element->refCount != 0)
					return false;

				element->letter = letter;
			}
			else if (element->letter.value() != letter)
				return false;

			element->refCount++;
			return true;
		});
}

bool Puzzle::UnstampWord(WordStamp& wordStamp, bool extract)
{
	wordStamp.text = "";

	return this->ForElementsOf(wordStamp.span, [&wordStamp, extract](Element* element) -> bool
		{
			if (!element->letter.has_value())
				return false;

			wordStamp.text += element->letter.value();
			
			if (extract)
			{
				if (element->refCount <= 0)
					return false;

				if(--element->refCount == 0)
					element->letter.reset();
			}

			return true;
		});
}

bool Puzzle::ForElementsOf(const WordSpan& wordSpan, std::function<bool(Element*)> callback)
{
	for (int i = 0; i < wordSpan.length; i++)
	{
		Location location = wordSpan.location;

		switch (wordSpan.orientation)
		{
		case Orientation::DOWN:
			location.row += i;
			break;
		case Orientation::ACROSS:
			location.col += i;
			break;
		}

		if (!this->IsLocationValid(location))
			return false;

		Element* element = &this->matrix[location.row][location.col];

		if (!callback(element))
			return false;
	}

	return true;
}