#pragma once
#include "Utils.h"

inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
	const char* data,
	int length) {
	return v8::String::NewFromOneByte(isolate,
		reinterpret_cast<const uint8_t*>(data),
		v8::NewStringType::kNormal,
		length).ToLocalChecked();
}

inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
	const signed char* data,
	int length) {
	return v8::String::NewFromOneByte(isolate,
		reinterpret_cast<const uint8_t*>(data),
		v8::NewStringType::kNormal,
		length).ToLocalChecked();
}

inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
	const unsigned char* data,
	int length) {
	return v8::String::NewFromOneByte(
		isolate, data, v8::NewStringType::kNormal, length)
		.ToLocalChecked();
}