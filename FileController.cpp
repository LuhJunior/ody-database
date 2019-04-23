//********************************* FileController.cpp *************************************
#include "FileController.hpp"
#include "FileToMemController.hpp"

void toUper(string& s){
    for(unsigned int i=0; i<s.size(); i++) s[i] = toupper(s[i]);
}

bool Segment::isValid(){
    return(this->base != 0 || this->limit != 0);
}

int Segment::size(){
    return(sizeof(*this));
}

unsigned int Segment::getBase(){
    return this->base;
}

unsigned int Segment::getLimit(){
    return this->limit;
}

int Data::size(){
    return(sizeof(*this));
}

bool Data::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool Data::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

bool Data::toNext(fstream& f){
    return FileControl::getBlock(f, cchar(this), this->next);
}

bool Data::getDataFromFile(fstream& f, string *data){
    data->resize(this->pos.getLimit());
    return FileControl::getBlock(f, cchar(data->c_str()), this->pos);
}

template<class T>
bool Data::getDataFromFile(fstream& f, T *data){
    return FileControl::getBlock(f, cchar(data), this->pos);
}

template<>
bool Data::getDataFromFile<char>(fstream& f, char *data){
    return FileControl::getBlock(f, data, this->pos);
}

string Data::getDataFromFile(fstream& f, char type){
    if(type == INT){
        int ax = 0;
        getDataFromFile(f, &ax);
        return to_string(ax);
    }
    else if(type == FLOAT){
        float ax = 0;
        getDataFromFile(f, &ax);
        return to_string(ax);
    }
    else if(type == DOUBLE){
        double ax = 0;
        getDataFromFile(f, &ax);
        return to_string(ax);
    }
    else if(type == CHAR || type == STRING){
        string ax = "";
        getDataFromFile(f, &ax);
        return ax;
    }
    else return "";
}

template<class T>
bool Data::setDataToFile(fstream& f, T *data){
    return FileControl::setBlock(f, cchar(data), this->pos);
}

template<>
bool Data::setDataToFile<string>(fstream& f, string *data){
    return FileControl::setBlock(f, cchar(data->c_str()), this->pos);
}

int HeadTableHeader::size(){
    return sizeof(*this);
}

bool HeadTableHeader::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool HeadTableHeader::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

int Table::size(){
    return sizeof(*this);
}

bool Table::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool Table::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

int TableHeader::size(){
    return sizeof(*this);
}

bool TableHeader::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool TableHeader::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

bool TableHeader::getTableFromFile(fstream& f, Table& t){
    return FileControl::getBlock(f, cchar(&t), this->tablePosition);
}

bool TableHeader::getTableMetaDado(fstream&f, TableMetaDado& tmd){
    return FileControl::getBlock(f, cchar(&tmd), this->metaTablePosition);
}

string TableHeader::getTableMeta(DataBase &db){
    TableMetaDado tmd;
    fstream f;
    db.open(f);
    this->getTableMetaDado(f, tmd);
    return tmd.metaToString(f)[0];
}

int TableMetaDado::size(){
    return sizeof(*this);
}

bool TableMetaDado::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool TableMetaDado::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

bool TableMetaDado::setNomeToFile(fstream& f, DataBase& db, string s){
    if(this->nome.getLimit() != s.size()){
        if(!db.freeSegment(f, this->nome)) return FileControl::Error("");
        this->nome = FileControl::discAlloc(f, s.size());
        if(!this->nome.isValid()) return FileControl::Error("FileControl::discAlloc");
    }
    if(!FileControl::setBlock(f, cchar(s.c_str()), this->nome)) return FileControl::Error("FileControl::setBlock");
    return true;
}

bool TableMetaDado::setTipoToFile(fstream& f, DataBase& db, string s){
    if(this->tipo.getLimit() != s.size()){
        if(!db.freeSegment(f, this->tipo)) return FileControl::Error("DataBase::freeSegment");
        this->tipo = FileControl::discAlloc(f, s.size());
        if(!this->tipo.isValid()) return FileControl::Error("FileControl::discAlloc");
    }
    if(!FileControl::setBlock(f, cchar(s.c_str()), this->tipo)) return FileControl::Error("FileControl::setBlock");
    return true;
}

bool TableMetaDado::getNomeFromFile(fstream& f, string& s){
    s.resize(this->nome.getLimit());
    return FileControl::getBlock(f, cchar(s.c_str()), this->nome);
}

bool TableMetaDado::getTipoFromFile(fstream& f, string& s){
    s.resize(this->tipo.getLimit());
    return FileControl::getBlock(f, cchar(s.c_str()), this->tipo);
}

bool TableMetaDado::stringToMeta(fstream& f, vector<string>& tableInfo){
    return !f.bad();
}

bool TableMetaDado::setVarNameToFile(fstream& f, string columnName){
    Segment s;
    Data d, ax;
    if(!this->varNames.isValid()){
        this->varNames = FileControl::discAlloc(f, d.size());
        if(!this->varNames.isValid()) return FileControl::discAllocError();
        d.pos = FileControl::discAlloc(f, columnName.size());
        if(!d.pos.isValid()) return FileControl::discAllocError();
        if(!d.setToFile(f, this->varNames)) return FileControl::Error("");
        if(!d.setDataToFile<string>(f, &columnName)) return FileControl::Error("");
        return !f.bad();
    }
    else if(!ax.getFromFile(f, this->varNames)) return FileControl::Error("");
    s = this->varNames;
    while(ax.next.isValid()) {
        s = ax.next;
        if(!ax.toNext(f)) return FileControl::Error("");
    }
    ax.next = FileControl::discAlloc(f, d.size());
    if(!ax.next.isValid()) return FileControl::discAllocError();
    if(!ax.setToFile(f, s)) return FileControl::Error("");
    d.pos = FileControl::discAlloc(f, columnName.size());
    if(!d.pos.isValid()) return FileControl::discAllocError();
    if(!d.setToFile(f, ax.next)) return FileControl::Error("");
    if(!d.setDataToFile<string>(f, &columnName)) return FileControl::Error("");
    return !f.bad();
}

vector<string> TableMetaDado::metaToString(fstream& f){
    vector<string> v;
    v.resize(this->tipo.getLimit()+1);
    v[0].resize(this->tipo.getLimit());
    if(!FileControl::getBlock(f, cchar(v[0].c_str()), this->tipo)) return FileControl::Error("FileControl::getBlock", vector<string>());
    Data d;
    if(this->varNames.isValid()) if(!d.getFromFile(f, this->varNames)) return FileControl::Error("Data::getFromFile", vector<string>());
    if(!d.getDataFromFile(f, &v[1])) return FileControl::Error("Data::getDataFromFile", vector<string>());
    for(unsigned int i=2; i <= this->tipo.getLimit()-1; i++){
        if(!d.toNext(f)) return FileControl::Error("Data::toNext", vector<string>(), f);
        if(!d.getDataFromFile(f, &v[i])) return FileControl::Error("Data::getDataFromFile", vector<string>(), f);
    }
    return v;
}

int HeadRegister::size(){
    return sizeof(*this);
}

bool HeadRegister::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool HeadRegister::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

int HeadData::size(){
    return sizeof(*this);
}

bool HeadData::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool HeadData::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

int Register::size(){
    return sizeof(*this);
}

bool Register::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool Register::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

bool Register::updateVar(fstream& f, DataBase& db, string meta, int var, string valor){
    Data d;
    if(!d.getFromFile(f, data)) return FileControl::Error("");
    for(int i=1; i<var; i++) if(!d.toNext(f)) return FileControl::Error("");
    if(meta[var-1] == STRING){
        if(d.pos.limit != valor.size()){
            if(!db.freeSegment(f, d.pos)) return FileControl::Error("");
            d.pos = FileControl::discAlloc(f, valor.size());
            if(!d.pos.isValid()) return FileControl::Error("");
            if(!d.setToFile(f, data)) return FileControl::Error("");
        }
    }
    else{
        if(!d.pos.isValid()){
            d.pos = FileControl::discAlloc(f, FileControl::size(meta[var-1]));
            if(!d.pos.isValid()) return FileControl::Error("");
            if(!d.setToFile(f, data)) return FileControl::Error("");
        }
    }

    if(!d.setDataToFile(f, &valor)) return FileControl::Error("");
    return !f.bad();
}

string Register::getVarFromFile(fstream& f, string meta, int var){
    Data d;
    if(!d.getFromFile(f, data)) return FileControl::Error("", "");
    for(int i = 1; i < var; i++) if(!d.toNext(f)) return FileControl::Error("", "");
    return d.getDataFromFile(f, meta[var-1]);
}

bool Register::setMemRegisterToFile(fstream& f, vector<string>& v, MemRegister& mr){
    Data d;
    this->data = FileControl::discAlloc(f, d.size());
    if(!this->data.isValid()) return FileControl::Error("");
    Segment s = this->data;
    if(mr.vars[0].nome.empty()){
        for(unsigned int i=0; i<v[0].size(); i++){
            mr.vars[i].nome = v[i+1];
        }
    }
    for(unsigned int i=0; i<mr.vars.size(); i++){
        for(unsigned int j=1; j<=v[0].size(); j++){
            if(mr.vars[i].nome == v[j]){
                if(v[0][j-1] != 's') {
                    d.pos = FileControl::discAlloc(f, FileControl::size(v[0][j-1]));
                    if(!d.pos.isValid()) return FileControl::Error("");
                    if(!d.setDataToFile(f, mr.vars[i].info))return FileControl::Error("");
                }
                else{
                    d.pos = FileControl::discAlloc(f, (int)(cstring(mr.vars[i].info))->size());
                    if(!d.pos.isValid()) return FileControl::Error("");
                    if(!d.setDataToFile<string>(f, cstring(mr.vars[i].info))) return FileControl::Error("");
                }
                break;
            }
            else{
                d.pos = Segment(0, 0);
            }
        }
        if(i != mr.vars.size()-1){
            d.next = FileControl::discAlloc(f, d.size());
            if(!d.next.isValid()) return FileControl::Error("");
        }
        else{
            d.next = Segment(0, 0);
        }
        if(!d.setToFile(f, s)) return FileControl::Error("");
        s = d.next;
    }
    return !f.bad();
}

bool Register::freeRegisterSeg(fstream& f, DataBase& db, Segment pos){
    Data d;
    d.getFromFile(f, data);
    do{
        if(!db.freeSegment(f, d.pos)) return FileControl::Error("DataBase::freeSegment");
        if(!db.freeSegment(f, d.next)) return FileControl::Error("DataBase::freeSegment");

    }while(d.next.isValid() && d.toNext(f));
    if(!db.freeSegment(f, data)) return FileControl::Error("DataBase::freeSegment");
    if(!db.freeSegment(f, next)) return FileControl::Error("DataBase::freeSegment");
    if(!db.freeSegment(f, pos)) return FileControl::Error("DataBase::freeSegment");
    return !f.bad();
}

MemRegister Register::registerToMemRegister(fstream& f, vector<string> meta, string name){
    Data d;
    Node n;
    MemRegister mr(name);
    if(!d.getFromFile(f, data)) return FileControl::Error("", MemRegister());
    n.nome = meta[1];
    if(!n.atribuir(f, d, meta[0][0])){
        n.free(meta[0][0]);
        return FileControl::Error("", MemRegister());
    }
    mr.vars.push_back(n);
    for(unsigned int i = 2; i < meta.size(); i++){
        if(!d.toNext(f)){
            n.free(meta[0][i-1]);
            mr.free(meta[0]);
            return FileControl::Error("", MemRegister());
        }
        n.nome = meta[i];
        if(!n.atribuir(f, d, meta[0][i-1])){
            n.free(meta[0][i-1]);
            mr.free(meta[0]);
            return FileControl::Error("", MemRegister());
        }
        mr.vars.push_back(n);
    }
    return mr;
}

int DataBaseHeader::size(){
    return sizeof(*this);
}

bool DataBaseHeader::getFromFile(fstream& f, Segment s){
    return FileControl::getBlock(f, cchar(this), s);
}

bool DataBaseHeader::setToFile(fstream& f, Segment s){
    return FileControl::setBlock(f, cchar(this), s);
}

bool DataBaseHeader::setToFile(fstream& f){
    return FileControl::setBlock(f, cchar(this), Segment(1, this->size()));
}

bool DataBase::create(string name){
    fstream file;
    HeadTableHeader hth;
    DataBase db(name, DataBaseHeader());
    char bit = BIT_PROTETOR;
    if(FileControl::openFile(file, db.name, ENTRADA)){
        cout("Esse banco ja existe se continuar ira apagar o ja existente deseja continuar? (Sim/Nao)");
        string ax;
        do{
            cin>>ax;
            toUper(ax);
            if(ax == "NAO") {
                file.close();
                exit(1);
            }
        }while(ax != "SIM");
        file.close();
    }
    if(!FileControl::openFile(file, db.name, SAIDA)) return FileControl::Error("FileControl::openFile");
    if(!FileControl::setBlock(file, &bit, FileControl::discAlloc(file, 1))) return FileControl::Error("FileControl::setBlock");
    if(!FileControl::discAlloc(file, db.Header.size()).isValid()) return FileControl::Error("FileControl::discAlloc", file);
    db.Header.headTableHeader = FileControl::discAlloc(file, hth.size());
    if(!db.Header.headTableHeader.isValid()) return FileControl::Error("FileControl::discAlloc", file);
    if(!db.Header.setToFile(file, Segment(1, db.Header.size()))) return FileControl::Error("DataBaseHeader::setToFile");
    if(!hth.setToFile(file, db.Header.headTableHeader)) return FileControl::Error("HeadTableHeader::setToFile");
    db.close();
    return !file.bad();
}

bool DataBase::open(){
    fstream file;
    char bit;
    if(!FileControl::openFile(file, this->name, ENTRADA_SAIDA)) return FileControl::Error("FileControl::openFile");
    if(!FileControl::getBlock(file, &bit, Segment(0, 1))) return FileControl::Error("FileControl::getBlock");
    if(bit != BIT_PROTETOR) return FileControl::Error("este arquivo nao foi reconhecido");
    if(!this->Header.getFromFile(file, Segment(1, this->Header.size()))) return FileControl::Error("DataBaseHeader::getFromFile");
    return !file.bad();
}

bool DataBase::open(fstream& file){
    char bit;
    if(!FileControl::openFile(file, this->name, ENTRADA_SAIDA)) return FileControl::Error("FileControl::openFile");
    if(!FileControl::getBlock(file, &bit, Segment(0, 1))) return FileControl::Error("FileControl::getBlock");
    if(bit != BIT_PROTETOR) return FileControl::Error("este arquivo nao foi reconhecido");
    return !file.bad();
}

bool DataBase::isOpen(){
    return this->name != "";
}

void DataBase::close(){
    this->~DataBase();
}

bool DataBase::freeSegment(fstream& f, Segment s){
    if(s.isValid()){
        Data d;
        d.pos = s;
        d.next = this->Header.freeSegments;
        this->Header.freeSegments = FileControl::discAlloc(f, d.size());
        if(!this->Header.freeSegments.isValid()) return FileControl::Error("FileControl::discAlloc");
        if(!d.setToFile(f, this->Header.freeSegments)) return FileControl::Error("Data::setToFile");
        if(!this->Header.setToFile(f)) return FileControl::Error("DataBaseHeader::setToFile");
    }
    return true;
}

bool DataBase::insertTable(string tableName){
    HeadTableHeader hth;
    TableHeader th;
    TableMetaDado tmd;
    Table t;
    HeadRegister hr;
    fstream file;
    if(!this->open(file)) return FileControl::Error("DataBase::open", file);
    if(!hth.getFromFile(file, this->Header.headTableHeader)) return FileControl::Error("HeadTableHeader::getFromFile", file);
    this->Header.numeroTabelas++;
    th.next = hth.firstTableHeader;
    //Alocando os espa�os no disco
    hth.firstTableHeader = FileControl::discAlloc(file, th.size());
    if(!hth.firstTableHeader.isValid()) return FileControl::discAllocError(file);
    th.metaTablePosition = FileControl::discAlloc(file, tmd.size());
    if(!th.metaTablePosition.isValid()) return FileControl::discAllocError(file);
    tmd.nome = FileControl::discAlloc(file, tableName.size());
    if(!tmd.nome.isValid()) return FileControl::discAllocError(file);
    th.tablePosition = FileControl::discAlloc(file, t.size());
    if(!th.tablePosition.isValid()) return FileControl::discAllocError(file);
    t.headRegister = FileControl::discAlloc(file, hr.size());
    if(!t.headRegister.isValid()) return FileControl::discAllocError(file);
    //gravando nos espa�os alocados
    if(!this->Header.setToFile(file, Segment(1, this->Header.size()))) return FileControl::Error("TableHeader::setToFile", file);
    if(!hth.setToFile(file, this->Header.headTableHeader)) return FileControl::Error("", file);
    if(!th.setToFile(file, hth.firstTableHeader)) return FileControl::Error("", file);
    if(!tmd.setToFile(file, th.metaTablePosition)) return FileControl::Error("", file);
    if(!t.setToFile(file, th.tablePosition)) return FileControl::Error("", file);
    if(!hr.setToFile(file, t.headRegister)) return FileControl::Error("", file);
    if(!tmd.setNomeToFile(file, *this, tableName)) return FileControl::Error("", file);
    file.close();
    return !file.bad();
}

Segment DataBase::searchTable(string tableName){
    HeadTableHeader hth;
    TableHeader th;
    TableMetaDado tmd;
    fstream file;
    if(!this->open(file)) return FileControl::Error("", Segment(0, 0), file);
    if(!hth.getFromFile(file, this->Header.headTableHeader)) return FileControl::Error("", Segment(0, 0), file);
    if(!th.getFromFile(file, hth.firstTableHeader)) return FileControl::Error("", Segment(0, 0), file);
    if(!tmd.getFromFile(file, th.metaTablePosition)) return FileControl::Error("", Segment(0, 0), file);
    string ax;
    if(!tmd.getNomeFromFile(file, ax)) return FileControl::Error("", Segment(0, 0), file);
    if(ax == tableName){
        file.close();
        return hth.firstTableHeader;
    }
    for(int i=0; i<this->Header.numeroTabelas-1; i++){
        Segment s = th.next;
        if(!th.getFromFile(file, th.next)) return FileControl::Error("", Segment(0, 0), file);
        if(!tmd.getFromFile(file, th.metaTablePosition)) return FileControl::Error("", Segment(0, 0), file);
        if(!tmd.getNomeFromFile(file, ax)) return FileControl::Error("", Segment(0, 0), file);
        if(ax == tableName) {
            file.close();
            return s;
        }
    }
    file.close();
    return Segment(0, 0);
}

Segment DataBase::searchTable(HeadTableHeader& hth, TableHeader& th, TableMetaDado& tmd, string tableName){
    fstream file;
    if(!this->open(file)) return FileControl::Error("", Segment(0, 0), file);
    if(!hth.getFromFile(file, this->Header.headTableHeader)) return FileControl::Error("", Segment(0, 0), file);
    if(!th.getFromFile(file, hth.firstTableHeader)) return FileControl::Error("", Segment(0, 0), file);
    if(!tmd.getFromFile(file, th.metaTablePosition)) return FileControl::Error("", Segment(0, 0), file);
    string ax;
    if(!tmd.getNomeFromFile(file, ax)) return FileControl::Error("", Segment(0, 0), file);
    if(ax == tableName){
        file.close();
        return hth.firstTableHeader;
    }
    for(int i=0; i<this->Header.numeroTabelas-1; i++){
        Segment s = th.next;
        if(!th.getFromFile(file, th.next)) return FileControl::Error("", Segment(0, 0), file);
        if(!tmd.getFromFile(file, th.metaTablePosition)) return FileControl::Error("", Segment(0, 0), file);
        if(!tmd.getNomeFromFile(file, ax)) return FileControl::Error("", Segment(0, 0), file);
        if(ax == tableName) {
            file.close();
            return s;
        }
    }
    file.close();
    return Segment(0, 0);
}

TableHeader DataBase::getTableHeader(string table){
    TableHeader th;
    fstream file;
    if(!this->open(file)) FileControl::Error("");
    th.getFromFile(file, this->searchTable(table));
    return th;
}

bool DataBase::insertColumn(string tableName, string columnName, char varType){
    HeadTableHeader hth;
    TableHeader th;
    TableMetaDado tmd;
    Segment s;
    fstream file;
    s = this->searchTable(hth, th, tmd, tableName);
    if(!s.isValid()) return FileControl::Error("A tabela nao foi encontrada");
    if(!this->open(file)) return FileControl::Error("DataBase::open", false, file);
    string ax;
    if(tmd.tipo.isValid()) tmd.getTipoFromFile(file, ax);
    ax.push_back(varType);
    if(!tmd.setTipoToFile(file, *this, ax)) return FileControl::Error("TableMetaDado::setToFile", false, file);
    if(!tmd.setVarNameToFile(file, columnName)) return FileControl::Error("TableMetaDado::setToFile", false, file);
    if(!tmd.setToFile(file, th.metaTablePosition)) return FileControl::Error("TableMetaDado::setToFile", false, file);
    file.close();
    return !file.bad();
}

bool DataBase::insertRegister(MemRegister& mr){
    HeadTableHeader hth;
    TableHeader th;
    TableMetaDado tmd;
    Table t;
    HeadRegister hr;
    Register r;
    Segment s;
    fstream file;
    if(!this->open(file)) return FileControl::Error("DataBase::open", false, file);
    if(!this->searchTable(hth, th, tmd, mr.nome).isValid()) return FileControl::Error("A tabela nao foi encontrada");
    vector<string> v = tmd.metaToString(file);
    if(v.empty()) return FileControl::Error("TableMetaDado::metaToString", file);
    if(!t.getFromFile(file, th.tablePosition)) return FileControl::Error("Table::getFromFile", file);
    if(!hr.getFromFile(file, t.headRegister)) return FileControl::Error("HeadRegister::getFromFile", file);
    r.next = hr.fistRegister;
    hr.fistRegister = FileControl::discAlloc(file, r.size());
    if(!hr.fistRegister.isValid()) return FileControl::discAllocError(file);
    if(!r.setMemRegisterToFile(file, v, mr)) return FileControl::Error("Register::setMemRegisterToFile", file);
    if(!r.setToFile(file, hr.fistRegister)) return FileControl::Error("Register::setToFile", file);
    if(!hr.setToFile(file, t.headRegister)) return FileControl::Error("HeadRegister::setToFile", file);
    file.close();
    return !file.bad();
}

bool DataBase::updateRegister(vector<string>  controle, string valor){
    /*
        0 ~ nome da tabela
        1 ~ ID, Coluna ou * where clause
        2 ~ caso 1, coluna tem q ser igual isso aqui
        3 ~ coluna a ser atualizada
    */
    HeadTableHeader hth;
    TableHeader th;
    TableMetaDado tmd;
    Table t;
    HeadRegister hr;
    Register r;
    fstream file;
    if(!searchTable(hth, th, tmd, controle[0]).isValid()) return FileControl::Error("A tabela nao foi encontrada");
    if(!open(file)) return FileControl::Error("DataBase::open", file);
    vector<string> v = tmd.metaToString(file);
    unsigned int coluna = 0;
    for(coluna = 1; coluna < v.size(); coluna++) if(controle[3] == v[coluna]) break;
    if(coluna == v.size()) return FileControl::Error("Coluna nao foi encontrada", file);
    if(v.empty()) return FileControl::Error("TableMetaDado::metaToString", file);
    if(!t.getFromFile(file, th.tablePosition)) return FileControl::Error("", file);
    if(!hr.getFromFile(file, t.headRegister)) return FileControl::Error("", file);
    if(!r.getFromFile(file, hr.fistRegister)) return FileControl::Error("", file);
    /*if(controle[1] == "ID"){
        //int id = stoi(controle[2]);
        for(int i = 1; i < stoi(controle[2]); i++) if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
        if(!r.updateVar(file, *this, v[0], coluna, valor)) return FileControl::Error("", file);
    }
    else */if(!controle[2].empty()){
        unsigned int coluna2 = 0;
        for(coluna2 = 1; coluna2 < v.size(); coluna2++) if(controle[1] == v[coluna2]) break;
        if(coluna2 == v.size()) return FileControl::Error("Coluna nao foi encontrada", file);
        string ax = r.getVarFromFile(file, v[0], coluna2);
        if(ax.empty()) return FileControl::Error("Register::getVarFromFile", file);
        if(ax == controle[2]) if(!r.updateVar(file, *this, v[0], coluna, valor)) return FileControl::Error("", file);
        for(int i = 1; i < th.numeroRegistros; i++){
              if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
              ax = r.getVarFromFile(file, v[0], coluna2);
              if(ax.empty()) return FileControl::Error("Register::getVarFromFile", file);
              if(ax == controle[2]) if(!r.updateVar(file, *this, v[0], coluna, valor)) return FileControl::Error("", file);
        }
    }
    else{
        if(!r.updateVar(file, *this, v[0], coluna, valor)) return FileControl::Error("", file);
        for(int i = 1; i < th.numeroRegistros; i++) {
            if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
            if(!r.updateVar(file, *this, v[0], coluna, valor)) return FileControl::Error("", file);
        }
    }
    file.close();
    return !file.bad();
}

bool DataBase::deleteRegister(vector<string> controle){
    /*
        *controle[0] ~ nome da tabela
        *controle[1] ~ ID ou coluna que tem que ser igual a controle[3] ou * deletar tudo
    */
    HeadTableHeader hth;
    TableHeader th;
    Table t;
    TableMetaDado tmd;
    HeadRegister hr;
    Register r;
    Segment thSeg;
    fstream file;
    thSeg = searchTable(hth, th, tmd, controle[0]);
    if(!thSeg.isValid()) return FileControl::Error("A tabela nao foi encontrada");
    if(!open(file)) return FileControl::Error("DataBase::open", file);
    if(!t.getFromFile(file, th.tablePosition)) return FileControl::Error("", file);
    if(!hr.getFromFile(file, t.headRegister)) return FileControl::Error("", file);
    if(!r.getFromFile(file, hr.fistRegister)) return FileControl::Error("", file);
    if(controle[0] == "ID"){
        Register ax;
        Segment seg = hr.fistRegister;
        if(stoi(controle[2]) > 1) if(!ax.getFromFile(file, r.next)) return FileControl::Error("", file);
        for(int i = 2; i < stoi(controle[2]); i++){
            seg = r.next;
            if(!ax.getFromFile(file, ax.next)) return FileControl::Error("", file);
            if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
        }
        if(stoi(controle[2]) == 1){
            if(!r.freeRegisterSeg(file, *this, hr.fistRegister)) return FileControl::Error("", file);
            hr.fistRegister = r.next;
            th.numeroRegistros--;
            if(!hr.setToFile(file, t.headRegister)) return FileControl::Error("", file);
            if(!th.setToFile(file, thSeg)) return FileControl::Error("", file);
        }
        else{
            if(!ax.freeRegisterSeg(file, *this, r.next)) return FileControl::Error("", file);
            r.next = ax.next;
            th.numeroRegistros--;
            if(!r.setToFile(file, seg)) return FileControl::Error("", file);
            if(!th.setToFile(file, thSeg)) return FileControl::Error("", file);
        }
    }
    else if(controle[2].empty()){
        Register ax;
        Segment seg = hr.fistRegister;
        vector<string> v = tmd.metaToString(file);
        unsigned int coluna;
        for(coluna = 1; coluna < v.size(); coluna++) if(controle[1] == v[coluna]) break;
        string vl;
        for(int i = 1; i < th.numeroRegistros; i++){
            vl = r.getVarFromFile(file, v[0], coluna);
            if(controle[2] == vl){
                if(!r.freeRegisterSeg(file, *this, hr.fistRegister)) return FileControl::Error("", file);
                hr.fistRegister = r.next;
                if(!hr.setToFile(file, t.headRegister)) return FileControl::Error("", file);
                th.numeroRegistros--;
            }
            else break;
            if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
        }
        for(int i = th.numeroRegistros; i > 1; i--){
            if(!ax.getFromFile(file, r.next)) return FileControl::Error("", file);
            vl = ax.getVarFromFile(file, v[0], coluna);
            if(controle[2] == vl){
                if(!ax.freeRegisterSeg(file, *this, r.next)) return FileControl::Error("", file);
                r.next = ax.next;
                if(!r.setToFile(file, seg)) return FileControl::Error("", file);
                th.numeroRegistros--;
            }
            else if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
        }
        if(!th.setToFile(file, thSeg)) return FileControl::Error("", file);
    }
    else if(controle[1] == "*"){
        Segment ax = hr.fistRegister;
        if(!r.freeRegisterSeg(file, *this, ax)) return FileControl::Error("", file);
        for(int i = 0; i < th.numeroRegistros; i++){
            ax = r.next;
            if(!r.getFromFile(file, r.next)) return FileControl::Error("", file);
            if(!r.freeRegisterSeg(file, *this, ax)) return FileControl::Error("", file);
        }
        th.numeroRegistros = 0;
        hr.fistRegister = Segment(0, 0);
        if(!th.setToFile(file, thSeg)) return FileControl::Error("", file);
        if(!hr.setToFile(file, t.headRegister)) return FileControl::Error("", file);

    }
    file.close();
    return !file.bad();
}

vector<MemRegister> DataBase::getRegister(vector<string> controle){
    /*
        controle[0] ~ nome da tabela
        controle[1] ~ ID ou coluna que tem que ser igual a controle[2] ou * pegar tudo
    */
    HeadTableHeader hth;
    TableHeader th;
    Table t;
    TableMetaDado tmd;
    HeadRegister hr;
    Register r;
    vector<MemRegister> mrv;
    fstream file;
    if(!searchTable(hth, th, tmd, controle[0]).isValid()) return FileControl::Error("A tabela nao foi encontrada", vector<MemRegister>(), file);
    if(!open(file)) return FileControl::Error("DataBase::open", vector<MemRegister>(), file);
    vector<string> v = tmd.metaToString(file);
    if(!t.getFromFile(file, th.tablePosition)) return FileControl::Error("", vector<MemRegister>(), file);
    if(!hr.getFromFile(file, t.headRegister)) return FileControl::Error("", vector<MemRegister>(), file);
    if(!r.getFromFile(file, hr.fistRegister)) return FileControl::Error("", vector<MemRegister>(), file);
    if(controle[1] == "ID"){
        for(int i = 1; i < stoi(controle[2]); i++) if(!r.getFromFile(file, r.next)) return FileControl::Error("", vector<MemRegister>(), file);
        MemRegister mr = r.registerToMemRegister(file, v, controle[0]);
        mr.meta = v[0];
        mrv.push_back(mr);
    }
    else if(controle[2].empty()){
        unsigned int coluna;
        if(v.empty()) return FileControl::Error("", vector<MemRegister>(), file);
        for(coluna = 1; coluna < v.size(); coluna++) if(controle[1] == v[coluna]) break;
        if(coluna == v.size()) return FileControl::Error("", vector<MemRegister>(), file);
        string vl = r.getVarFromFile(file, v[0], coluna);
        if(vl == controle[3]) mrv.push_back(r.registerToMemRegister(file, v, controle[0]));
        for(int i = 1; i < th.numeroRegistros; i++){
            if(!r.getFromFile(file, r.next)) return FileControl::Error("", vector<MemRegister>(), file);
            vl = r.getVarFromFile(file, v[0], coluna);
            if(vl == controle[3]) mrv.push_back(r.registerToMemRegister(file, v, controle[0]));
        }
    }
    else if(controle[1] == "*"){
        mrv.push_back(r.registerToMemRegister(file, v, controle[0]));
        for(int i = 1; i < th.numeroRegistros; i++){
            cout<<"Minha pica rapaz"<<endl;
            if(!r.getFromFile(file, r.next)) return FileControl::Error("", vector<MemRegister>(), file);
            mrv.push_back(r.registerToMemRegister(file, v, controle[0]));
        }
    }
    file.close();
    return mrv;
}

int FileControl::size(char type){
    if(type == INT) return sizeof(int);
    else if(type == CHAR) return sizeof(char);
    else if(type == FLOAT) return sizeof(float);
    else if(type == DOUBLE) return sizeof(double);
    else return -1;
}

Segment FileControl::discAlloc(fstream& f, int tam){
    Segment s;
    if(tam>0){
        char lixo[tam];
        f.seekp(0, fstream::end);
        s.base = f.tellp();
        s.limit = tam;
        f.write(lixo, tam);
    }
    else{
        s.base = 0;
        s.limit = 0;
    }
    return s;
}

bool FileControl::discAllocError(){
    return FileControl::Error("Error: FileControl::discAlloc");
}

bool FileControl::discAllocError(fstream& f){
    return FileControl::Error("Error: FileControl::discAlloc", f);
}

bool FileControl::getBlock(fstream& f, char *p, Segment s){
    if(s.isValid()){
        f.seekp(s.base);
        f.read(p, s.limit);
    }
    else{
        cerr("Segmento invalido");
        return false;
    }
    return !f.bad();
}

bool FileControl::getBlockError(){
    cerr("Error: FileControl::getBlock");
    return false;
}

bool FileControl::getBlockError(fstream& f){
    cerr("Error: FileControl::getBlock");
    f.close();
    return false;
}

bool FileControl::setBlock(fstream& f, char *p, Segment s){
    if(s.isValid()){
        f.seekp(s.base);
        f.write(p, s.limit);
    }
    else{
        cerr("Segmento invalido");
        return false;
    }
    return !f.bad();
}

bool FileControl::setBlockError(){
    cerr("Error: FileControl::setBlock");
    return false;
}

bool FileControl::setBlockError(fstream& f){
    cerr("Error: FileControl::setBlock");
    f.close();
    return false;
}

bool FileControl::openFile(fstream& f, string name, char modo){
    if(modo == ENTRADA) f.open(name, fstream::in | fstream::binary);
    else if(modo == SAIDA) f.open(name, fstream::out | fstream::binary);
    else if(modo == ENTRADA_SAIDA) f.open(name, fstream::in | fstream::out | fstream::binary);
    else cerr("Tipo de abertura invalido");
    return f.is_open();
}

bool FileControl::openFileError(){
    cerr("Error: FileControl::openFile");
    return false;
}

bool FileControl::openFileError(fstream& f){
    cerr("Error: FileControl::openFile");
    f.close();
    return false;
}

bool FileControl::Error(string s){
    cerr("Error: "<<s);
    return false;
}

bool FileControl::Error(string s, fstream& f){
    cerr("Error: "<<s);
    f.close();
    return false;
}

template<class T>
T FileControl::Error(string s, T r){
    cerr("Error: "<<s);
    return r;
}

template<class T>
T FileControl::Error(string s, T r, fstream& f){
    cerr("Error: "<<s);
    f.close();
    return r;
}
