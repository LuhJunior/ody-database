//********************* FileToMemController.hpp *******************************
#ifndef FILE_TO_MEMORY_CONTROLLER
#define FILE_TO_MEMORY_CONTROLLER

#include "FileController.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

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
    ~MemRegister(){
        nome.clear();
        for(unsigned int i = 0; i < vars.size(); i++) vars[i].~Node();
    }
    bool free(string);
    bool insert(Node&);
    void print(string);
    static void *stringToVoid(const string&, void *&, char);
//private:
    string nome;
    vector<Node> vars;
    friend class Register;
    friend class DataBase;
};
#endif // FILE_TO_MEMORY_CONTROLLER
