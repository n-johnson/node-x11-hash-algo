#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdint.h>

extern "C" {
    #include "x11.h"
}


using namespace node;
using namespace v8;

Handle<Value> except(const char* msg) {
    return ThrowException(Exception::Error(String::New(msg)));
}

Handle<Value> hash(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 1)
        return except("You must provide one argument.");

    Local<Object> target = args[0]->ToObject();

    if(!Buffer::HasInstance(target))
        return except("Argument should be a buffer object.");

    char * input = Buffer::Data(target);
    char output[32];

    uint32_t input_len = Buffer::Length(target);

    x11_hash(input, output, input_len);

    Buffer* buff = Buffer::New(output, 32);
    return scope.Close(buff->handle_);
}


void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("hash"), FunctionTemplate::New(hash)->GetFunction());
}

NODE_MODULE(x11, init)