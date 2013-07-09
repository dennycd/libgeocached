#include <iostream>
#include <node.h>
#include <v8.h>

using namespace v8;
using namespace std;



static Handle<Value> Dummy(const Arguments& args){
    HandleScope scope;
    cout << "dummy call" << endl;
    return scope.Close(Undefined());   
}

void InitModule(Handle<Object> target){

    //expose module elements
    target->Set(v8::String::NewSymbol("dummy"), FunctionTemplate::New(Dummy)->GetFunction());
}

NODE_MODULE(geocached, InitModule)