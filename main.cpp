#include <iostream>
#include "FileController.hpp"
#include "Interpreter.hpp"
using namespace std;

int main()
{
    prepare_file("queries.txt");
    Token t = get_token();
    t.print();
    t = get_token();
    t.print();
    t = get_token();
    t.print();
    /*
    DataBase db("teste");
    if(!db.create("teste")) return 0;
    if(!db.open()) return 0;
    if(!db.insertTable("Alunos")) return 0;
    db.insertColumn("Alunos", "Nome", 's');
    string nome = "Candido";
    Node n("Nome", &nome);
    MemRegister mr("Alunos");
    mr.insert(n);
    db.insertRegister(mr);
    db.close();
    //mr.
    */
    return 0;
}
