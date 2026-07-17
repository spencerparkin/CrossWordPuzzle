#pragma once

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <functional>
#include <string>
#include <format>

namespace CrossWordEngine
{
	class Word;
	class WordSource;
	class Random;

	struct LetterPosition
	{
		char letter;
		int position;

		bool operator==(const LetterPosition& letterPosition) const;
	};

	struct WordBucketKey
	{
	public:
		WordBucketKey();

		void SortIfNeeded() const;
		bool operator==(const WordBucketKey& key) const;
		const std::vector<LetterPosition>& GetLetterPositionArray() const;
		void AddLetterPosition(const LetterPosition& letterPosition);

	private:
		mutable std::vector<LetterPosition> letterPositionArray;
		mutable bool isSorted;
	};
}

template<>
struct std::hash<CrossWordEngine::WordBucketKey>
{
	size_t operator()(const CrossWordEngine::WordBucketKey& key) const;
};

namespace CrossWordEngine
{
	/**
	 * These are words that share the same word bucket key.
	 */
	class WordBucket
	{
	public:
		void Intersect(const WordBucket& wordBucketA, const WordBucket& wordBucketB);
		bool ContainsWord(const std::shared_ptr<Word>& word) const;
		std::shared_ptr<Word> GetRandomWord(Random* random);

		std::unordered_set<std::shared_ptr<Word>> wordSet;
	};

	/**
	 * These are words that share the same length.
	 */
	class WordGroup
	{
	public:
		WordGroup();

		std::shared_ptr<Word> GetNextRandomWord(Random* random);

		bool isShuffled;
		int nextWordOffset;
		std::vector<std::shared_ptr<Word>> wordArray;
	};

	/**
	 * These are built from a given word source and are used to accelerate
	 * the look-up of words needed with certain characteristics.  In particular,
	 * an index is a collection of work buckets.  Each bucket is a collection
	 * words sharing the same characteristic.  The main characteristic we're
	 * interested in is a word having a specific letter at a specific position.
	 * Buckets should be such that intersecting them is easy and efficient.
	 */
	class WordIndex
	{
	public:
		WordIndex();
		virtual ~WordIndex();

		void Clear();

		unsigned int GetNumBuckets();

		/**
		 * Encorporate all words from the given source into this index.
		 * Note that this index is not cleared before we begin, so this
		 * can be used to enlarge the index.
		 * 
		 * @param[in] wordSource Words are pulled from this source.
		 * @param[in,out] progressLambda This gives the caller a way to give feedback to the user on our progress since this procedure takes a bit of time.
		 * @return True is returned on success.
		 */
		bool Generate(WordSource* wordSource, std::function<bool(int, int)> progressLambda = [](int, int) -> bool { return false; });

		/**
		 * Find all words having the certain given letters at the given positions.
		 * 
		 * @param[in] key This specifies what letters need to be in what positions.
		 * @param[out] wordBucket This found (or created) bucket is returned here.
		 * @return True is returned if a bucket of non-zero size was found.
		 */
		bool FindOrCreateBucket(const WordBucketKey& key, std::shared_ptr<const WordBucket>& wordBucket);

		/**
		 * Find and return a word of the given length at random.
		 * 
		 * @param[in] wordLength This is the length of the desired word.
		 * @param[out] word The randomly found word is returned here.
		 * @return True is returned if a word was found.
		 */
		bool GetRandomWordOfLength(unsigned int wordLength, std::shared_ptr<Word>& word, Random* random);

		/**
		 * Tell us if the given word is an actual word in our language.
		 */
		bool IsValidWord(const std::string& word) const;

	private:

		bool IntegrateWord(std::shared_ptr<Word> word);

		std::unordered_map<WordBucketKey, std::shared_ptr<WordBucket>> wordBucketMap;
		std::unordered_map<std::string, std::shared_ptr<Word>> wordMap;
		std::unordered_map<unsigned int, std::shared_ptr<WordGroup>> wordGroupMap;
	};
}