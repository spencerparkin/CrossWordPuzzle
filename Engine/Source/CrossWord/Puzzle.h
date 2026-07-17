#pragma once

#include <optional>
#include <vector>
#include <string>
#include <functional>

namespace CrossWordEngine
{
	class WordIndex;
	class Random;

	struct Location
	{
		int row;
		int col;
	};

	enum Orientation
	{
		DOWN,
		ACROSS
	};

	struct WordSpan
	{
		Location location;
		Orientation orientation;
		int length;
	};

	struct WordStamp
	{
		WordSpan span;
		std::string text;
	};

	/**
	 * This class represents and is capable of generating a cross-word puzzle.
	 */
	class Puzzle
	{
	public:
		Puzzle();
		virtual ~Puzzle();

		void SetSize(int numRows, int numCols);
		void Clear();
		bool Generate(WordIndex* wordIndex, Random* random);

	private:

		struct Element
		{
			std::optional<char> letter;
			int refCount;
		};

		void AnalyzeMatrix(WordIndex* wordIndex, std::vector<WordStamp>& invalidWordsArray, std::vector<WordStamp>& validWordsArray);
		bool IsLocationValid(const Location& location) const;
		bool HasCharacterAtLocation(const Location& location) const;
		bool IsStartOfWordLocation(const Location& location, std::vector<WordSpan>& wordSpanArray) const;
		bool StampWord(const WordStamp& wordStamp);
		bool UnstampWord(WordStamp& wordStamp, bool extract);
		bool ForElementsOf(const WordSpan& wordSpan, std::function<bool(Element*)> callback);

		Element** matrix;
		int numRows;
		int numCols;
	};
}