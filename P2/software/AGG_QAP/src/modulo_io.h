#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

/*
   Realiza la lectura de las matrices, las guarda en vectores de vectores de la stl
   y devuelve el tamaño del problema.
*/

int lecturaMatrices(vector<vector<int> > &m1, vector<vector<int> > &m2, fstream & file){
   string cad;

   getline(file, cad);

   int tam = atoi( cad.c_str());

   m1.resize(tam);
   m2.resize(tam);

   for(int i=0; i < tam; ++i){
      m1[i].resize(tam);
      m2[i].resize(tam);
   }

   for(int i = 0; i < tam && !file.eof(); ++i){
      for(int j = 0; j < tam && !file.eof(); ++j){
         file >> cad;

         m1[i][j] = atoi( cad.c_str());
      }
   }

   for(int i = 0; i < tam && !file.eof(); ++i){
      for(int j = 0; j < tam && !file.eof() && cad != "\n"; ++j){
         file >> cad;

         m2[i][j] = atoi( cad.c_str());
      }
   }


   return tam;
}
