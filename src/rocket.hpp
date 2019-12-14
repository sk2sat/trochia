/* ----------------------------------------------------------------------- *
 *
 *    Copyright (C) 2019 sksat <sksat@sksat.net>
 *
 *    This file is part of Trochia.
 *
 *    Trochia is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Trochia is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You Should have received a copy of the GNU General Public License
 *    along with Trochia.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ----------------------------------------------------------------------- */

#ifndef ROCKET_HPP_
#define ROCKET_HPP_

#include <memory>
#include <string>
#include "math.hpp"
#include "coordinate.hpp"
#include "engine.hpp"

#include "object.hpp"

namespace trochia::rocket {
	using LocalFrame = coordinate::local::NED;

	class Rocket : public object::Object<LocalFrame> {
	public:
		Rocket(){}
		Rocket(const object::Object<LocalFrame> &o) : object::Object<LocalFrame>(o) {}

		std::string name;
		Engine engine;

		math::Float diameter;		// 直径(m)
		math::Float lcg0, lcgf;		// 重心位置
		math::Float Cd;				// 抗力係数

		static auto dx(const math::Float &t, const Rocket &r) -> const Rocket {
			return object::Object<LocalFrame>::dx(t, r);
		}

		auto lcg() const -> math::Float {
			return math::lerp(lcg0, lcgf, engine.progress(this->time));
		}

		auto weight() const -> math::Float {
			return 10.0 + engine.weight(this->time);
		}
	};
}

#endif
