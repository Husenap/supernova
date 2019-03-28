#include "gtest/gtest.h"

#include "../precompiled.h"

class singleton_mock : public singleton<singleton_mock> {
public:
	int func(){ return 42; }

	inline static int get_val() { return get().m_val; }
private:
	int m_val = 666;
};



TEST(singleton, get){
	auto& obj = singleton_mock::get();

	EXPECT_NE(&obj, nullptr);
}

TEST(singleton, function_call){
	auto& obj = singleton_mock::get();
	EXPECT_EQ(obj.func(), 42);
}

TEST(singleton, getter){
	EXPECT_EQ(singleton_mock::get_val(), 666);
}
