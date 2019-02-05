LIBS=-L/usr/X11/lib/ -lGL -lGLU -lglut -lm -lX11 -lpthread

all: ejemplo

ejemplo: opengl_textura.cc ./CImg.h
	g++ -o ejemplo -std=c++11 -I./ opengl_textura.cc $(LIBS)

clean:
	$(RM) *.o *~
