#pragma once

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
				virtual EventType GetEventType() const override {return GetStaticType();}\
				virtual const char* GetName() const override {return #type;}
#define EVENT_CLASS_CATEGORY(category) virtual unsigned int GetCategoryFlags() const override {return category;}

#define INFORMER() unsigned int leftCount = 0;\
				   void SetupInformer(unsigned int count) { leftCount += count; }\
				   bool Informing() { if(leftCount == 0) return false; else { --leftCount; return true; } }