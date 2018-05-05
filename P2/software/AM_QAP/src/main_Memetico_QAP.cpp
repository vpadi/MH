#include <string>
#include <fstream>
#include <chrono>
#include "modulo_io.h"
#include "QAP.hpp"
#include "AGG_QAP_Posicion.hpp"
#include "LS_QAP.cpp"

using namespace std;

// Global variables
unsigned int TAM;
vector<vector<int> > flux, dist;

int main(int argc, char *argv[]){
   if(argc < 3){
      cerr << "\nFalta la semilla o el fichero de datos. La sintaxis es: ./Programa archivo_datos semilla. Exit. \n\n";
      return 1;
   }

   /* Intentamos abrir el fichero de datos */
   fstream fichero_datos(argv[1]);

   if(!fichero_datos){
      cerr << "No se ha podido abrir el fichero. Abort" << endl;
      abort();
   }

   /* Inicializamos la semilla para obtener siempre los mismos resultados */
   eo::rng.reseed(atoi(argv[2]));
   /* Leemos la matrices de datos. En los ficheros, la primera matriz es de flujo y la segunda de distancias */
   TAM = lecturaMatrices(flux,dist,fichero_datos);

    /* Inicializamos una solución */
   ProblemaInit chromInit;

   int tamPob = 50;
   eoPop<Problema> pop(tamPob, chromInit);  // Inicializamos la poblacion

    /* Declaramos la clase que envuelve la función de evaluacion */
   ProblemaEvalFunc eval;

   size_t tam_torneo = 2; // El tamaño del torneo, en este caso binario

   /*
      Con esta clase, indicamos que el torneo debe realizarse de manera
      determinista. Es decir, los individuos con mejores soluciones tendrán
      mayor probabilidad de escogerse (El mejor tiene probabilidad 1)

      Esta solo escoge UNO, ya que deriva de la clase eoSelectONE
   */
   eoDetTournamentSelect<Problema> selecInd(tam_torneo);

   /*
      Con esta clase, seleccionamos varios individuos como queramos
      utilizando como motor de seleccion una clase derivada de
      eoSelectONE (en este caso eoDetTournamentSelect).

      Hay varias clases correspondiente a la forma indicarle
      cuantos individuos queremos. En este caso hemos escogido
      "eoSelectPerc", que seleccionará tantos individuos como
      porcentaje le pasemos:
      (Numero individuos seleccionados = Nº individuos poblacion * porcentaje)
   */
   eoSelectPerc<Problema> seleccion(selecInd/*, porcentaje elegido*/);// El porcentaje por defecto es 1.0

   ProblemaCruce Cruce;  // CRUCE
   double probCruce = 0.7; //Probabilidad de cruce

   ProblemaSwapMutacion  mutacionSwap;  // MUTACION
   double pMut = 0.001; // Probabilidad de mutación

   /* Creamos los objetos necesarios para realizar una Búsqueda Local */
   Vecindario vecin;
   QAPIncrEval funcIncr;

   moIterContinuator<Vecino> contIter(400);

 	moSimpleHC<Vecino> hill_climbing(vecin, eval, funcIncr, contIter);

   // Los operadores estan encapsulados en un eoTRansform object
   eoSGATransform<Problema> transform(Cruce, probCruce, mutacionSwap, pMut);

   // Motor de remplazo
	eoDeterministicSaDReplacement<Problema> remplazo(1, tamPob-1, tamPob-1, 1, false);

   eoGenContinue<Problema> genCont(10); // number of maximum generation;

   eoEasyEA<Problema> gga(genCont, eval, seleccion, transform, remplazo);

   unsigned int iter_totales = 0;

   // A su vez, realizamos la medida de tiempo
   auto t_inicio = chrono::system_clock::now();

   do{
      // Aplicamos el algoritmo durante 10 iteraciones
      gga(pop);

      // Aplicamos la local search (Ascension de Colinas) a cada cromosoma
      for(eoPop<Problema>::iterator it = pop.begin(); it != pop.end(); it++){
         hill_climbing(*it);
      }

      iter_totales += 400 * pop.size() + 10;
   }while(iter_totales < 50000);

   auto t_fin = chrono::system_clock::now();

   chrono::duration<double> tiempo_total = t_fin - t_inicio;

   /****************************************************************
   ** Imprimimos la poblacion final en orden y el tiempo tardado ***
   *****************************************************************/
   const Problema best = pop.best_element();

   cout << endl << "Mejor individuo (Solucion) final:" << endl;
   cout << best << endl;

   cout << "Tiempo tardado: " << tiempo_total.count() << "s\n";
}
