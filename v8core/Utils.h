#pragma once
#include <v8.h>

class PersistentToLocal {
public:
	// If persistent.IsWeak() == false, then do not call persistent.Reset()
	// while the returned Local<T> is still in scope, it will destroy the
	// reference to the object.
	template <class TypeName>
	static inline v8::Local<TypeName> Default(
		v8::Isolate* isolate,
		const v8::PersistentBase<TypeName>& persistent) {
		if (persistent.IsWeak()) {
			return PersistentToLocal::Weak(isolate, persistent);
		}
		else {
			return PersistentToLocal::Strong(persistent);
		}
	}

	// Unchecked conversion from a non-weak Persistent<T> to Local<T>,
	// use with care!
	//
	// Do not call persistent.Reset() while the returned Local<T> is still in
	// scope, it will destroy the reference to the object.
	template <class TypeName>
	static inline v8::Local<TypeName> Strong(
		const v8::PersistentBase<TypeName>& persistent) {
		return *reinterpret_cast<v8::Local<TypeName>*>(
			const_cast<v8::PersistentBase<TypeName>*>(&persistent));
	}

	template <class TypeName>
	static inline v8::Local<TypeName> Weak(
		v8::Isolate* isolate,
		const v8::PersistentBase<TypeName>& persistent) {
		return v8::Local<TypeName>::New(isolate, persistent);
	}
};

// Convenience wrapper around v8::String::NewFromOneByte().
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
	const char* data,
	int length = -1);

// For the people that compile with -funsigned-char.
inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
	const signed char* data,
	int length = -1);

inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
	const unsigned char* data,
	int length = -1);

// Used to be a macro, hence the uppercase name.
template <int N>
inline v8::Local<v8::String> FIXED_ONE_BYTE_STRING(
	v8::Isolate* isolate,
	const char(&data)[N]) {
	return OneByteString(isolate, data, N - 1);
}

template <std::size_t N>
inline v8::Local<v8::String> FIXED_ONE_BYTE_STRING(
	v8::Isolate* isolate,
	const std::array<char, N>& arr) {
	return OneByteString(isolate, arr.data(), N - 1);
}