#include "CrossWord/WordIndex.h"
#include "CrossWord/WordSource.h"
#include "CrossWord/Word.h"
#include <algorithm>

using namespace CrossWordEngine;

//------------------------------- LetterPosition -------------------------------

bool LetterPosition::operator==(const LetterPosition& letterPosition) const
{
	return this->letter == letterPosition.letter && this->position == letterPosition.position;
}

//------------------------------- WordBucketKey -------------------------------

WordBucketKey::WordBucketKey()
{
	this->isSorted = true;
}

void WordBucketKey::SortIfNeeded() const
{
	if (!this->isSorted)
	{
		std::sort(this->letterPositionArray.begin(), this->letterPositionArray.end(), [](const LetterPosition& letterPositionA, const LetterPosition& letterPositionB)
			{
				return letterPositionA.position < letterPositionB.position;
			});

		this->isSorted = true;
	}
}

bool WordBucketKey::operator==(const WordBucketKey& key) const
{
	if (this->letterPositionArray.size() != key.letterPositionArray.size())
		return false;

	this->SortIfNeeded();
	key.SortIfNeeded();

	for (int i = 0; i < (int)this->letterPositionArray.size(); i++)
		if (this->letterPositionArray[i] != key.letterPositionArray[i])
			return false;

	return true;
}

const std::vector<LetterPosition>& WordBucketKey::GetLetterPositionArray() const
{
	return this->letterPositionArray;
}

void WordBucketKey::AddLetterPosition(const LetterPosition& letterPosition)
{
	this->letterPositionArray.push_back(letterPosition);
	this->isSorted = false;
}

//------------------------------- std::hash<WordBucketKey> -------------------------------

size_t std::hash<CrossWordEngine::WordBucketKey>::operator()(const CrossWordEngine::WordBucketKey& key) const
{
	std::string str;

	key.SortIfNeeded();

	for (const CrossWordEngine::LetterPosition& letterPosition : key.GetLetterPositionArray())
		str += std::format("{}{}", letterPosition.letter, letterPosition.position);

	return std::hash<std::string>{}(str);
}

//------------------------------- WordBucket -------------------------------

void WordBucket::Intersect(const WordBucket& wordBucketA, const WordBucket& wordBucketB)
{
	this->wordSet.clear();
	
	if (wordBucketA.wordSet.size() < wordBucketB.wordSet.size())
	{
		for (const std::shared_ptr<Word>& word : wordBucketA.wordSet)
			if (wordBucketB.ContainsWord(word))
				this->wordSet.insert(word);
	}
	else
	{
		for (const std::shared_ptr<Word>& word : wordBucketB.wordSet)
			if (wordBucketA.ContainsWord(word))
				this->wordSet.insert(word);
	}
}

bool WordBucket::ContainsWord(const std::shared_ptr<Word>& word) const
{
	return this->wordSet.find(word) != this->wordSet.end();
}

//------------------------------- WordIndex -------------------------------

WordIndex::WordIndex()
{
}

/*virtual*/ WordIndex::~WordIndex()
{
}

void WordIndex::Clear()
{
	this->wordBucketMap.clear();
	this->wordSet.clear();
}

unsigned int WordIndex::GetNumBuckets()
{
	return (unsigned int)this->wordBucketMap.size();
}

bool WordIndex::Generate(WordSource* wordSource, std::function<bool(int, int)> progressLambda /*= [](int, int) -> bool { return false; }*/)
{
	unsigned int numWords = wordSource->GetNumWords();

	for (unsigned int i = 0; i < numWords; i++)
	{
		if (progressLambda(i, numWords))
			return false;

		std::shared_ptr<Word> word = wordSource->GetWord(i);

		if (this->wordSet.find(word->word) != this->wordSet.end())
			continue;

		if (!this->IntegrateWord(word))
			return false;

		this->wordSet.insert(word->word);
	}

	if (progressLambda(numWords, numWords))
		return false;

	return true;
}

bool WordIndex::IntegrateWord(std::shared_ptr<Word> word)
{
	for (int position = 0; word->word.c_str()[position] != '\0'; position++)
	{
		char letter = word->word.c_str()[position];

		WordBucketKey key;
		key.AddLetterPosition({ letter, position });

		std::shared_ptr<WordBucket> wordBucket;

		auto pair = this->wordBucketMap.find(key);
		if (pair != this->wordBucketMap.end())
			wordBucket = pair->second;
		else
		{
			wordBucket = std::make_shared<WordBucket>();
			this->wordBucketMap.insert(std::pair(key, wordBucket));
		}

		wordBucket->wordSet.insert(word);
	}

	return true;
}

bool WordIndex::FindOrCreateBucket(const WordBucketKey& key, std::shared_ptr<const WordBucket>& wordBucket)
{
	auto pair = this->wordBucketMap.find(key);
	if (pair != this->wordBucketMap.end())
	{
		wordBucket = pair->second;
		return true;
	}

	if (key.GetLetterPositionArray().size() < 2)
		return false;

	std::vector<std::shared_ptr<WordBucket>> wordBucketArray;

	for (const LetterPosition& letterPosition : key.GetLetterPositionArray())
	{
		WordBucketKey subKey;
		subKey.AddLetterPosition(letterPosition);

		pair = this->wordBucketMap.find(subKey);
		if (pair == this->wordBucketMap.end())
			return false;

		wordBucketArray.push_back(pair->second);
	}

	auto newWordBucket = std::make_shared<WordBucket>();
	newWordBucket->Intersect(*wordBucketArray[0], *wordBucketArray[1]);

	for (int i = 2; i < (int)wordBucketArray.size(); i++)
	{
		WordBucket tempWordBucket;
		tempWordBucket.Intersect(*newWordBucket, *wordBucketArray[i]);
		newWordBucket->wordSet = std::move(tempWordBucket.wordSet);
	}

	if (newWordBucket->wordSet.size() == 0)
		return false;

	this->wordBucketMap.insert(std::pair(key, newWordBucket));
	wordBucket = newWordBucket;
	return true;
}