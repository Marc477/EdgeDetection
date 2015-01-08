.SUFFIXES:
.SUFFIXES: .o .cpp

CCC        = g++
CCFLAGS    = -g
LIBS       = -lm
EXECUTABLE = all
OBJETS     = MImage.o

all:tp3A tp3B tp3C tp3D tp3E tp3F tp3G

tp3A: tp3A.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp3B: tp3B.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp3C: tp3C.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp3D: tp3D.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp3E: tp3E.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp3F: tp3F.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)

tp3G: tp3G.o $(OBJETS)
	$(CCC) -g -o $@ $@.o $(OBJETS) $(LIBS)
	
.cpp.o:
	$(CCC) $(CCFLAGS) -c $< -o $@ 

clean:
	rm tp3A tp3B tp3C tp3D tp3E tp3F tp3G *~ *.o *.exe
