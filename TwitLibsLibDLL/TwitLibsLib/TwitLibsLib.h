//TwitLibsLib.h
//TwitLibs allows for users to post tweets with tags for some random words
#include <string>
#include <vector>

namespace TwitLibsLib
{
	class TwitLibs
	{
	public:
		//The entry point for doing a twitLibs
		static __declspec(dllexport) void doTwitLibs();

		//changes the string tags to random words
		static __declspec(dllexport) void replaceWords(std::string &theString);

		//loads the words from files into the vectors of word choices
		static __declspec(dllexport) void loadWords();

	private:
		static std::vector<std::string> vNouns;
		static std::vector<std::string> vVerbs;
		static std::vector<std::string> vAdjectives;
	};

	std::vector<std::string> TwitLibs::vNouns;
	std::vector<std::string> TwitLibs::vVerbs;
	std::vector<std::string> TwitLibs::vAdjectives;
} 