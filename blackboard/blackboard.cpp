/**
 * blackboard.cpp
 * =============================================================================
 * Copyright 2021-2023 Serhii Snitsaruk
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#include "blackboard.h"

#ifdef LIMBOAI_MODULE
#include "core/error/error_macros.h"
#include "core/variant/variant.h"
#include "scene/main/node.h"
#endif // LIMBOAI_MODULE

#ifdef LIMBOAI_GDEXTENSION
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>
using namespace godot;
#endif

Ref<Blackboard> Blackboard::top() const {
	Ref<Blackboard> bb(this);
	while (bb->get_parent_scope().is_valid()) {
		bb = bb->get_parent_scope();
	}
	return bb;
}

Variant Blackboard::get_var(const String &p_name, const Variant &p_default) const {
	if (data.has(p_name)) {
		return data.get(p_name).get_value();
	} else if (parent.is_valid()) {
		return parent->get_var(p_name, p_default);
	} else {
		return p_default;
	}
}

void Blackboard::set_var(const String &p_name, const Variant &p_value) {
	// TODO: Check if p_value can be converted into required type!
	if (data.has(p_name)) {
		data[p_name].set_value(p_value);
	} else {
		BBVariable var;
		var.set_value(p_value);
		data.insert(p_name, var);
	}
}

bool Blackboard::has_var(const String &p_name) const {
	return data.has(p_name) || (parent.is_valid() && parent->has_var(p_name));
}

void Blackboard::erase_var(const String &p_name) {
	data.erase(p_name);
}

void Blackboard::add_var(const String &p_name, const BBVariable &p_var) {
	ERR_FAIL_COND(data.has(p_name));
	data.insert(p_name, p_var);
}

void Blackboard::prefetch_nodepath_vars(Node *p_node) {
	ERR_FAIL_COND(p_node == nullptr);
	for (KeyValue<String, BBVariable> &kv : data) {
		if (kv.value.get_value().get_type() == Variant::NODE_PATH) {
			Node *fetched_node = p_node->get_node_or_null(kv.value.get_value());
			if (fetched_node != nullptr) {
				kv.value.set_value(fetched_node);
			}
		}
	}
}

void Blackboard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_var", "p_name", "p_default"), &Blackboard::get_var, Variant());
	ClassDB::bind_method(D_METHOD("set_var", "p_name", "p_value"), &Blackboard::set_var);
	ClassDB::bind_method(D_METHOD("has_var", "p_name"), &Blackboard::has_var);
	ClassDB::bind_method(D_METHOD("set_parent_scope", "p_blackboard"), &Blackboard::set_parent_scope);
	ClassDB::bind_method(D_METHOD("get_parent_scope"), &Blackboard::get_parent_scope);
	ClassDB::bind_method(D_METHOD("erase_var", "p_name"), &Blackboard::erase_var);
	ClassDB::bind_method(D_METHOD("prefetch_nodepath_vars", "p_node"), &Blackboard::prefetch_nodepath_vars);
	ClassDB::bind_method(D_METHOD("top"), &Blackboard::top);
	// ClassDB::bind_method(D_METHOD("get_data"), &Blackboard::get_data);
	// ClassDB::bind_method(D_METHOD("set_data", "p_data"), &Blackboard::set_data);
}
