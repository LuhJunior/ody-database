//********************************* FileController.h *************************************
#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include "FileToMemController.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#define cout(x) cout<<x<<endl
#define cerr(x) cerr<<x<<endl
#define cint(x) (int *) x
#define cchar(x) (char *) x
#define cstring(x) (string *) x
#define cfloat(x) (float *) x
#define cdouble(x) (double *) x
#define cvoid(x) (void *) x
#define INT 'i'
#define FLOAT 'f'
#define DOUBLE 'd'
#define CHAR 'c'
#define STRING 's'
#define BIT_PROTETOR '�'
#define ENTRADA 'I'
#define SAIDA 'O'
#define ENTRADA_SAIDA '�'

using namespace std;


class Segment{
public:
    Segment(int base = 0, int limit = 0){
        this->base = base;
        this->limit = limit;
    }
    ~Segment(){
        this->base = 0;
        this->limit = 0;
    }
    bool isValid();
    int size();
    unsigned int getBase();
    unsigned int getLimit();
    Segment& operator= (const Segment& s){
        if(*this != s){
            this->base = s.base;
            this->limit = s.limit;
        }
        return *this;
    }
    bool operator== (const Segment& s) const{
        return (this->base == s.base && this->limit == s.limit);
    }
    bool operator!= (const Segment& s) const{
        return !(*this == s);
    }
    bool operator< (const Segment& s) const{
        return (this->limit < s.limit);
    }
    bool operator> (const Segment& s) const{
        return (this->limit > s.limit);
    }
private:
    friend class Data;
    friend class FileControl;
    friend class Register;
    unsigned int base, limit;
};

class Data{
public:
    Data(Segment pos = Segment(), Segment next = Segment()){
        this->pos = pos;
        this->next = next;
    }
    ~Data(){
        pos.~Segment();
        next.~Segment();
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool toNext(fstream&);
    template<class T> bool getDataFromFile(fstream&, T*);
    template<class T> bool setDataToFile(fstream&, T*);
    bool getDataFromFile(fstream&, string*);
    string getDataFromFile(fstream&, char);
    bool setDataToFile(fstream&, string, char);
    Data& operator= (const Data& d){
        if(*this != d){
            this->pos = d.pos;
            this->next = d.next;
        }
        return *this;
    }
    bool operator== (const Data& d) const{
        return (this->pos == d.pos && this->next == d.next);
    }
    bool operator!= (const Data& d) const{
        return !(*this == d);
    }
    bool operator< (const Data& d) const{
        return this->pos < d.pos;
    }
    bool operator> (const Data& d) const{
        return this->pos > d.pos;
    }
private:
    Segment pos;
    Segment next;
    friend class DataBase;
    friend class TableMetaDado;
    friend class Table;
    friend class Register;
};

class HeadRegister{
public:
    HeadRegister(Segment fistRegister = Segment()){
        this->fistRegister = fistRegister;
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool operator== (const HeadRegister& HR) const{
        return(this->fistRegister == HR.fistRegister);
    }
    bool operator!= (const HeadRegister& HR) const{
        return !(*this == HR);
    }
    HeadRegister& operator= (const HeadRegister& HR){
        if(*this != HR) this->fistRegister = HR.fistRegister;
        return *this;
    }
private:
    Segment fistRegister;
    friend class DataBase;
};

class HeadData{
public:
    HeadData(Segment firstData = Segment()){
        this->firstData = firstData;
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool operator== (const HeadData& HD) const{
        return (this->firstData == HD.firstData);
    }
    bool operator!= (const HeadData& HD) const{
        return !(*this == HD);
    }
    HeadData& operator= (const HeadData& HD){
        if(*this != HD){
            this->firstData = HD.firstData;
        }
        return *this;
    }
private:
    Segment firstData;
    friend class DataBase;
};

class Register{
public:
    Register(Segment data = Segment(), Segment next = Segment()){
        this->data = data;
        this->next = next;
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool updateVar(fstream&, class DataBase&, string, int, string);
    bool setMemRegisterToFile(fstream&, vector<string>&, class MemRegister&);
    string getVarFromFile(fstream&, string, int);
    bool freeRegisterSeg(fstream&, class DataBase&, Segment);
    MemRegister registerToMemRegister(fstream&, vector<string>, string);
    bool operator== (const Register& r) const{
        return (this->data == r.data && this->next == r.next);
    }
    bool operator!= (const Register& r) const{
        return !(*this == r);
    }
    Register& operator= (const Register& r){
        if(*this != r){
            this->data = r.data;
            this->next = r.next;
        }
        return *this;
    }
private:
    Segment data, next;
    friend class DataBase;
};

class TableMetaDado{
public:
    TableMetaDado(Segment nome = Segment(), Segment tipo = Segment(), Segment varNames = Segment()){
        nome = nome;
        tipo = tipo;
        this->varNames = varNames;
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool setNomeToFile(fstream&,class DataBase&, string);
    bool setTipoToFile(fstream&,class DataBase&, string);
    bool setVarNameToFile(fstream&, string);
    bool getNomeFromFile(fstream&, string&);
    bool getTipoFromFile(fstream&, string&);
    vector<string> metaToString(fstream&);
    bool stringToMeta(fstream&, vector<string>&);
    bool operator== (const TableMetaDado& TMD) const{
        return (this->nome == TMD.nome && this->tipo == TMD.tipo && this->varNames == TMD.varNames);
    }
    bool operator!= (const TableMetaDado& TMD) const{
        return !(*this == TMD);
    }
    TableMetaDado& operator= (const TableMetaDado& TMD){
        if(*this != TMD){
            this->nome = TMD.nome;
            this->tipo = TMD.tipo;
            this->varNames = TMD.varNames;
        }
        return *this;
    }
private:
    Segment nome, tipo, varNames;
    friend class DataBase;
};

class Table{
public:
    Table(Segment HeadRegister = Segment()){
        this->idAtual = 0;
        this->headRegister = HeadRegister;
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    //bool getHeadRegisterFromFile(fstream&, Register& R);
    bool operator== (const Table& t) const{
        return (this->idAtual == t.idAtual && this->headRegister == t.headRegister);
    }
    bool operator!= (const Table& t) const{
        return !(*this == t);
    }
    Table& operator= (const Table& t){
        if(*this != t){
            this->idAtual = t.idAtual;
            this->headRegister = t.headRegister;
        }
        return *this;
    }
private:
    int idAtual;
    Segment headRegister;
    //Segment BtreeArea;
    friend class DataBase;
};

class TableHeader{
public:
    TableHeader(int numeroColunas = 0, int numeroRegistros = 0, Segment tablePosition = Segment(0, 0),
                Segment metaTablePosition = Segment(0, 0), Segment next = Segment(0, 0)){
        this->numeroColunas = numeroColunas;
        this->numeroRegistros = numeroRegistros;
        this->tablePosition = tablePosition;
        this->metaTablePosition = metaTablePosition;
        this->next = next;
    }
    bool getTableFromFile(fstream&, Table&);
    bool getTableMetaDado(fstream&, TableMetaDado&);
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    vector<string> getTableMeta();
    bool operator== (const TableHeader& th) const{
        return (this->numeroColunas == th.numeroColunas && this->numeroRegistros == th.numeroRegistros
                && this->tablePosition == th.tablePosition && this->metaTablePosition == th.metaTablePosition
                && this->next == th.next);
    }
    bool operator!= (const TableHeader& th) const{
        return !(*this == th);
    }
    TableHeader& operator= (const TableHeader& th){
        if(*this != th){
            this->numeroColunas = th.numeroColunas;
            this->numeroRegistros = th.numeroRegistros;
            this->tablePosition = th.tablePosition;
            this->metaTablePosition = th.metaTablePosition;
            this->next = th.next;
        }
        return *this;
    }
private:
    int numeroColunas, numeroRegistros;
    Segment tablePosition, metaTablePosition, next;
    friend class DataBase;
};

class HeadTableHeader{
public:
    HeadTableHeader(Segment firstTableHeader = Segment()){
        this->firstTableHeader = firstTableHeader;
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool operator== (const HeadTableHeader& HTH) const{
        return this->firstTableHeader == HTH.firstTableHeader;
    }
    bool operator!= (const HeadTableHeader& HTH) const{
        return !(*this == HTH);
    }
    HeadTableHeader& operator= (const HeadTableHeader& HTH){
        if(*this != HTH){
            this->firstTableHeader = HTH.firstTableHeader;
        }
        return *this;
    }
private:
    Segment firstTableHeader;
    friend class DataBase;
};

class DataBaseHeader{
public:
    DataBaseHeader(int numeroTabelas = 0, Segment headTableHeader = Segment(), Segment freeSegments = Segment()){
        this->numeroTabelas = numeroTabelas;
        this->headTableHeader = headTableHeader;
        this->freeSegments = freeSegments;
    }
    ~DataBaseHeader(){
        this->numeroTabelas = 0;
        this->headTableHeader.~Segment();
        this->freeSegments.~Segment();
    }
    int size();
    bool getFromFile(fstream&, Segment);
    bool setToFile(fstream&, Segment);
    bool setToFile(fstream&);
    bool operator== (const DataBaseHeader& DBH) const{
        return (this->numeroTabelas == DBH.numeroTabelas && this->headTableHeader == DBH.headTableHeader
                && this->freeSegments == DBH.freeSegments);
    }
    bool operator!= (const DataBaseHeader& DBH) const{
        return !(*this == DBH);
    }
    DataBaseHeader& operator= (const DataBaseHeader& DBH){
        if(*this != DBH){
            this->numeroTabelas = DBH.numeroTabelas;
            this->headTableHeader = DBH.headTableHeader;
            this->freeSegments = DBH.freeSegments;
        }
        return *this;
    }
private:
    int numeroTabelas;
    Segment headTableHeader, freeSegments;
    friend class DataBase;
};

class DataBase{
public:
    DataBase(string name = "", DataBaseHeader DBH = DataBaseHeader()){
        this->name = name+".mydb";
        this->Header = DBH;
    }
    ~DataBase(){
        this->name = "";
        this->Header.~DataBaseHeader();
    }
    bool open();
    bool open(fstream&);
    static bool create(string);
    bool insertTable(string);
    bool insertColumn(string, string, char);
    bool insertRegister(MemRegister&);
    bool updateRegister(string[4], string);
    bool deleteRegister(string[3]);
    bool freeSegment(fstream&, Segment);
    vector<MemRegister> getRegister(string[3], string&);
    Segment searchTable(string);
    Segment searchTable(HeadTableHeader&, TableHeader&, TableMetaDado&, string);
    void close();
    bool operator== (const DataBase& DB) const{
        return (this->name == DB.name && this->Header == DB.Header);
    }
    bool operator!= (const DataBase& DB) const{
        return !(*this == DB);
    }
    DataBase& operator= (const DataBase& DB){
        if(*this != DB){
            this->name = DB.name;
            this->Header = DB.Header;
        }
        return *this;
    }
private:
    string name;
    DataBaseHeader Header;
};

class FileControl{
public:
    static bool setBlock(fstream&, char *, Segment);
    static bool setBlockError();
    static bool setBlockError(fstream&);
    static bool getBlock(fstream&, char *, Segment);
    static bool getBlockError();
    static bool getBlockError(fstream&);
    static bool openFile(fstream&, string, char);
    static bool openFileError();
    static bool openFileError(fstream&);
    static int size(char);
    static Segment discAlloc(fstream&, int);
    static bool discAllocError();
    static bool discAllocError(fstream&);
    static bool Error(string);
    static bool Error(string, fstream&);
    template<class T> static T Error(string s, T);
    template<class T> static T Error(string, T, fstream&);
};

void toUper(string&);
#endif // FILE_CONTROLLER
