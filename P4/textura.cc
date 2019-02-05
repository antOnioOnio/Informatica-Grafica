#include"textura.h"


textura::textura(std::vector<GLfloat> _vertices ,std::vector<GLfloat> _texVertices){
    for( int i = 0; i<_vertices.size() ; i++){
        vertices.push_back(_vertices[i]);
    }
    for( int i = 0; i<_texVertices.size() ; i++){
        texVertices.push_back(_texVertices[i]);
    }
    
}


// ----------------------------------------------------------------------

//-----------------------------------------------------------------------
void textura::prepara_textura (const char* ruta)
{
   std::vector<unsigned char> data;

   CImg<unsigned char> logo;
   logo.load(ruta);

   // empaquetamos bien los datos
   for (long y = 0; y < logo.height(); y ++)
      for (long x = 0; x < logo.width(); x ++)
      {
	 unsigned char *r = logo.data(x, y, 0, 0);
	 unsigned char *g = logo.data(x, y, 0, 1);
	 unsigned char *b = logo.data(x, y, 0, 2);
	 data.push_back(*r);
	 data.push_back(*g);
	 data.push_back(*b);

     
      }

   glGenTextures(1, &textura_id);
   glBindTexture(GL_TEXTURE_2D, textura_id);

   glActiveTexture(GL_TEXTURE0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // TRASFIERE LOS DATOS A GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, logo.width(), logo.height(),
		0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}

// ----------------------------------------------------------------------

//-----------------------------------------------------------------------
void textura::dibuja (int dimensiones, int tamanio)
{
   glEnable(GL_TEXTURE_2D);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textura_id);

   glVertexPointer(dimensiones, GL_FLOAT, 0, vertices.data());
   glTexCoordPointer(2, GL_FLOAT, 0, texVertices.data());

   glDrawArrays(GL_QUADS, 0, tamanio);

   glDisableClientState(GL_VERTEX_ARRAY);
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);
}
// ----------------------------------------------------------------------

//-----------------------------------------------------------------------
void textura::libera_textura (void)
{
   glDeleteTextures(1, &textura_id);
}

