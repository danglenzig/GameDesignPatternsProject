#pragma once


class Tester
{
private:
	Tester();

	Tester(const Tester&) = delete;
	Tester& operator=(const Tester&) = delete;

public:
	static Tester& Instance()
	{
		static Tester instance;
		return instance;
	}
};