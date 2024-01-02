#include "sync.h"

Syncronization& Syncronization::Syncronizer() {
	static Syncronization instance;
	return instance;
}