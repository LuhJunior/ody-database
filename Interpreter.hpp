//****************************** Interpreter.h *************************************
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "FileController.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>

typedef enum { IDENTIFIER, RESERVED, OPERATOR, LOGICAL, TYPE, CBOOL, CINT, CFLOAT, CCHAR, 
    CSTRING, FINAL } Categoria;

typedef enum { MAIS, MENOS, VEZES, DIVISAO, ATRIBUICAO, PONTO, 
    ABRE_PARENTESIS, FECHA_PARENTESIS, VIRGULA, PONTO_VIRGULA } Operator;

typedef enum { NOT, IGUAL, MAIOR, MENOR, MAIOR_IGUAL, MENOR_IGUAL } Logical;

typedef enum { ADD, ALTER, COLUMN, CREATE, DATABASE, DELETE, FROM, IN, 
    INSERT, INTO, ON, SELECT, SET, TABLE, UPDATE, USE, VALUES, WHERE } Words;

class Token{
public:
    Token(){
        this->valor = nullptr;
    }
    Token(Categoria cat, void *valor){
        this->cat = cat;
        this->valor = nullptr;
        switch(this->cat){
            case IDENTIFIER:
                this->valor = new string;
                *cstring(this->valor) = *cstring(valor);
                break;
            case RESERVED:
                this->valor = new char;
                *cchar(this->valor) = *cchar(valor);
                break;
            case TYPE:
                this->valor = new char;
                *cchar(this->valor) = *cchar(valor);
                break;
            case CINT:
                this->valor = new int;
                *cint(this->valor) = stoi(*cstring(valor));
                break;
            case CFLOAT:
                this->valor = new float;
                *cfloat(this->valor) = stof(*cstring(valor));
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
        this->valor = nullptr;
        //free(this->valor);
    }
    Token& operator= (const Token& t){
        this->cat = t.cat;
        this->valor = t.valor;
        return *this;
    }
    void free_token();
    void print();
    bool constant();
    char reserved();
    char oper();
    bool identifier();
    char logical();
    char type();
    string get_string();
    char get_char();
    int get_int();
    float get_float();
    bool is_final();
private:
    Categoria cat;
    void *valor;
};

class Expression{
public:
    Expression(Type tipo, void *valor){
        this->tipo = tipo;
        this->valor = valor;
    }
    /*
    Expression operator+ (Expression e) {
        Type aType;
        void *aValor = nullptr;
        if(this->tipo == e.tipo) {
            aType = this->tipo;
            switch (this->tipo) {
                case INT:
                    aValor = new int;
                    *cint(aValor) = this->get_int() + e.get_int();
                case FLOAT:
                    aValor = new float;
                    *cfloat(aValor) = this->get_float() + e.get_float();
                case CHAR:
                    aValor = new char;
                    *cchar(aValor) = this->get_char() + e.get_char();
                case STRING:
                    aValor = new string;
                    *cstring(aValor) = this->get_string() + e.get_string();
                case BOOL:
                    aValor = new bool;
                    *cbool(aValor) = this->get_bool() + e.get_bool();
                default:
                    Error("Tipo da Expressao invalido");
                    break;
            }
        } else if (this->tipo == INT && e.tipo == CHAR || this->tipo == CHAR && e.tipo == INT) {
            aType = INT;
            aValor = new int;
            if(this->tipo == INT) *cint(aValor) = this->get_int() + e.get_char();
            else *cint(aValor) = this->get_char() + e.get_int();
        } else if (this->tipo == INT && e.tipo == FLOAT || this->tipo == FLOAT && e.tipo == INT) {
            aType = FLOAT;
            aValor = new float;
            if(this->tipo == FLOAT) *cfloat(aValor) = this->get_float() + e.get_int();
            else *cint(aValor) = this->get_int() + e.get_float();
        } else if (this->tipo == INT && e.tipo == BOOL || this->tipo == BOOL && e.tipo == INT) {

        } else if (this->tipo == CHAR && e.tipo == FLOAT || this->tipo == FLOAT && e.tipo == CHAR) {

        } else if (this->tipo == CHAR && e.tipo == BOOL || this->tipo == BOOL && e.tipo == CHAR) {

        } else if (this->tipo == FLOAT && e.tipo == BOOL || this->tipo == BOOL && e.tipo == FLOAT) {

        } 
        return Expression(aType, aValor);
    }

    Expression& operator- (const Expression& e) {
        
    }
    Expression& operator* (const Expression& e) {
        
    }
    Expression& operator/ (const Expression& e) {
        
    }
    
    string get_string();
    char get_char();
    int get_int();
    float get_float();
    bool get_bool();
    */
private:
    Type tipo;
    void *valor;
};

int is_reserved(string);

Token get_token();

void get_first_token();

void get_next_token();

char get_next_char();

void unget_char(char);

bool prepare_file(string);

bool close_file();

void Error(string);

void Query();

bool Expre();

void Values();

void Where();

void Set();

/*
Expression Expre();

Expression SimpleExpre();

Expression Term();

Expression Factor();
*/

void Execute();

#endif // Interpreter
