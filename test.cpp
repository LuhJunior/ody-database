#include <iostream>
#include <vector>

using namespace std;

typedef enum c { um = 'u', dois = 'd'} e;

int main (){
    string a= "ASDADAS", b= "SDFDSFSD";
    e u;
    u = 2;
    cout<<u;
    vector<string> v = {a, b};
    cout<<v[0]<<v[1];
    return 0;    
}