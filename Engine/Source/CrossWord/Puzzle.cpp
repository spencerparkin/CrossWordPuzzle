#include "Puzzle.h"

using namespace CrossWordEngine;

Puzzle::Puzzle()
{
}

/*virtual*/ Puzzle::~Puzzle()
{
}

bool Puzzle::Generate(int numRows, int numCols, WordIndex* wordIndex)
{
	return true;
}