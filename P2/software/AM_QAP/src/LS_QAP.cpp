#ifndef _LS_QAP_
#define _LS_QAP_

#include <mo>
#include "QAP.hpp"

class Vecino : public moNeighbor<Problema>{
private:
   std::pair<unsigned int, unsigned int> indices;
public:

   void move(Problema& _problema){
      std::swap(_problema.solucion[indices.first], _problema.solucion[indices.second]);
   }

   void setIndices(unsigned int _first, unsigned int _second) {
      indices.first = _first;
      indices.second = _second;
   }

   void getIndices(unsigned int & _first, unsigned int & _second) {
      _first = indices.first;
      _second = indices.second;
   }

};

class Vecindario : public moNeighborhood<Vecino> {
private:
   std::pair<unsigned int, unsigned int> indices;
public:
   bool hasNeighbor(Problema& _problema){
      return (_problema.solucion.size() > 1);
   }

   void init(Problema& _problema, Vecino& _actual){
      indices.first = 0;
      indices.second = 1;
      _actual.setIndices(0,1);
   }

   void next(Problema& _problema, Vecino& _actual){
      if (indices.second == _problema.solucion.size()-1) {
          indices.first++;
          indices.second=indices.first+1;
      }
      else
          indices.second++;
      _actual.setIndices(indices.first, indices.second);
   }

   bool cont(Problema& _problema) {
      return !((indices.first == (_problema.solucion.size()-2)) && (indices.second == (_problema.solucion.size() -1 )));
   }
};

class QAPIncrEval : public moEval<Vecino> {
public:

   void operator()(Problema& _problema, Vecino & _vecino) {
     int d;
     int k;

     unsigned i, j;

     _vecino.getIndices(i, j);

     d = (flux[i][i]-flux[j][j])*(dist[_problema.solucion[j]][_problema.solucion[j]]-dist[_problema.solucion[i]][_problema.solucion[i]]) +
         (flux[i][j]-flux[j][i])*(dist[_problema.solucion[j]][_problema.solucion[i]]-dist[_problema.solucion[i]][_problema.solucion[j]]);

      for (k = 0; k < TAM; k++)
         if (k != i && k != j)
            d = d + (flux[k][i]-flux[k][j])*(dist[_problema.solucion[k]][_problema.solucion[j]]-dist[_problema.solucion[k]][_problema.solucion[i]]) +
            (flux[i][k]-flux[j][k])*(dist[_problema.solucion[j]][_problema.solucion[k]]-dist[_problema.solucion[i]][_problema.solucion[k]]);

     _vecino.fitness(_problema.fitness() + d);
   }
};

#endif
