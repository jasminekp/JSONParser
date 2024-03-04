CSE 681 - Software Modeling
Project Phase 2 - OOP Project for JSON Parser


Project Phase 1:
	The JSONParser Project is a standalone command-line program written in Visual Studio using the C++ Language.
	The command-line program prompts the user for input to a web URL containing JSON formatted data and allows the user to select from three options. 

	The data structure used to hold all the JSON Object data is a struct named matchUpStats. All individual MatchUpStats are combined into a vector of MatchUpStats.

Project Phase 2:
	The JSONParser Project has been modified to calculate each team's season records (wins, losses, and ties) and print the scores to the console.



-------------------------------------------------------------------------------
TO RUN THE PROGRAM:
	- Option 1: Compile the *.cpp and *.h files using the command line and your choice of compiler.

	- Option 2: In Visual Studio 2019 or Higher, load the *.sln file from the repository. Make sure to point the linker to the cURL dynamic link libraries that are stored in the
			\JSONParser\src\x64\Debug directory. The code can then be viewed and debugged using Visual Studio.

	- Option 3: Navigate to the \JSONParser\x64\Debug\ directory from the command line and run the JSONParser.exe executable.


-------------------------------------------------------------------------------
GITHUB AUTHORS:
@jasminekp
@risktakingkev