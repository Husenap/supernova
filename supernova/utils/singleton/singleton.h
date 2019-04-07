#pragma once

template <class T>
class singleton {
public:
	inline static T& get() {
		static T our_instance;
		return our_instance;
	}
};
