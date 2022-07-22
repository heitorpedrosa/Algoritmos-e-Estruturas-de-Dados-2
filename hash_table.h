#ifndef TABELAHASH2EXERCICIO_H
#define TABELAHASH2EXERCICIO_H

#include<iostream>
#include <vector>

using namespace std;

template <typename K, typename V>
class Tupla {
    private:
        K chave;
        V valor;
        Tupla* prox;
    public:
        Tupla ( K c, V v ) {
            chave = c;
            valor = v;
            prox = NULL;
        }

        K getChave () {
            return chave;
        }

        V getValor () {
            return valor;
        }

        Tupla* getProx () {
            return prox;
        }

        void setProx ( Tupla* prox ) {
            Tupla::prox = prox;
        }
};

template <typename Chave, typename Valor>
class TabelaHash2 {

    private:
        Tupla<Chave, Valor>** tabela;

        //tamanho atual do array
        int qtdeBuckets;
        //qtdade de elementos já inseridos na tabela hash
        int qtdadeElementos;

        bool inserir ( Chave c, Valor v, bool novoElemento ) {
            int codHash = hash<Chave>{}( c );
            int indice = abs ( codHash ) % qtdeBuckets;

            if ( !novoElemento ) {
                if ( tabela [ indice ] != NULL ) {
                    int indice_antigo = abs ( codHash ) % ( qtdeBuckets / 8 );
                    if ( indice_antigo == indice ) return false;
                }
            }
            if ( tabela [ indice ] == NULL ) tabela [ indice ] = new Tupla<Chave, Valor>( c, v );
            else {
                Tupla<Chave, Valor>* aux = new Tupla<Chave, Valor>( c, v );
                aux -> setProx ( tabela [ indice ] );
                tabela [ indice ] = aux;
            }
            return true;
        }

        void aumentaArray () {
            qtdeBuckets = qtdeBuckets * 8;
            tabela = ( Tupla<Chave, Valor>** ) realloc ( tabela, qtdeBuckets * sizeof ( Tupla <Chave, Valor>* ) );
            for ( int i = qtdeBuckets / 8; i < qtdeBuckets; i++ ) tabela [ i ] = NULL;

            for ( int i = 0; i < qtdeBuckets / 8; i++ ) {
                if ( tabela [ i ] != NULL ) {
                    Tupla<Chave, Valor>* aux = tabela [ i ];
                    Tupla<Chave, Valor>* temp = aux -> getProx ();
                    Tupla<Chave, Valor>* anterior;
                    while ( aux != NULL ) {
                        if ( aux == tabela [ i ] ) {
                            if ( temp == NULL ) {
                                if ( inserir ( aux -> getChave (), aux -> getValor (), false ) ) {
                                    tabela [ i ] = NULL;
                                    delete ( aux );
                                    break;
                                }
                            }
                            else {
                                if ( inserir ( aux -> getChave (), aux -> getValor (), false ) ) {
                                    tabela [ i ] = temp;
                                    delete ( aux );
                                }
                                anterior = aux;
                            }
                        }
                        else {
                           if ( inserir ( aux -> getChave (), aux -> getValor (), false ) ) {
                               anterior -> setProx ( temp );
                               delete ( aux );
                            }
                           else anterior = aux;
                        }
                        aux = temp;
                        if ( aux != NULL ) temp = aux -> getProx ();
                    }
                }
            }
        }

    public:
        TabelaHash2 () {
            qtdeBuckets = 8;
            qtdadeElementos = 0;
            tabela = ( Tupla<Chave, Valor>** ) calloc ( qtdeBuckets, sizeof ( Tupla<Chave, Valor>* ) );
            for ( int i = 0; i < qtdeBuckets; i++ ) tabela [ i ] = NULL;
        }

        void inserir ( Chave c, Valor v ) {
            if ( load_factor () >= 1 ) aumentaArray ();
            inserir ( c, v, true );
            qtdadeElementos++;
        }

        double load_factor () {
            return ( double ) size () / ( double) bucket_count ();
        }

        Valor getValor ( Chave chave ) {
            if ( !contemChave ( chave ) ) return NULL;

            int codHash = hash<Chave>{}( chave );
            int indice = abs ( codHash ) % qtdeBuckets;

            Tupla<Chave, Valor>* aux = tabela [ indice ];
            while ( aux != NULL ) {
                if ( aux -> getChave () == chave ) return aux -> getValor ();
                aux = aux -> getProx ();
            }

        }

        bool contemChave ( Chave chave ) {
            int codHash = hash<Chave>{}( chave );
            int indice = abs ( codHash ) % qtdeBuckets;

            Tupla<Chave, Valor>* aux = tabela [ indice ];
            while ( aux != NULL ) {
                if ( aux -> getChave () == chave ) return true;
                aux = aux -> getProx ();
            }
            return false;
        }

        vector<Chave> getChaves () {
            vector<string> chaves;
            for ( int i = 0; i < qtdeBuckets; i++ ) {
                 Tupla<Chave, Valor>* aux = tabela [ i ];
                 while ( aux != NULL ) {
                     chaves.push_back( aux -> getChave () );
                     aux = aux -> getProx ();
                 }
            }
            return chaves;
        }

        void clear () {
            for ( int i = 0; i < qtdeBuckets; i++ ) delete ( tabela [ i ] );
            qtdeBuckets = 8;
            qtdadeElementos = 0;
            tabela = ( Tupla <Chave, Valor>** ) realloc ( tabela, qtdeBuckets * sizeof ( Tupla <Chave, Valor>* ) );
        }

        void remover ( Chave chave ) {
            if ( contemChave ( chave ) ) {
                int codHash = hash<Chave>{}( chave );
                int indice = abs ( codHash ) % qtdeBuckets;

                Tupla<Chave, Valor>* aux = tabela [ indice ];
                Tupla<Chave, Valor>* temp = aux -> getProx ();
                Tupla<Chave, Valor>* anterior;
                while ( aux != NULL ) {
                    if ( aux == tabela [ indice ] ) {
                        if ( temp == NULL ) {
                            tabela [ indice ] = NULL;
                            delete ( aux );
                            qtdadeElementos--;
                            break;
                        }
                        else {
                            if ( aux -> getChave () == chave ) {
                                tabela [ indice ] = temp;
                                delete ( aux );
                                qtdadeElementos--;
                            }
                            else anterior = aux;
                        }
                    }
                    else {
                       if ( aux -> getChave () == chave ) {
                           anterior -> setProx ( temp );
                           delete ( aux );
                           qtdadeElementos--;
                        }
                       else anterior = aux;
                    }
                    aux = temp;
                    if ( aux != NULL ) temp = aux -> getProx ();
                }
            }
        }

        int size () {
            return qtdadeElementos;
        }

        int bucket_count () {
            return qtdeBuckets;
        }
};

#endif // TABELAHASH2EXERCICIO_H
