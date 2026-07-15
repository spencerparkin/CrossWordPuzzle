#pragma once

namespace CrossWordEngine
{
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

		// STPTODO: class Bucket
	};
}