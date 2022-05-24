#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <atomic>

#include "jibject.h"
#include "jdynamic_object.h"

namespace jengine {

	class JE {
	public:

		JE() {
			//m_obgects.push_back( std::make_unique < JDynamicObject >( JDynamicObject( 0, 0, 0, 1 ) ) );
		}

		void engage();
		void recalculationDisplacement();
		void addObject();

	private:
		std::thread m_thread;
		std::atomic< bool > m_isTerminating = false;
		std::vector<std::unique_ptr<JIObgect>> m_obgects;
	};
}