CSE 681 - Software Modeling
Project Phase 1 - OOP Project for JSON Parser


The JSONParser Project is a standalone command-line program written in Visual Studio using the C++ Language.
The command-line program prompts the user for input to a web URL containing JSON formatted data and allows the user to select from three options. 

The data structure used to hold all the JSON Object data is a struct named matchUpStats. All individual MatchUpStats are combined into a vector of MatchUpStats.

-------------------------------------------------------------------------------
TO RUN THE PROGRAM:
	- Option 1: Compile the *.cpp and *.h files using the command line and your choice of compiler.

	- Option 2: In Visual Studio 2019 or Higher, load the *.sln file from the repository. 
			IMPORTANT: Make sure to point the linker to the cURL dynamic link libraries that are stored in the \src\x64\Debug\ directory. The code can then be viewed and debugged using Visual Studio.

	- Option 3: Navigate to the \exe\ directory from the command line and run the JSONParser.exe executable.


-------------------------------------------------------------------------------
GITHUB AUTHORS:
@jasminekp
@risktakingkev