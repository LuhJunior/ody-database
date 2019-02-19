//********************* FileToMemController.cpp *******************************
#include "FileToMemController.hpp"
#include "FileController.hpp"
bool Node::atribuir(fstream& f, Data d, char type){
    if(type == INT){
        info = new int;
        if(!d.getDataFromFile(f, cint(info))) return FileControl::Error("");
    }
    else if(type == FLOAT){
        info = new float;
        if(!d.getDataFromFile(f, cfloat(info))) return FileControl::Error("");
    }
    else if(type == DOUBLE){
        info = new double;
        if(!d.getDataFromFile(f, cdouble(info))) return FileControl::Error("");
    }
    else if(type == CHAR){
        info = new char;
        if(!d.getDataFromFile(f, cchar(info))) return FileControl::Error("");
    }
    else if(type == STRING){
        info = new string;
        if(!d.getDataFromFile(f, cstring(info))) return FileControl::Error("");
    }
    else{
        info = nullptr;
        return FileControl::Error("");
    }
    return !f.bad();
}

bool Node::free(char type){
    if(info != nullptr){
        if(type == INT) delete cint(info);
        else if(type == CHAR) delete cchar(info);
        else if(type == FLOAT) delete cfloat(info);
        else if(type == DOUBLE) delete cdouble(info);
        else if(type == STRING) delete cstring(info);
        else return FileControl::Error("");
    }
    return true;
}

void * MemRegister::stringToVoid(const string& s, void*& p, char type){
    if(type == INT){
        p = new int;
        *cint(p) = stoi(s);
    }
    else if(type == CHAR){
        p = new char;
        *cchar(p) = s[0];
    }
    else if(type == FLOAT){
        p = new float;
        *cfloat(p) = stof(s);
    }
    else if(type == DOUBLE){
        p = new double;
        *cdouble(p) = stod(s);
    }
    else if(type == STRING){
        p = new string;
        *cstring(p) = s;
    }
    else p = nullptr;
    return p;
}

bool MemRegister::free(string meta){
    for(unsigned int i = 0; i < vars.size(); i++) if(!vars[i].free(meta[i])) return FileControl::Error("");
    return true;
}

bool MemRegister::insert(Node& n){
    vars.push_back(n);
    return true;
}

void MemRegister::print(){
    string meta = this->meta;
    //cout("====== "<<nome<<" ======");
    for(unsigned int i=0; i<meta.size(); i++){
        if(meta[i] == INT) cout(vars[i].nome<<" : "<<(vars[i].info ? to_string(*cint(vars[i].info)) : ""));
        else if(meta[i] == FLOAT) cout(vars[i].nome<<" : "<<(vars[i].info ? to_string(*cfloat(vars[i].info)) : ""));
        else if(meta[i] == DOUBLE) cout(vars[i].nome<<" : "<<(vars[i].info ? to_string(*cdouble(vars[i].info)) : ""));
        else if(meta[i] == CHAR) cout(vars[i].nome<<" : "<<(vars[i].info ? to_string(*cchar(vars[i].info)) : ""));
        else if(meta[i] == STRING) cout(vars[i].nome<<" : "<<(vars[i].info ? *cstring(vars[i].info) : ""));
    }
}
