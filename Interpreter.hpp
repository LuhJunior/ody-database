//****************************** Interpreter.h *************************************
#ifndef Interpreter_h
#define Interpreter_h

#include "FileController.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>


/*
class Term{
public:
    Term(string term){
        this->term = term;
    }
protected:
    string term;
};

class Comand:Term{
public:
    bool isComand(){
        for(string x : comands) if(term == x) return true;
        return false;
    }
    static bool isComand(string term){
        for(string x : comands) if(term == x) return true;
        return false;
    }
private:
    const vector<string> comands = {"SELECT", "INSERT", "DELETE", "UPDATE"};
};

class Identifier:Term{
public:
    bool isIdentifier(){
        return !isComand(term) && !isConnecter(term);
    }
};

class Connecter:Term{
public:
    bool isConnecter(){
        for(string x : connectors) if(term == x) return true;
        return false;
    }
    static bool isConnecter(string term){
        for(string x : connectors) if(term == x) return true;
        return false;
    }
private:
    const vector<string> connectors = {"FROM", "INTO", "ON", "IN"};
};

class Expression{
public:
    Expression(vector<Term> terms){
        this->terms = terms;
    }
    bool analisarEexecutar(){
        if(terms[0].term == "SELECT"){
            if(terms[1].isIdentifier() || terms[1].term == "*"){
                string nomeColuna = terms[1].term;
                if(terms[2].isConnecter()){
                    if(terms[3].isIdentifier()){
                        string nomeBanco = terms[3].term;
                        if(terms[4].isIdentifier()){
                            string nomeTabela = terms[4].term;
                            if(terms.size() == 5){
                                DataBase db(nomeBanco);
                                string meta;
                                vector<MemRegister> mr(db.getRegister({nomeTabela, nomeColuna}, meta));
                            }
                        }
                    }
                }
            }
        }
        else if(terms[0].term == "INSERT"){

        }
        else if(terms[0].term == "UPDATE"){

        }
        else if(terms[0].term == "DELETE"){

        }
        return FileControl::Error("Expressao Invalido!", false);
    }
private:
    vector<Term> terms;
};
*/

vector<string> Reserved_Words = { "ADD", "ALTER", "COLUMN", "CREATE", "DATABASE", "DELETE", "FROM", "IN", 
    "INSERT", "INTO", "ON", "SELECT", "SET", "TABLE", "UPDATE", "VALUES", "WHERE" };

typedef enum { IDENTIFIER, OPERATOR, LOGICAL, CINT, CFLOAT, CCHAR, CSTRING, FINAL } Categoria;

typedef enum { MAIS, MENOS, VEZES, DIVISAO, ATRIBUICAO, PONTO } Operator;

typedef enum { NOT, IGUAL, MAIOR, MENOR, MAIOR_IGUAL, MENOR_IGUAL } Logical;

class Token{
public:
    Token(Categoria cat, void *valor){
        this->cat = cat;
        this->valor = nullptr;
        switch(this->cat){
            case IDENTIFIER:
                this->valor = new string;
                *cstring(this->valor) = *cstring(valor);
                break;
            case CINT:
                this->valor = new int;
                *cint(this->valor) = *cint(valor);
                break;
            case CFLOAT:
                this->valor = new float;
                *cfloat(this->valor) = *cfloat(valor);
                break;
            case CCHAR:
                this->valor = new char;
                *cchar(this->valor) = *cchar(valor);
                break;
            case CSTRING:
                this->valor = new string;
                *cstring(this->valor) = *cstring(valor);
                break;
            case OPERATOR:
                this->valor = new char;
                *cchar(this->valor) = *cchar(valor);
                break;
            case LOGICAL:
                this->valor = new char;
                *cchar(this->valor) = *cchar(valor);
                break;
        }
    }
    ~Token(){
        delete this->valor;
    }
    void print();
    bool is_final();
private:
    Categoria cat;
    void *valor;
};

int is_reserved(string);

Token get_token();

char get_next_char();

void unget_char(char);

bool prepare_file(string);

bool close_file();

#endif // Interpreter
