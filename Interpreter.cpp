//****************************** Interpreter.cpp *************************************
#include "Interpreter.hpp"

ifstream Entrada;
Token tn, tna;
int linha = 1, coluna = 1;

DataBase database;
vector<string> identificadores;

vector<string> Reserved_Words = { "ADD", "ALTER", "COLUMN", "CREATE", "DATABASE", "DELETE", "FROM", "IN", 
    "INSERT", "INTO", "ON", "SELECT", "SET", "TABLE", "UPDATE", "USE", "VALUES", "WHERE" };

bool prepare_file(string s){
    Entrada.open(s, ifstream::binary);
    return Entrada.is_open();
}

void Error(string e){
    cerr<<"Ocorreu um erro na linha "<< linha << "e coluna " << coluna <<" : "<<e<<endl;
    exit(0);
}

int is_reserved(string s){
    auto it = lower_bound(Reserved_Words.begin(), Reserved_Words.end(), s);
    return *it == s ? it - Reserved_Words.begin() : -1;
}

int is_type(string s){
    if(s == "int") return INT;
    else if (s == "float") return FLOAT;
    else if (s == "char") return CHAR;
    else if (s == "string") return STRING;
    else return -1;
}

bool close_file(){
    Entrada.close();
    return true;
}

char get_next_char(){
    coluna++;
    return Entrada.get();
}

void unget_char(char c){
    coluna--;
    Entrada.unget();
}

void get_first_token(){
    tna = get_token();
}

void get_next_token(){
    tn.free_token();
    tn = tna;
    if(!tna.is_final()) tna = get_token();
    else cout("Fim arquivo");
}

bool Token::is_final(){
    return this->cat == FINAL;
}

void Token::free_token(){
    free(this->valor);
}

void Token::print(){
    if(this->valor){
        cout<<"Categoria: ";
        switch(this->cat){
            case IDENTIFIER:
                cout<<"Identificador"<<endl;
                cout<<"Valor: "<< *cstring(this->valor)<<endl;
                break;
            case RESERVED:
                cout<<"Reservado"<<endl;
                cout<<"Valor: "<< Reserved_Words[*cchar(this->valor)]<<endl;
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
}

bool Token::constant(){
    return (this->cat == CINT || this->cat == CFLOAT || this->cat == CSTRING || this->cat == CCHAR);
}

char Token::reserved(){
    if(this->cat == RESERVED) return this->get_char();
    return -1;
}

char Token::oper(){
    if(this->cat == OPERATOR) return this->get_char();
    return -1;
}

char Token::logical(){
    if(this->cat == LOGICAL) return this->get_char();
    return -1;
}

bool Token::identifier(){
    return this->cat == IDENTIFIER;
}

char Token::type(){
    if(this->cat == TYPE) return this->get_char();
    return -1;
}

int Token::get_int(){
    return *cint(this->valor);
}

float Token::get_float(){
    return *cfloat(this->valor);
}

char Token::get_char(){
    return *cchar(this->valor);
}

string Token::get_string(){
    if(this->cat == CINT) return to_string(this->get_int());
    //TOF
    else if (this->cat == CFLOAT) return to_string(this->get_float());
    else if (this->cat == CCHAR) return string("" + this->get_char());
    return *cstring(this->valor);
}

bool Expre(){
    get_next_token();
    if(tn.identifier()){
        identificadores.push_back(tn.get_string());
        get_next_token();
        if(tn.logical() == IGUAL) {
            get_next_token();
            return tn.constant();
        }
    } else if(tn.constant()){
        identificadores.push_back(tn.get_string());
        return true;
    }
    return false;
}

void Values(){
    get_next_token();
    if(tn.oper() != ABRE_PARENTESIS) Error("Esperava-se um \' ( \' depois de VALUES");
    get_next_token();
    if(!tn.constant()) Error("Esperava-se uma constante depois do \' ( \'");
    identificadores.push_back(tn.get_string());
    while(tna.oper() == VIRGULA) {
        get_next_token();
        get_next_token();
        if(!tn.constant()) Error("Esperava-se uma constante depois da \' , \'");
        identificadores.push_back(tn.get_string());
    }
    get_next_token();
    if(tn.oper() != FECHA_PARENTESIS) Error("Esperava-se um \' ) \' depois de VALUES");
}

void Set(){
    get_next_token();
    if(!tn.identifier()) Error("Esperava-se um Identificador depois do SET");
    identificadores.push_back(tn.get_string());
    get_next_token();
    if(tn.oper() != ATRIBUICAO) Error("Esperava-se um \' = \' depois do Identificador");
    get_next_token();
    if(!tn.constant()) Error("Esperava-se uma Constante depois do \' = \'");
    identificadores.push_back(tn.get_string());
    get_next_token();
    if(tn.oper() != ATRIBUICAO) Error("Esperava-se um \' = \' depois do ");
    while(tna.oper() == VIRGULA){
        get_next_token();
        get_next_token();
        if(!tn.identifier()) Error("Esperava-se um Identificador depois da \' ,\'");
        identificadores.push_back(tn.get_string());
        get_next_token();
        if(tn.oper() != ATRIBUICAO) Error("Esperava-se um \' = \' depois do Identificador");
        get_next_token();
        if(!tn.constant()) Error("Esperava-se uma Constante depois do \' = \'");
        identificadores.push_back(tn.get_string());
        get_next_token();
    }
}

void Where(){
    if(tna.reserved() == WHERE){
        get_next_token();
        if(!Expre()) Error("Expressao invalida!");
        identificadores.push_back(tn.get_string());
    }
}

void Query(){
    get_next_token();
    switch (tn.reserved())
    {
        case CREATE:
            get_next_token();
            if(tn.reserved() == DATABASE){
                get_next_token();
                if(!tn.identifier()) Error("Esperava-se um identificador depois do DATABASE");
                identificadores.push_back(tn.get_string());
                get_next_token();
                if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
                if(DataBase::create(identificadores[0])) cout("DataBase Criado com sucesso!");
                else Error("Houve um Error na criacao do Banco!");
            } else if(tn.reserved() == TABLE){
                get_next_token();
                tn.print();
                cout(tn.identifier());
                if(!tn.identifier()) Error("Esperava-se um identificador depois do TABLE");
                identificadores.push_back(tn.get_string());
                get_next_token();
                if(tna.oper() == ABRE_PARENTESIS){
                    get_next_token();
                    if(!tn.type()) Error("Esperava-se um Type depois do \' ( \'");
                    identificadores.push_back(tn.get_string());
                    get_next_token();
                    if(!tn.identifier()) Error("Esperava-se um identificador depois do Type");
                    identificadores.push_back(tn.get_string());
                    while(tna.oper() == VIRGULA){
                        get_next_token();
                        get_next_token();
                        if(!tn.type()) Error("Esperava-se um Type depois da \' , \'");
                        identificadores.push_back(tn.get_string());
                        get_next_token();
                        if(!tn.identifier()) Error("Esperava-se um identificador depois do Type");
                        identificadores.push_back(tn.get_string());
                    }
                }
                if(!database.isOpen()) Error("Nenhum banco selecionado");
                if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
                if(database.insertTable(identificadores[0])) cout("Tabela Adicionada");
            } else Error("Esperava-se um identificador depois do comando CREATE");
            break;
        case ALTER:
            get_next_token();
            if(tn.reserved() != TABLE) Error("Esperava-se TABLE depois do comando ALTER");
            get_next_token();
            if(!tn.identifier()) Error("Esperava-se um identificador depois de TABLE");
            identificadores.push_back(tn.get_string());
            get_next_token();
            if(tn.reserved() != ADD) 
            get_next_token();
            if(tn.reserved() != COLUMN){
                get_next_token();
                if(tn.reserved() != ABRE_PARENTESIS) Error("Esperava-se um \"(\" depois do COLUMN");
                get_next_token();
                if(!tn.type()) Error("Esperava-se um tipo depois do \"(\"");
                identificadores.push_back("" + tn.get_char());
                get_next_token();
                if(!tn.identifier()) Error("Esperava-se um Identificador depois do tipo");
                identificadores.push_back(tn.get_string());
                get_next_token();
                if(tn.reserved() != FECHA_PARENTESIS) Error("Esperava-se um \")\" depois do identificador");
                get_next_token();
                if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");

                if (database.insertColumn(identificadores[0], identificadores[2], identificadores[1][0]))
                    cout("Coluna Criada!");
            //} else if() {

            } else {
                Error("");
            }
            break;
        case SELECT:
            get_next_token();
            if(!(tn.identifier())) Error("Esperava-se um identificador depois do comando SELECT");
            identificadores.push_back(tn.get_string());
            get_next_token();
            if(tn.reserved() != FROM) Error("Esperava-se o comando FROM depois do identificador");
            get_next_token();
            if(!(tn.identifier())) Error("Esperava-se um identificador depois do comando FROM");
            identificadores.push_back(tn.get_string());
            Where();
            get_next_token();
            if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
            {
                auto mrs = database.getRegister(identificadores);
                for(auto mr : mrs) mr.print();
            }
            break;
        case INSERT:
            get_next_token();
            if(!(tn.reserved() == INTO)) Error("Esperava-se INTO depois do comando INSERT");
            get_next_token();
            if(!tn.identifier()) Error("Esperava-se um Identificador depois do INTO");
            identificadores.push_back(tn.get_string());
            get_next_token();
            if(!(tn.reserved() == VALUES)) Error("Esperava-se VALUES depois do identificador");
            Values();
            get_next_token();
            if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
            {
                string meta = database.getTableHeader(identificadores[0]).getTableMeta(database);
                auto mr = MemRegister(identificadores, meta);
                database.insertRegister(mr);
            }
            break;
        case UPDATE:
            get_next_token();
            if(!tn.identifier()) Error("Esperava-se um identificador depois do UPDATE");
            identificadores.push_back(tn.get_string());
            get_next_token();
            if(tn.reserved() != SET) Error("Esperava-se SET depois do identificador");
            Set();
            Where();
            get_next_token();
            if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
            if(identificadores.size() > 3){
                vector<string>  param = { identificadores[0], "", "", identificadores[1] };
                database.updateRegister(param, identificadores[2]);
            } else {
                vector<string>  param = { identificadores[0], identificadores[3], identificadores[4], identificadores[1] };
                database.updateRegister(param, identificadores[2]);
            }
            break;
        case DELETE:
            get_next_token();
            if(tn.reserved() != FROM) Error("Esperava-se um identificador depois do DELETE");
            get_next_token();
            if(!tn.identifier()) Error("Esperava-se um identificador depois do FROM");
            identificadores.push_back(tn.get_string());
            Where();
            get_next_token();
            if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
            {
                vector<string> param = { identificadores[0], identificadores[1], identificadores[2] };
                database.deleteRegister(param);
            }
            break;
        case USE:
            get_next_token();
            if(!tn.identifier()) Error("Esperava-se um identificador depois do USE");
            identificadores.push_back(tn.get_string());
            get_next_token();
            if(tn.oper() != PONTO_VIRGULA) Error("Ponto e virgula");
            database = DataBase(identificadores[0]);
            if(!database.open()) Error("Error ao abrir o banco");
            cout("Usando "<< identificadores[0]);
            break;
        default:
            Error("Esperava-se um comando USE, SELECT, INSERT, UPDATE OU DELETE");
            break;
    }
    identificadores.clear();
}

void Execute(){
    prepare_file("queries.txt");
    get_first_token();
    while(!tn.is_final()){
        get_next_token();
        tn.print();
    }
    //while(!tn.is_final()) Query();
}

Token get_token(){
    int estado = 0;
    char valor = 0;
    string buffer = "";
    while(true){
        char c;
        switch(estado){
            case 0:
                c = get_next_char();
                if(isalpha(c)){
                    estado = 1;
                    buffer += c;
                } else if(isdigit(c)){
                    estado = 3;
                    buffer += c;
                } else if (c == '\"'){
                    estado = 23;
                } else if (c == '\''){
                    estado = 21;
                } else if(c == '+'){
                    estado = 16;
                    buffer += c;
                } else if(c == '-'){
                    estado = 17;
                    buffer += c;
                } else if(c == '*'){
                    estado = 18;
                    buffer += c;
                } else if(c == '/'){
                    estado = 19;
                    buffer += c;
                } else if(c == '.'){
                    estado = 25;
                    buffer += c;
                } else if (c == '='){
                    estado = 7;
                    buffer += c;
                } else if (c == '>'){
                    estado = 10;
                    buffer += c;
                } else if (c == '<'){
                    estado = 12;
                    buffer += c;
                } else if (c == ' '){
                    //coluna++;
                } else if (c == '\n' || c == '\r'){
                    coluna = 1;
                    linha++;
                } else if (c == '\t'){
                    coluna+=3;
                } else if(c == ';') {
                    valor = PONTO_VIRGULA;
                    return Token(OPERATOR, &valor);
                } else if(c == EOF) {
                    return Token(FINAL, nullptr);
                } else {
                    cout<<int(c)<<endl;
                    estado = -1;
                }
                break;
            case 1:
                c = get_next_char();
                if(!(isalpha(c) || isdigit(c))) estado = 2;
                else buffer += c;
                break;
            case 2:
                unget_char(c);
                valor = is_reserved(buffer);
                if(valor != -1) return Token(RESERVED, &valor);
                else {
                    valor = is_type(buffer);
                    if(valor != -1) return Token(TYPE, &valor);
                    return Token(IDENTIFIER, &buffer);
                }
            case 3:
                c = get_next_char();
                if(isdigit(c)) buffer += c;
                else if(c == '.') {
                    buffer += c;
                    estado = 4;
                }
                else estado = 5;
                break;
            case 4:
                c = get_next_char();
                if(isdigit(c)) buffer += c;
                else estado = 6;
                break;
            case 5:
                unget_char(c);
                //int ax = stoi(buffer, nullptr, 10);
                return Token(CINT, &buffer);
            case 6:
                unget_char(c);
                //float ax = stof(buffer, nullptr);
                return Token(CFLOAT, &buffer);
            case 7:
                c = get_next_char();
                if (c == '=') estado = 9;
                else estado = 8;
                break;
            case 8:
                unget_char(c);
                valor = ATRIBUICAO;
                return Token(OPERATOR, &valor);
            case 9:
                valor = IGUAL;
                return Token(LOGICAL, &valor);
            case 10:
                c = get_next_char();
                if(c == '=') estado = 14;
                else estado = 11;
                break;
            case 11:
                valor = MAIOR_IGUAL;
                return Token(LOGICAL, &valor);
                break;
            case 12:
                c = get_next_char();
                if(c == '=') estado = 13;
                else estado = 15;
                break;
            case 13:
                valor = MENOR_IGUAL;
                return Token(LOGICAL, &valor);
            case 14:
                unget_char(c);
                valor = MAIOR;
                return Token(LOGICAL, &valor);
            case 15:
                unget_char(c);
                valor = MENOR;
                return Token(LOGICAL, &valor);
            case 16:
                valor = MAIS;
                return Token(OPERATOR, &valor);
            case 17:
                c = get_next_char();
                if(c == '-') estado = 27;
                else estado = 26;
            case 18:
                valor = VEZES;
                return Token(OPERATOR, &valor);
            case 19:
                valor = DIVISAO;
                return Token(OPERATOR, &valor);
            case 20:
                c = get_next_char();
                buffer += c;
                estado = 21;
                break;
            case 21:
                c = get_next_char();
                if(c == '\'') estado = 22;
                else estado = -1;
                break;
            case 22:
                return Token(CCHAR, &c);
            case 23:
                c = get_next_char();
                if(c == '\"') estado = 24;
                else buffer += c;
                break;
            case 24:
                return Token(CSTRING, &buffer);
            case 25:
                valor = PONTO;
                return Token(OPERATOR, &valor);
            case 26:
                unget_char(c);
                valor = MENOS;
                return Token(OPERATOR, &valor);
            case 27:
                get_next_char();
                if(c == '\n') estado = 0;
                break;
            default:
                FileControl::Error("Ocorreu um erro na linha: " + to_string(linha) + " e coluna: " + to_string(coluna));
                exit(0);
                break;    
        }
    }
}