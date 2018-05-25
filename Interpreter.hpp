//****************************** Interpreter.h *************************************
#ifndef Interpreter
#define Interpreter

#include "FileController.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>

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

#endif // Interpreter
