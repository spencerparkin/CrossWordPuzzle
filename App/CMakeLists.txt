# CMakeLists.txt for cross-word puzzle app.

set(CROSSWORD_PUZZLE_APP_SOURCE
	Source/App.cpp
	Source/App.h
	Source/Frame.cpp
	Source/Frame.h
)

add_executable(CrossWordPuzzleApp WIN32
	${CROSSWORD_PUZZLE_APP_SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/wx_dpi_aware_pmv2.manifest
)

set(wxBUILD_SHARED OFF)

include(FetchContent)

FetchContent_Declare(
	wxWidgets
	GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
	GIT_TAG 3.3-release
)

FetchContent_MakeAvailable(wxWidgets)

target_compile_definitions(CrossWordPuzzleApp PUBLIC
	__WXMSW__
	_UNICODE
)

target_compile_definitions(CrossWordPuzzleApp PRIVATE
	WIN32_LEAN_AND_MEAN
	NOMINMAX
	_USE_MATH_DEFINES
)

target_link_libraries(CrossWordPuzzleApp
	CrossWordEngine
	wx::core
	wx::base
	wx::gl
)