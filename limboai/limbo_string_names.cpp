/* limbo_string_names.cpp */

#include "limbo_string_names.h"
#include "core/string_name.h"

LimboStringNames *LimboStringNames::singleton = nullptr;

LimboStringNames::LimboStringNames() {
	_generate_name = StaticCString::create("_generate_name");
	_setup = StaticCString::create("_setup");
	_enter = StaticCString::create("_enter");
	_exit = StaticCString::create("_exit");
	_tick = StaticCString::create("_tick");
	behavior_tree_finished = StaticCString::create("behavior_tree_finished");
}