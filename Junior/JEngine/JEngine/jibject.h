#include "type.h"
#include "enum.h"

namespace jengine {
	class JIObgect
	{
	public:
		JIObgect(common::Coordinate x, common::Coordinate y, common::Coordinate alpha);
		virtual ~JIObgect() = 0;

		common::Coordinate getX();
		common::Coordinate getX();
		common::Angle getAlpha();

		void setX(common::Coordinate x);
		void setY(common::Coordinate y);
		void setAlpha(common::Coordinate alpha);

	private:
		common::Coordinate m_x;
		common::Coordinate m_y;
		common::Angle m_alpha;
	};
}