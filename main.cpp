#include "game_parameters.hpp"
#include "game_system.hpp"

using param = Parameters;

int main() {
	GameSystem::start(param::game_width, param::game_height, "GE-LAB", param::time_step);
	return 0;
}