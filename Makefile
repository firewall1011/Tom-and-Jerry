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
	g++ -o exec $(files) Game.cpp $(args)

run: exec
	./exec

clear:
	$(RM) exec

args = -lglut -lGLU -lGL -O2
files = classes/Entity.cpp classes/Cat.cpp classes/Mouse.cpp