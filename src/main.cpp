#include <iostream>
#include <string>
#include <toml.hpp>
#include "simulation.hpp"

#include "rocket.hpp"
#include "environment/gravity.hpp"

using math::Float;

auto do_simulation(Simulation &sim) -> void;

auto main(int argc, char **argv) -> int {
	Simulation sim;

	std::cout << "rocket simulator by sksat" << std::endl;

	std::cout << "loading config file ...";
	{
		using namespace toml;
		const auto config = parse("config.toml");

		const auto &cfg_sim = find(config, "simulation");
		const auto &cfg_output = find(cfg_sim, "output");
		sim.dt = cfg_sim.at("dt").as_floating();
		sim.output_dt = cfg_output.at("dt").as_floating();

		const auto &rocket = find(config, "rocket");
		const auto stage = find<std::vector<toml::table>>(rocket, "stage");
		if(stage.size() != 1){
			std::cerr
				<< "multistage rocket is not implemented now. sorry."
				<< std::endl;
			return -1;
		}

		const auto engine = stage[0].at("engine");
		sim.rocket.engine.load_eng(engine.as_string());
	}

	std::cout << "start simulation" << std::endl;
	do_simulation(sim);

	return 0;
}

auto do_simulation(Simulation &sim) -> void {
	const auto &timeout = sim.timeout;
	const auto &dt = sim.dt;
	const auto &output_dt = sim.output_dt;
	if(output_dt < dt)
		return;

	const size_t output_rate = sim.output_dt / sim.dt;
	auto &rocket = sim.rocket;

	// init
	rocket.time = 0.0;
	auto &pos = rocket.pos;
	pos.altitude(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec << 0.0, 0.0, 0.0;
	rocket.acc.vec << 0.0, 0.0, 0.0;

	// main loop
	size_t step = 0;
	while(true){
		// thrust
		const auto thrust = rocket.engine.thrust(rocket.time); // first stage only
		rocket.acc.altitude(thrust / rocket.weight());

		// gravity
		if(rocket.pos.altitude() > 0.0)
			environment::gravity(rocket.pos, rocket.acc);

		// update
		rocket.update(dt);
		step++;

		// TODO save to file

		// log
		if(step % output_rate == 0){
			std::cout << rocket.time << " " << rocket.pos.altitude() << std::endl;
		}

		if(step > 100 && rocket.pos.altitude() <= 0.0)
			break;

		if(rocket.time > timeout)
			break;
	}
}
