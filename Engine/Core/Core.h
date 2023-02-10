#pragma once

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
				virtual EventType GetEventType() const override {return GetStaticType();}\
				virtual const char* GetName() const override {return #type;}
#define EVENT_CLASS_CATEGORY(category) virtual unsigned int GetCategoryFlags() const override {return category;}

#define INFORMER() unsigned int leftCount = 0;\
				   void SetupInformer(unsigned int count) { leftCount += count; }\
				   bool Informing() { if(leftCount == 0) return false; else { --leftCount; return true; } }

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

#define FLOAT_MIN  1.175494351e-38F
#define FLOAT_MAX  3.402823466e+38F
#define INT32_MAX  2147483647i32
#define UINT64_MAX 0xffffffffffffffffui64