#pragma once
#include "Utils.h"
#include <iostream>
#include <string>
#include <sstream>

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

inline std::string report_exception(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::TryCatch& try_catch) {
	std::stringstream ss;
	ss << "Uncaught exception: ";

	std::string exceptionStr = str(isolate, try_catch.Exception());
	ss << exceptionStr; // TODO(aroman) JSON-ify objects?

	if (!try_catch.Message().IsEmpty()) {
		if (!try_catch.Message()->GetScriptResourceName()->IsUndefined()) {
			ss << std::endl
				<< "at " << str(isolate, try_catch.Message()->GetScriptResourceName());

			v8::Maybe<int> line_no = try_catch.Message()->GetLineNumber(ctx);
			v8::Maybe<int> start = try_catch.Message()->GetStartColumn(ctx);
			v8::Maybe<int> end = try_catch.Message()->GetEndColumn(ctx);
			v8::MaybeLocal<v8::String> sourceLine = try_catch.Message()->GetSourceLine(ctx);

			if (line_no.IsJust()) {
				ss << ":" << line_no.ToChecked();
			}
			if (start.IsJust()) {
				ss << ":" << start.ToChecked();
			}
			if (!sourceLine.IsEmpty()) {
				ss << std::endl
					<< "  " << str(isolate, sourceLine.ToLocalChecked());
			}
			if (start.IsJust() && end.IsJust()) {
				ss << std::endl
					<< "  ";
				for (int i = 0; i < start.ToChecked(); i++) {
					ss << " ";
				}
				for (int i = start.ToChecked(); i < end.ToChecked(); i++) {
					ss << "^";
				}
			}
		}
	}

	if (!try_catch.StackTrace(ctx).IsEmpty()) {
		ss << std::endl << "Stack trace: " << str(isolate, try_catch.StackTrace(ctx).ToLocalChecked());
	}

	return ss.str();
}