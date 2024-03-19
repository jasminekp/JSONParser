CSE 681 - Software Modeling


Project Phase 3:
	The JSONParser Project has been modified to incorporate a GUI to load team's season records.

-------------------------------------------------------------------------------
TO COMPILE/RUN THE PROGRAM:
	Option 1) In VS Code or your choice of compiler, run the jsonGUI.py file. Make sure to have Python and Tkinter pre-installed.


	Option 2) a.	From the command line, enter the folder location for where the following files are all stored:
				- jsonGUI.py, json.DLL, libcurl.lib, zlib.lib

	  	  b.	Now run "py jsonGUI.py" in the terminal to load the GUI. Again, make sure to have a version of Python installed and make sure the DLL/so files are in the same folder as this jsonGUI.py.



-------------------------------------------------------------------------------
TO COMPILE/RUN THE DLL:
	- Step 1: From the terminal, Compile the *.cpp and *.h files using the command line and your choice of compiler. Example:
				g++ -c Main.cpp JSONParser.cpp FileHandler.cpp

	- Step 2: Compile the object files into a .DLL (for Windows) or .so (Linux) file. In the options, make sure to include the path to the libcurl.lib static file as shown below:
				g++ -static -shared -o json.dll Main.o JSONParser.o FileHandler.o /path/to/libcurl.lib



-------------------------------------------------------------------------------
GITHUB AUTHORS:
@jasminekp
@risktakingkev