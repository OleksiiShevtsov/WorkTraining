#include "jibject.h"

namespace jengine {

	class JDynamicObject : public JIObgect
	{
	public:
		JDynamicObject(common::Coordinate x, common::Coordinate y, common::Coordinate alpha, common::Coordinate omega);
		~JDynamicObject();

		common::AngularVelocity getOmega();
		common::Speed getSpeed();
		common::Acceleration getAcceleration();

		void setOmega(common::Coordinate omega);
		void setSpeed(common::Speed speed);
		void setAcceleration(common::Acceleration acceleration);

	protected:
		common::AngularVelocity m_omega;
		common::Speed m_speed;
		common::Acceleration m_acceleration;

		CurrentLinearMotion m_currentLinearMotion;
		CurrentAngularMotion m_currentAngularMotion;
	};
}