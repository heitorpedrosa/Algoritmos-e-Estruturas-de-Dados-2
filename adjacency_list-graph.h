#ifndef ADJACENCY_LISTGRAPH_H
#define ADJACENCY_LISTGRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class GrafoListaAdj {
    private:
        vector<string> vertices;

        //first é o indice do vertice, second é o peso (caso o grafo seja ponderado)
        vector<vector<pair<int, int>>> arestas;

        int obterIndiceVertice ( string rotuloVertice ) {
            vector<string>::iterator itr;
            itr = find ( vertices.begin (), vertices.end (), rotuloVertice );

            if ( itr == vertices.end () ) return -1;

            return distance ( vertices.begin (), itr );
        }

    public:
        void inserirVertice ( string rotuloVertice ) {
            vector<string>::iterator itr;
            itr = find ( vertices.begin (), vertices.end (), rotuloVertice );

            if ( itr != vertices.end () ) return;

            vertices.push_back ( rotuloVertice );
            arestas.push_back ( vector<pair<int, int>> () );
        }

        void inserirArestaDirecionada ( string rotuloVOrigem, string rotuloVDestino ) {
            inserirArestaDirecionada ( rotuloVOrigem, rotuloVDestino, 1 );
        }

        void inserirArestaNaoDirecionada ( string rotuloVOrigem, string rotuloVDestino ) {
            inserirArestaDirecionada ( rotuloVOrigem, rotuloVDestino, 1 );
            inserirArestaDirecionada ( rotuloVDestino, rotuloVOrigem, 1 );
        }

        void inserirArestaNaoDirecionada ( string rotuloVOrigem, string rotuloVDestino, int peso ) {
            inserirArestaDirecionada ( rotuloVOrigem, rotuloVDestino, peso );
            inserirArestaDirecionada ( rotuloVDestino, rotuloVOrigem, peso );
        }

        void inserirArestaDirecionada ( string rotuloVOrigem, string rotuloVDestino, int peso ) {
            int i = obterIndiceVertice ( rotuloVOrigem );
            int j = obterIndiceVertice( rotuloVDestino );

            if ( i == -1 || j == -1 ) return;

            pair<int, int> aux;

            aux.first = j;
            aux.second = peso;

            arestas.at ( i ).push_back ( aux );
        }

        bool saoConectados ( string rotuloVOrigem, string rotuloVDestino ) {
            int i = obterIndiceVertice ( rotuloVOrigem );
            int j = obterIndiceVertice( rotuloVDestino );

            if ( i == -1 || j == -1 ) return false ;

            if ( arestas.at ( i ).size () == 0 ) return false;

            vector<pair<int, int>> aux = getArestas ().at ( i );

            if ( aux.at ( 0 ).first == j ) return true;
            return false;
        }

        vector<string> getVertices () {
            return vertices;
        }

        vector<vector<pair<int, int>>> getArestas () {
            return arestas;
        }
};

#endif // ADJACENCY_LISTGRAPH_H
