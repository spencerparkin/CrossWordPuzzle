# CMakeLists.txt for cross-word puzzle engine C++ library.

set(CROSSWORD_ENGINE_SOURCES
	Source/CrossWord/Puzzle.cpp
	Source/CrossWord/Puzzle.h
	Source/CrossWord/WordIndex.cpp
	Source/CrossWord/WordIndex.h
	Source/CrossWord/WordSource.cpp
	Source/CrossWord/WordSource.h
	Source/CrossWord/Word.cpp
	Source/CrossWord/Word.h
)

add_library(CrossWordEngine STATIC ${CROSSWORD_ENGINE_SOURCES})

target_include_directories(CrossWordEngine PUBLIC Source)