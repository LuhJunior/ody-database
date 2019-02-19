#ifndef TOOLS_H
#define TOOLS_H

#define cout(x) cout<<x<<endl
#define cerr(x) cerr<<x<<endl
#define cint(x) (int *) x
#define cchar(x) (char *) x
#define cstring(x) (string *) x
#define cfloat(x) (float *) x
#define cdouble(x) (double *) x
#define cbool(x) (bool *) x
#define cvoid(x) (void *) x

typedef enum Types { BOOL = 'b', INT = 'i', FLOAT = 'f', DOUBLE = 'd', CHAR = 'c', STRING = 's'} Type;

#endif