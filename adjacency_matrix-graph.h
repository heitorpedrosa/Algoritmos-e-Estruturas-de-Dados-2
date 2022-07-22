#ifndef ADJACENCY_MATRIXGRAPH_H
#define ADJACENCY_MATRIXGRAPH_H

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

struct GrafoMatrizAdj {
    int** arestas;
    char** rotuloVertices;
    int verticesInseridos;
    int maxNumVertices;
};

struct GrafoMatrizAdj* inicializar ( int numVertices, bool ponderado ) {
    struct GrafoMatrizAdj* grafo = ( struct GrafoMatrizAdj* ) malloc ( sizeof ( struct GrafoMatrizAdj ) );

    grafo -> arestas = ( int** ) malloc ( numVertices * sizeof ( int* ) );
    for ( int i = 0; i < numVertices; i++ ) grafo -> arestas [ i ] = ( int* ) malloc ( numVertices * sizeof ( int ) );

    grafo -> rotuloVertices = ( char** ) malloc ( numVertices * sizeof ( char* ) );
    for ( int i = 0; i < numVertices; i++ ) grafo -> rotuloVertices [ i ] = ( char* ) malloc ( 3 * sizeof ( char ) );

    grafo -> verticesInseridos = 0;

    grafo -> maxNumVertices = numVertices;

    if ( ponderado ) {
        for ( int i = 0; i < numVertices; i++ ) {
            for ( int j = 0; j < numVertices; j++ ) grafo -> arestas [ i ] [ j ] = INT_MAX;
        }
    }
    else {
        for ( int i = 0; i < numVertices; i++ ) {
            for ( int j = 0; j < numVertices; j++ ) grafo -> arestas [ i ] [ j ] = 0;
        }
    }

    return grafo;
}

int obterIndiceVertice ( struct GrafoMatrizAdj* grafo, char* rotuloVertice ) {
    for ( int i = 0; i < grafo -> maxNumVertices; i++ ) {
        if ( strcmp ( grafo -> rotuloVertices [ i ], rotuloVertice ) == 0 ) return i;
    }
    return -1;
}

void inserirAresta ( struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso ) {
    int i = obterIndiceVertice( grafo, rotuloVOrigem );
    int j = obterIndiceVertice( grafo, rotuloVDestino );

    if ( i == -1 || j == -1 ) return;

    if ( grafo -> arestas [ 0 ] [ 0 ] == INT_MAX ) grafo -> arestas [ i ] [ j ] = peso;
    else if ( grafo -> arestas [ 0 ] [ 0 ] == 0 ) {
        grafo -> arestas [ i ] [ j ] = 1;
        grafo -> arestas [ j ] [ i ] = 1;
    }
}

void inserirVertice ( struct GrafoMatrizAdj* grafo, char* rotuloVertice ) {
    grafo -> verticesInseridos++;
    grafo -> rotuloVertices [ grafo -> verticesInseridos - 1 ] = rotuloVertice;
}

bool saoConectados ( struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino ) {
    int i = obterIndiceVertice( grafo, rotuloVOrigem );
    int j = obterIndiceVertice( grafo, rotuloVDestino );

    if ( i == -1 || j == -1 ) return false;

    if ( grafo -> arestas [ 0 ] [ 0 ] == INT_MAX ) {
        if ( grafo -> arestas [ i ] [ j ] != INT_MAX ) return true;
    }
    else if ( grafo -> arestas [ 0 ] [ 0 ] == 0 ) {
        if ( grafo -> arestas [ i ] [ j ] != 0 || grafo -> arestas [ j ] [ i ] != 0 ) return true;
    }
    return false;
}

#endif // ADJACENCY_MATRIXGRAPH_H
