#pragma once

#include "jibject.h"

namespace jengine {

	class JDynamicObject : public JIObgect
	{
	public:
		
		JDynamicObject(common::Coordinate x, common::Coordinate y, common::Angle alpha, common::AngularVelocity omega)
			: m_omega{ omega }
		{
			m_x = x;
			m_y = y;
			m_alpha = alpha;
		}

		~JDynamicObject();

		/*common::Coordinate getX();
		common::Coordinate getY();
		common::Angle getAlpha();

		common::AngularVelocity getOmega();
		common::Speed getSpeed();
		common::Acceleration getAcceleration();

		void setX(common::Coordinate x);
		void setY(common::Coordinate y);
		void setAlpha(common::Coordinate alpha);

		void setOmega(common::Coordinate omega);
		void setSpeed(common::Speed speed);
		void setAcceleration(common::Acceleration acceleration);*/

	private:
		common::AngularVelocity m_omega;
		common::Speed m_speed;
		common::Acceleration m_acceleration;

		CurrentLinearMotion m_currentLinearMotion;
		CurrentAngularMotion m_currentAngularMotion;
	};
}