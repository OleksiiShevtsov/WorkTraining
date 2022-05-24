#pragma once

#include "type.h"
#include "enum.h"

namespace jengine {
	class JIObgect
	{
	public:
		virtual ~JIObgect() = 0;

		/*virtual common::Coordinate getX() = 0;
		virtual common::Coordinate getY() = 0;
		virtual common::Angle getAlpha() = 0;

		virtual void setX(common::Coordinate x) = 0;
		virtual void setY(common::Coordinate y) = 0;
		virtual void setAlpha(common::Coordinate alpha) = 0;*/

	protected:
		common::Coordinate m_x;
		common::Coordinate m_y;
		common::Angle m_alpha;
	};
}