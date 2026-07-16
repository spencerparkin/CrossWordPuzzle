#pragma once

#include <string>

namespace CrossWordEngine
{
	/**
	 *
	 */
	class Word
	{
	public:
		Word(const std::string& word);
		virtual ~Word();

	public:
		std::string word;
		std::string puzzleHint;
	};
}