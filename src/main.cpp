#include <iostream>
#include <string>
#include "rocket.hpp"

using math::Float;

Rocket rocket;

auto do_simulation(const Float &dt, const Float &timeout) -> void;

auto main(int argc, char **argv) -> int {
	std::string engine_file;

	std::cout << "rocket simulator by sksat" << std::endl;

	std::cout << "engine data file: ";
	std::cin >> engine_file;

	rocket.engine.load_data(engine_file);

	// thrust test
	for(double t=0.0;t<10.0;t+=0.1){
		std::cout << t << "\t" << rocket.engine.thrust(t) << std::endl;
	}

	std::cout << "start simulation" << std::endl;
	do_simulation(0.01, 60.0*10);

	return 0;
}

void do_simulation(const math::Float &dt, const math::Float &timeout){
	Float time = 0.0;

	// init
	auto &pos = rocket.pos;
	pos.altitude(0.0);
	pos.east(0.0);
	pos.north(0.0);
	rocket.vel.vec << 0.0, 0.0, 0.0;
	rocket.acc.vec << 0.0, 0.0, 0.0;

	// main loop
	for(;time<=timeout;time+=dt){
		// update
		rocket.update(dt);

		// TODO save to file
	}
}
