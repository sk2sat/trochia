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

#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <string>
#include "math.hpp"
#include "rocket.hpp"

namespace trochia {
	class Simulation {
	public:
		math::Float timeout = 60.0;
		math::Float dt, output_dt;
		std::string output_dir;

		rocket::Rocket rocket;
	};

	auto do_simulation(Simulation &sim) -> void;
}

#endif
