TARGETS:=graphics3.exe graphics3.out
OBJS:= PixelPlane.o Polygon.o Unit.o Vertex.o World.o Line.o Projection.o Triangle.o main.o

all: $(TARGETS)

$(TARGETS) : $(OBJS)  
	g++ -ansi -Wall -o $@ $^ -lglut -lGL

%.o : %.cpp %.h
	g++ -ansi -Wall -c $< -lglut -lGL

clean : 
	rm -f $(OBJS)     
