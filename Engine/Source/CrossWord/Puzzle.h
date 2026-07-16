#pragma once

namespace CrossWordEngine
{
	class WordIndex;

	/**
	 * 
	 */
	class Puzzle
	{
	public:
		Puzzle();
		virtual ~Puzzle();

		bool Generate(int numRows, int numCols, WordIndex* wordIndex);

		// STPTODO: Own matrix?
	};
}