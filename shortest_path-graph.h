#ifndef SHORTEST_PATHGRAPH_H
#define SHORTEST_PATHGRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>

using namespace std;

#define POS_INF 1000000000
#define NEG_INF -1000000000

class GrafoListaAdj {
    private:
        vector<string> vertices;

        //first é o indice do vertice, second é o peso (caso o grafo seja ponderado)
        vector<vector<pair<int, int>>> arestas;

        int obterIndiceVertice(string rotuloVertice) {
            vector<string>::iterator itr;
            itr = find ( vertices.begin (), vertices.end (), rotuloVertice );

            if ( itr == vertices.end () ) return -1;

            return distance ( vertices.begin (), itr );
        }

        void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
            int idx = obterIndiceVertice ( rotuloVOrigem );
            if ( indicesVerticesVisitados [ idx ] ) return;

            indicesVerticesVisitados [ idx ] = true;

            vector<pair<int, int>> vizinhos = arestas [ idx ];
            for ( unsigned i = 0; i < vizinhos.size (); i++ ){
                int aux = vizinhos.at ( i ).first;
                string rotulo;
                stringstream sstm;
                sstm << "v" << ( vertices.at ( 0 ) == "v0" ? aux : aux + 1 );
                rotulo = sstm.str();
                dfs ( rotulo, indicesVerticesVisitados );
            }
        }

    public:
        void inserirVertice(string rotuloVertice) {
            vector<string>::iterator itr;
            itr = find ( vertices.begin (), vertices.end (), rotuloVertice );

            if ( itr != vertices.end () ) return;

            vertices.push_back ( rotuloVertice );
            arestas.push_back ( vector<pair<int, int>> () );
        }

        void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino) {
            inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
        }

        void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino) {
            inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
            inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, 1);
        }

        void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
            inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, peso);
            inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, peso);
        }

        void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
            int i = obterIndiceVertice ( rotuloVOrigem );
            int j = obterIndiceVertice( rotuloVDestino );

            if ( i == -1 || j == -1 ) return;

            pair<int, int> aux;

            aux.first = j;
            aux.second = peso;

            arestas.at ( i ).push_back ( aux );
        }

        bool saoConectados(string rotuloVOrigem, string rotuloVDestino) {
            int i = obterIndiceVertice ( rotuloVOrigem );
            int j = obterIndiceVertice( rotuloVDestino );

            if ( i == -1 || j == -1 ) return false ;

            if ( arestas.at ( i ).size () == 0 ) return false;

            vector<pair<int, int>> aux = getArestas ().at ( i );

            if ( aux.at ( 0 ).first == j ) return true;
            return false;
        }

        bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
            bool visitados [ 100 ];
            for ( int i = 0; i < 100; i++ ) visitados [ i ] = false;

            if ( rotuloVOrigem == rotuloVDestino ) {
                int idx = obterIndiceVertice ( rotuloVOrigem );
                bool flag = false;
                for ( unsigned j = 0; j < getArestas ().at ( idx ).size (); j++ ) {
                    if ( getArestas ().at ( idx ).at ( j ).first == idx ) flag = true;
                }

                if ( !flag ) return false;
                return true;
            }

            dfs ( rotuloVOrigem, visitados );

            if ( visitados [ obterIndiceVertice ( rotuloVDestino ) ] ) return true;
            return false;
        }

        int colorir() {
            bool visitados [ 100 ];
            bool colorido [ 100 ];
            for ( unsigned int i = 0; i < vertices.size (); i++ ) {
                visitados [ i ] = false;
                colorido [ i ] = false;
            }

            int count = 0;
            for ( unsigned int i = 0; i < vertices.size (); i++ ) {
                if ( !visitados [ i ] ) {
                    dfs ( vertices.at ( i ), visitados );
                    for ( unsigned int j = 0; j < vertices.size (); j++ ) {
                        if ( visitados [ j ] && !colorido [ j ] ) {
                            vertices.at ( j ) = vertices.at ( i );
                            colorido [ j ] = true;
                        }
                    }
                    count++;
                }
            }
            return count;
        }

        int* bfs(string rotuloVOrigem) {
            bool visitados [ 100 ];
            int *distancias = ( int* ) malloc ( 30 * sizeof ( int ) );
            queue<pair<int, int>> fila;

            for ( unsigned int i = 0; i < vertices.size (); i++ ) visitados [ i ] = false;
            for ( unsigned int i = 0; i < vertices.size (); i++ ) distancias [ i ] = 0;

            int idx = obterIndiceVertice ( rotuloVOrigem );
            visitados [ idx ] = true;
            distancias [ idx ] = 0;
            fila.push ( make_pair ( idx, 0 ) );

            while ( !fila.empty () ) {
                pair<int, int> temp = fila.front ();
                fila.pop ();

                for ( pair<int, int> aux : arestas [ temp.first ] ) {
                    if ( !visitados [ aux.first ] ) {
                        visitados [ aux.first ] = true;
                        distancias [ aux.first ] = distancias [ temp.first ] + 1;
                        fila.push ( aux );
                    }
                }
            }
            return distancias;
        }

        int* bellmanFord(string rotuloVOrigem) {
            unsigned int tamanho = getVertices ().size ();
            int src = obterIndiceVertice ( rotuloVOrigem );

            int *distancias = ( int* ) malloc ( tamanho * sizeof ( int ) );

            for ( unsigned i = 0; i < tamanho; i++ ) distancias [ i ] = POS_INF;
            distancias [ src ] = 0;

            for ( unsigned int i = 0; i <= tamanho - 1; i++ ) {
                for ( unsigned int j = 0; j < tamanho; j++ ) {
                    for ( auto aux : arestas [ j ] ) {
                        int b = aux.first;
                        int peso = aux.second;
                        distancias [ b ] = min ( distancias [ b ], distancias [ j ] + peso );
                    }
                }
            }

            for ( unsigned int i = 0; i < tamanho; i++ ) {
                for ( auto aux : arestas [ i ] ) {
                    int b = aux.first;
                    int peso = aux.second;
                    if ( distancias [ i ] + peso < distancias [ b ] ) distancias [ b ] = NEG_INF;
                }
            }

            for ( unsigned int i = 0; i < tamanho; i++ ) {
                if ( ( int ) i != src ) {
                    if ( !haCaminho ( rotuloVOrigem, vertices.at ( i ) ) ) {
                        distancias [ i ] = POS_INF;
                    }
                }
            }

            return distancias;
        }

        int* dijkstra(string rotuloVOrigem) {
            priority_queue<pair<int,int>> fila;

            unsigned int tamanho = getVertices ().size ();
            int src = obterIndiceVertice ( rotuloVOrigem );

            int *distancias = ( int* ) malloc ( tamanho * sizeof ( int ) );
            int processado [ tamanho ];

            for ( unsigned i = 0; i < tamanho; i++ ) {
                distancias [ i ] = POS_INF;
                processado [ i ] = false;
            }
            distancias [ src ] = 0;

            fila.push ( { 0, src } );
            while ( !fila.empty () ) {
                int temp = fila.top ().second;
                fila.pop ();

                if ( processado [ temp ] ) continue;
                processado [ temp ] = true;

                for ( pair<int, int> aux : arestas [ temp ] ) {
                    int b = aux.first;
                    int peso = aux.second;
                    if ( distancias [ temp ] + peso < distancias [ b ] ) {
                        distancias [ b ] = distancias [ temp ] + peso;
                        fila.push ( { -distancias [ b ], b } );
                    }
                }
            }
            return distancias;
        }

        vector<string> getVertices() {
            return vertices;
        }

        vector<vector<pair<int, int>>> getArestas() {
            return arestas;
        }
};

#endif // SHORTEST_PATHGRAPH_H
