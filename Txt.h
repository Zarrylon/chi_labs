#pragma once

#include <iostream>
#include <string>
#include <fstream>

using std::string;
using std::fstream;

namespace l1
{
	class Txt
	{
	private:
		size_t file_size = 0;
		string* file = nullptr;

	public:
		Txt() = default;
		Txt(const char*);
		~Txt();

		// copy
		Txt(const Txt&);
		Txt& operator=(const Txt&);

		// move
		Txt(Txt&&) noexcept;
		Txt& operator=(Txt&&) noexcept;

		size_t size() const;
	};
}