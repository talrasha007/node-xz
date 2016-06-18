#pragma once
#ifdef min
#undef min
#endif

#include <algorithm>
#include <nan.h>

template <typename TR> class BufferExports {
public:
    void setup(v8::Handle<v8::Object> exports) {
        Nan::SetMethod(exports, "compress", compress);
        Nan::SetMethod(exports, "decompress", decompress);
    }

    static NAN_METHOD(compress) {

        if (info.Length() == 0) {
            return;
        }

        int arg1 = -1;
        int arg2 = -1;

        if (info[1]->IsNumber()) { arg1 = (int)info[1]->IntegerValue(); }
        if (info[2]->IsNumber()) { arg2 = (int)info[2]->IntegerValue(); }

        size_t len = node::Buffer::Length(info[0]);
        const char *in = node::Buffer::Data(info[0]);

        size_t outLen = len + 128 + std::min(size_t(128 * 1024 * 1024), len / 3);
        char *out = new char[outLen];

        TR::compress(out, &outLen, in, len, arg1, arg2);

        v8::Local<v8::Object> ret = Nan::CopyBuffer(out, outLen).ToLocalChecked();

        delete out;
        info.GetReturnValue().Set(ret);
    }

    static NAN_METHOD(decompress) {

        if (info.Length() == 0) {
            return;
        }

        size_t len = node::Buffer::Length(info[0]);
        const char *in = node::Buffer::Data(info[0]);

        size_t outLen = TR::getUnpackSize(in, len);
        if (outLen == 0) {
            Nan::ThrowError("Bad input.");
            return;
        }
		
		v8::Local<v8::Object> ret = Nan::NewBuffer(outLen).ToLocalChecked();
        char *out = node::Buffer::Data(ret);

        if (TR::decompress(out, &outLen, in, &len) != 0) {
            Nan::ThrowError("Bad input.");
            return;
        }

        info.GetReturnValue().Set(ret);
    }
};