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


#include <iostream>
#include <toml.hpp>
#include "../simulation.hpp"
#include "config.hpp"

namespace trochia::io::config {

auto load(const std::string &fname, std::vector<simulation::Simulation> &sims) -> void {
	using namespace toml;

	std::vector<math::Float> launcher_angle;
	simulation::Simulation sim;

	const auto config = parse(fname);

	// simulation common
	{
		const auto &cfg_sim = find(config, "simulation");
		sim.dt = cfg_sim.at("dt").as_floating();

		const auto output = find(cfg_sim, "output");
		sim.output_dt = output.at("dt").as_floating();

		sim.output_dir_fmt = output.at("dir").as_string();
	}

	// launcher info
	{
		const auto &cfg_launcher = find(config, "launcher");

		// get angle
		const auto angle = find(cfg_launcher, "angle");
		if(angle.is_integer())
			launcher_angle.push_back(angle.as_integer());
		else if(angle.is_floating())
			launcher_angle.push_back(angle.as_floating());
		else if(angle.is_table()){
			const auto start = find(angle, "start");
			const auto end   = find(angle, "end");
			if(start.is_integer() && end.is_integer()){
				for(int a=start.as_integer();a<end.as_integer();a++)
					launcher_angle.push_back(a);
			}else{
				std::cerr << "error: angle.start or angle.end are not integer" << std::endl;
			}
		}else if(angle.is_array()){
			launcher_angle = get<std::vector<double>>(angle);
		}
	}

	// rocket info
	{
		const auto &cfg_rkt = find(config, "rocket");
		const auto stage = find<std::vector<toml::table>>(cfg_rkt, "stage");
		if(stage.size() != 1){
			std::cerr
				<< "multistage rocket is not implemented now. sorry."
				<< std::endl;
		}

		const auto engine = stage[0].at("engine");
		sim.rocket.engine.load_eng(engine.as_string());

		sim.rocket.diameter = stage[0].at("diameter").as_floating();

		sim.rocket.lcg0 = stage[0].at("lcg0").as_floating();
		sim.rocket.lcgf = stage[0].at("lcgf").as_floating();

		sim.rocket.Cd = stage[0].at("Cd").as_floating();
		sim.rocket.Cna= stage[0].at("Cna").as_floating();
	}

	for(const auto &a : launcher_angle){
		sim.launcher_angle = a;
		sims.push_back(sim);
	}
}

}
