# To run, OpenGL is needed.
# To install on linux, just need to run the following commands:
#
# $ sudo apt-get update
# $ sudo apt-get install libgl1-mesa-dev
# $ sudo apt-get install freeglut3-dev
# $ sudo apt-get install mesa-utils
# $ sudo apt-get install libxmu-dev libxi-dev
#
# then you'll have OpenGL on your OS.


all: Game.cpp
	g++ -o exec.o $(files) Game.cpp $(args)

win: Game.cpp
	#g++ -o game.o Game.cpp classes/Entity.cpp classes/Cat.cpp classes/Mouse.cpp -I"C:\MinGW\freeglut\include"
	g++ -o test.exe exec.o -L"C:\MinGW\freeglut\lib\x64" -lopengl -lglu -Wl,--subsystem,windows 


run: exec
	./exec

clear:
	$(RM) exec

args = -lglut -lGLU -lGL -O2

files = classes/Entity.cpp  \
		classes/Cat.cpp \
		classes/Mouse.cpp