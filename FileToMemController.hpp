//********************* FileToMemController.hpp *******************************
#ifndef FILE_TO_MEMORY_CONTROLLER_H
#define FILE_TO_MEMORY_CONTROLLER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "Tools.hpp"

using namespace std;

class Node{
public:
    Node(const string nome = "", void *info = nullptr){
        this->nome = nome;
        this->info = info;
    }
    ~Node(){
        this->nome = "";
        this->info = nullptr;
    }
    bool free(char);
    bool atribuir(fstream&, class Data, char);
    template<class T> Node& operator= (const Node& node){
        if(this->operator!=<T>(node)){
            this->nome = node.nome;
            *((T *)this->info) = *((T *) node.info);
        }
        return *this;
    }
    template<class T> bool operator== (const Node& node) const{
        return *((T *)this->info) == *((T *) node.info);
    }
    template<class T> bool operator!= (const Node& node) const{
        return *((T *)this->info) != *((T *) node.info);
    }
    template<class T> bool operator> (const Node& node) const{
        return *((T *)this->info) > *((T *) node.info);
    }
    template<class T> bool operator< (const Node& node) const{
        return *((T *)this->info) < *((T *) node.info);
    }
private:
    string nome;
    void *info;
    friend class MemRegister;
    friend class Register;
};

class MemRegister{
public:
    MemRegister(string s = ""): nome(s){}
    MemRegister(string nome, vector<string> v, string meta){
        this->nome = nome;
        for(int i=0; i<meta.size(); i++){
            void *ax = nullptr;
            switch (meta[i])
            {
                case INT:
                    ax = new int;
                    *cint(ax) = stoi(v[i], nullptr, 10);
                    break;
                case CHAR:
                    ax = new char;
                    *cchar(ax) = v[i][0];
                    break;
                case FLOAT:
                    ax = new float;
                    *cfloat(ax) = stof(v[i]);
                    break;
                case DOUBLE:
                    ax = new double;
                    *cdouble(ax) = stod(v[i]);
                    break;
                case STRING:
                    ax = new string;
                    *cstring(ax) = v[i];
                    break;
                default:
                    break;
            }
            Node n = Node("", ax);
            this->insert(n);
        }
    }
    ~MemRegister(){
        nome.clear();
        for(unsigned int i = 0; i < vars.size(); i++) vars[i].~Node();
    }
    bool free(string);
    bool insert(Node&);
    void print();
    static void *stringToVoid(const string&, void *&, char);
//private:
    string nome, meta;
    vector<Node> vars;
    friend class Register;
    friend class DataBase;
};
#endif // FILE_TO_MEMORY_CONTROLLER
