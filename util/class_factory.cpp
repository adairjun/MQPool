#include "MQPool/class_factory.h"

ClassFactory::ClassFactory() {
}

ClassFactory::~ClassFactory() {
}

void ClassFactory::Dump() const {

}

ClassFactory& ClassFactory::Instance() {
	static ClassFactory sInstance;
	return sInstance;
}

