#pragma once

namespace CrossWordEngine
{
	class WordIndex;
	class Random;

	/**
	 * 
	 */
	class Puzzle
	{
	public:
		Puzzle();
		virtual ~Puzzle();

		bool Generate(int numRows, int numCols, WordIndex* wordIndex, Random* random);

		// STPTODO: Own matrix?
	};
}