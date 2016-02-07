#pragma once

#include "Event.hpp"

#include <chrono>
#include <deque>


namespace exc {


struct EventEntry {
	std::chrono::high_resolution_clock::time_point timestamp;
	Event event;
};


using EventBuffer = std::deque<EventEntry>;


}