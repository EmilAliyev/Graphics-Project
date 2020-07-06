graphics3.exe : PixelPlane.o Polygon.o Unit.o Vertex.o World.o Line.o Projection.o Triangle.o main.o  
	g++ -ansi -Wall -o graphics3.exe PixelPlane.o Polygon.o Unit.o Vertex.o World.o Line.o Projection.o Triangle.o main.o -lglut -lGL

PixelPlane.o : PixelPlane.cpp PixelPlane.h Polygon.h
	g++ -ansi -Wall -c PixelPlane.cpp -lglut -lGL

Polygon.o : Polygon.cpp Polygon.h Vertex.h Line.h Triangle.h
	g++ -ansi -Wall -c Polygon.cpp -lglut -lGL

Unit.o : Unit.cpp Unit.h Polygon.h 
	g++ -ansi -Wall -c Unit.cpp -lglut -lGL
	
Vertex.o : Vertex.cpp Vertex.h
	g++ -ansi -Wall -c Vertex.cpp -lglut -lGL
	
World.o : World.cpp World.h Polygon.h
	g++ -ansi -Wall -c World.cpp -lglut -lGL
	
Line.o : Line.cpp Line.h Vertex.h
	g++ -ansi -Wall -c Line.cpp -lglut -lGL

Projection.o : Projection.cpp Projection.h Polygon.h
	g++ -ansi -Wall -c Projection.cpp -lglut -lGL
	
Triangle.o : Triangle.cpp Triangle.h Vertex.h Line.h
	g++ -ansi -Wall -c Triangle.cpp -lglut -lGL
	
main.o : main.cpp PixelPlane.h Polygon.h Unit.h World.h Projection.h
	g++ -ansi -Wall -c main.cpp -lglut -lGL 


clean : 
	rm -f graphics3.exe PixelPlane.o Polygon.o Unit.o Vertex.o World.o Line.o Projection.o Triangle.o main.o     
