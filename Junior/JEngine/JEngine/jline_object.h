#include "jibject.h"

namespace jengine {

	class JLineObject : public JIObgect
	{
	public:
		JLineObject(common::Coordinate x, common::Coordinate y, common::Coordinate alpha);
		~JLineObject();

		common::Speed getSpeed();
		common::Acceleration getAcceleration();

		void setSpeed(common::Speed speed);
		void setAcceleration(common::Acceleration acceleration);

	protected:
		common::Speed m_speed;
		common::Acceleration m_acceleration;
		CurrentLinearMotion m_currentLinearMotion;
	};
}