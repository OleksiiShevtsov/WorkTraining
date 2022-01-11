#include <vector>
#include <memory>
#include <thread>

#include "jibject.h"

namespace jengine {

	class JE {
	public:
		void engage();
		void recalculationDisplacement();
		void addObject();

	private:
		std::thread m_thread;
		std::vector<std::unique_ptr<JIObgect>> m_obgects;
	};
}