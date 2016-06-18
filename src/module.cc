#include <nan.h>

#include "lzma.h"
#include "ppmd.h"

using namespace v8;

void init(v8::Handle<v8::Object> exports) {
    Local<Object> lzma = Nan::New<Object>();
    LzmaTr::setup(lzma);
    Nan::Set(exports, Nan::New("lzma").ToLocalChecked(), lzma);

    Local<Object> ppmd = Nan::New<Object>();
    PpmdTr::setup(ppmd);
    Nan::Set(exports, Nan::New("ppmd").ToLocalChecked(), ppmd);

}

NODE_MODULE(lzma, init);