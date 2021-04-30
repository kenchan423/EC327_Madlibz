# EC327_Madlibz
EC327 final project. A Madlibz app using SFML


Compile using the make file. Just run "make" in the terminal. The compiled file is called "proj" so just run that after compiling. 
Make sure to have the libcurl and nlohmann library (as well as the json.hpp file where the project file is)

The program will run and first ask for the maximum and minimum amount of blanks (size of the mad libs) you want. After pressing submit, it will call for a mad libs from this api (https://madlibz.herokuapp.com/api). It will then ask for you to fill in the blanks. Lastly, the app will print out the final mad libs. When you're done and closed the last screen, the app will cycle back to the first screen (asking for maximums and minimums).
