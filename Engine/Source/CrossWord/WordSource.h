#pragma once

#include <string>
#include <vector>
#include <memory>

namespace CrossWordEngine
{
	class Word;

	/**
	 * This class abstracts away whatever source of words and definitions
	 * we'd like to feed to the engine.  Conceptually, this is just one
	 * big long list of word/definition pairs from which we can draw.
	 */
	class WordSource
	{
	public:
		WordSource();
		virtual ~WordSource();

		virtual unsigned int GetNumWords() = 0;
		virtual std::shared_ptr<Word> GetWord(unsigned int i) = 0;
	};

	/**
	 * This is the simplest word source being just a simple plain text
	 * file containing a list of words, one per line.  Note that definitions
	 * or any other meta-data about the words isn't needed.  Those can
	 * often be gathered later once the puzzle is finished.  Only the words
	 * themselves are needed when making a puzzle.
	 */
	class WordFileSource : public WordSource
	{
	public:
		WordFileSource();
		virtual ~WordFileSource();

		bool Load(const std::string& wordFilePath);

		virtual unsigned int GetNumWords() override;
		virtual std::shared_ptr<Word> GetWord(unsigned int i) override;

	private:
		std::vector<std::shared_ptr<Word>> wordArray;
	};
}