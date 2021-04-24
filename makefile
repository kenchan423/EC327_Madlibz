# This Makefile
# gives instructions to the command
# make
# about how and whether to build programs
#
# The syntax is

# thingtobuld:  what-it-needs-or-depends-on
#    instructions on how to build it
#

# make uses the modification dates to determine
# is the programs need to be re-built

#all: showtime dash_sfml bigger

#showtime: showtime.cpp
#	g++ showtime.cpp -o showtime

#dash_sfml: text_dash_sfml.cpp
#	g++ text_dash_sfml.cpp -o dash_sfml -lsfml-graphics -lsfml-window -lsfml-system


#bigger: bigger.cpp
#	g++ bigger.cpp -o bigger -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
#	echo "hi"
#	ls

all: test

test: project_test_1.cpp
	g++ project_test_1.cpp -o proj -g -lsfml-graphics -lsfml-window -lsfml-system -lcurl