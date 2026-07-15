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
		Word();
		virtual ~Word();

	public:
		std::string word;
		std::string definition;
		std::string puzzleHint;
	};
}