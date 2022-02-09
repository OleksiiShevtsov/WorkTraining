#include "jengine.h"

void jengine::JE::engage() {
	// ???
	while (!m_isTerminating) {
		m_thread = std::thread(&JE::recalculationDisplacement, this);
	}
}

void jengine::JE::recalculationDisplacement() {

}

void jengine::JE::addObject() {

}
