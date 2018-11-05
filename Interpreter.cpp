//****************************** Interpreter.cpp *************************************
#include "interpreter.hpp"

fstream Entrada;

bool prepare_file(string s){
    return FileControl::openFile(Entrada, s, ENTRADA_SAIDA); 
}

int is_reserved(string s){
    auto it = lower_bound(Reserved_Words.begin(), Reserved_Words.end(), s);
    return *it == s ? it - Reserved_Words.begin() : -1;
}

bool close_file(){
    Entrada.close();
    return true;
}

char get_next_char(){
    return Entrada.get();
}

void unget_char(char c){
    Entrada.put(c);
}

bool Token::is_final(){
    return this->cat == FINAL;
}

void Token::print(){
    cout<<"Categoria: ";
    switch(this->cat){
        case IDENTIFIER:
            cout<<"Identificador"<<endl;
            cout<<"Valor: "<< *cstring(this->valor)<<endl;
            break;
        case CINT:
            cout<<"Integer"<<endl;
            cout<<"Valor: "<< *cint(this->valor)<<endl;
            break;
        case CFLOAT:
            cout<<"Float"<<endl;
            cout<<"Valor: "<< *cfloat(this->valor)<<endl;
            break;
        case CCHAR:
            cout<<"Caracter"<<endl;
            cout<<"Valor: "<< *cchar(this->valor)<<endl;
            break;
        case CSTRING:
            cout<<"String"<<endl;
            cout<<"Valor: "<< cstring(this->valor)<<endl;
            break;
        case OPERATOR:
            cout<<"Operator"<<endl;
            switch(*cchar(this->valor)){
                case MAIS:
                    cout<<"Valor: "<< "+"<<endl;
                    break;
                case MENOS:
                    cout<<"Valor: "<< "-"<<endl;
                    break;
                case VEZES:
                    cout<<"Valor: "<< "*"<<endl;
                break;
                case DIVISAO:
                    cout<<"Valor: "<< "/"<<endl;
                break;
                case ATRIBUICAO:
                    cout<<"Valor: "<< "="<<endl;
                break;
                case PONTO:
                    cout<<"Valor: "<< "."<<endl;
                break;
            }
            break;
        case LOGICAL:
            cout<<"Logical"<<endl;
            switch(*cchar(this->valor)){
                case NOT:
                    cout<<"Valor: "<< "!"<<endl;
                    break;
                case IGUAL:
                    cout<<"Valor: "<< "="<<endl;
                    break;
                case MAIOR:
                    cout<<"Valor: "<< ">"<<endl;
                break;
                case MENOR:
                    cout<<"Valor: "<< "<"<<endl;
                break;
                case MAIOR_IGUAL:
                    cout<<"Valor: "<< ">="<<endl;
                break;
                case MENOR_IGUAL:
                    cout<<"Valor: "<< "<="<<endl;
                break;
            }
            break;
    }
}

Token get_token(){
    int estado = 0;
    static int linha = 1, coluna = 1;
    string buffer;
    
    while(true){
        char c;
        switch(estado){

        }
    }
}