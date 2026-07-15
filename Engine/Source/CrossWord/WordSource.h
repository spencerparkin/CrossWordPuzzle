#pragma once

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
}