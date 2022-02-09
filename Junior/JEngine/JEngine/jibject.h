#include "type.h"
#include "enum.h"

namespace jengine {
	class JIObgect
	{
	public:
		JIObgect(common::Coordinate x, common::Coordinate y, common::Coordinate alpha);
		virtual ~JIObgect() = 0;

		virtual common::Coordinate getX();
		virtual common::Coordinate getY();
		virtual common::Angle getAlpha();

		virtual void setX(common::Coordinate x);
		virtual void setY(common::Coordinate y);
		virtual void setAlpha(common::Coordinate alpha);

	private:
		common::Coordinate m_x;
		common::Coordinate m_y;
		common::Angle m_alpha;
	};
}