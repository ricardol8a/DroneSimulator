PATHCONFIG = config
PATHDATA = data
PATHHELP = doc
PATHHEADER = include
PATHSOURCE = src
PATHLIBRARY = lib
PATHBUILD = build
PATHBINARY = bin
EXECUTABLE = setup

#\
!ifndef 0
#\
CXX = "cl /NOLOGO"
#\
LINK = "link /NOLOGO"
#\
!if (defined(MODE) && ($(MODE) == DEBUG))
	#\
	CXXFLAGS += " /Zi"
	#\
	LDFLAGS += " /DEBUG /Wall"
#\
!endif
#\
CXXFLAGS += " /Ox /GA"
#\
!ifdef VIDEO
	#\
	CXXFLAGS += " /DVIDEO="$(VIDEO)
#\
!endif
#\
!ifdef MATH
	#\
	CXXFLAGS += " /DMATH="$(MATH)
#\
!endif
#\
!ifdef MODEL
	#\
	CXXFLAGS += " /DMODEL="$(MODEL)
#\
!endif
#\
!ifdef CONTROL
	#\
	CXXFLAGS += " /DCONTROL="$(CONTROL)
#\
!endif
#\
EXECUTABLE = $(EXECUTABLE).exe
#\
all: $(PATHBINARY)\$(EXECUTABLE)
#\
$(PATHBINARY)\$(EXECUTABLE): $(PATHBUILD)\*.obj
#\
	$(LINK) $(LDFLAGS) /LIBPATH:$(PATHLIBRARY) /OUT:$@ $**
#\
{$(PATHSOURCE)}.cpp{$(PATHBUILD)}.obj::
#\
	$(CXX) $(CXXFLAGS) /DCONFIG=\"$(PATHCONFIG)\\\" /DDATA=\"$(PATHDATA)\\\" /I$(PATHHEADER) /c /Fo$(PATHBUILD)\ $<
#\
clean:
#\
	del /F $(PATHBUILD)\*.obj $(PATHBINARY)\$(EXECUTABLE)
#\
!else
CXX = g++ -std=c++11
ifeq ($(MODE), DEBUG)
	CXXFLAGS += -g -Wall
endif
CXXFLAGS += -O3 -pipe
ifdef VIDEO
	CXXFLAGS += -DVIDEO=$(VIDEO)
endif
ifdef MATH
	CXXFLAGS += -DMATH=$(MATH)
endif
ifdef MODEL
	CXXFLAGS += -DMODEL=$(MODEL)
endif
ifdef CONTROL
	CXXFLAGS += -DCONTROL=$(CONTROL)
endif
ifeq ($(OS), Windows_NT)
	TARGET += .exe
	LDFLAGS += -mthreads -mwindows -lglut -lglu32 -lopengl32
else
	ifeq ($(shell uname -s), Darwin)
		CXXFLAGS += -Wno-deprecated-declarations -Qunused-arguments
		LDFLAGS += -framework Carbon -framework OpenGL -framework GLUT
	else
		LDFLAGS += -pthread -lm -lglut -lGLU -lGL
	endif
endif
all: $(PATHBINARY)/$(EXECUTABLE)
$(PATHBINARY)/$(EXECUTABLE): $(patsubst $(PATHSOURCE)/%.cpp,$(PATHBUILD)/%.obj,$(wildcard $(PATHSOURCE)/*.cpp))
	$(CXX) $(CXXFLAGS) -L$(PATHLIBRARY) -o $@ $^ $(LDFLAGS)
$(PATHBUILD)/%.obj: $(PATHSOURCE)/%.cpp
	$(CXX) $(CXXFLAGS) -DCONFIG=\"$(PATHCONFIG)/\" -DDATA=\"$(PATHDATA)/\" -I$(PATHHEADER) -c -o $@ $<
clean:
	rm -f $(PATHBUILD)/*.obj $(PATHBINARY)/$(EXECUTABLE)
#\
!endif
#TIP: cd "Program Files (x86)\Microsoft Visual Studio 14.0\VC" && vcvarsall x86
#TIP: Mode debugging with MODE=DEBUG
#TIP: Quality of Video with VIDEO=NONE/LOW/MEDIUM/HIGH
#TIP: Accuracy of Calculus with MATH=FASTEST/QUICK/BALANCED/BEST
#TIP: Type of Models with MODEL=REAL/VIRTUAL/VIRTUAL2/VIRTUAL3
#TIP: Type of Controls with CONTROL=MANUAL/AUTOMATIC/AUTOMATIC2/AUTOMATIC3/AUTOMATIC4
