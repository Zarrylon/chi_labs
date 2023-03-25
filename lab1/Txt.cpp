#include "Txt.h"

namespace l1
{
	Txt::Txt(const char* filename)
	{
		fstream f(filename);
		string line;

		if (f)
		{
			while (std::getline(f, line))
			{
				file_size++;
			}

			file = new string[file_size];

			for (int i = 0; i < file_size; i++)
			{
				std::getline(f, file[i]);
			}
		}
		else
		{
			std::cout << "Empty file" << std::endl;
		}
	}

	Txt::~Txt()
	{
		if(file != nullptr)
			delete[] file;
	}

	Txt::Txt(const Txt& other)
	{
		file_size = other.file_size;
		file = new string[file_size];

		for (int i = 0; i < file_size; i++)
		{
			file[i] = other.file[i];
		}
	}

	Txt& Txt::operator=(const Txt& other)
	{
		file_size = other.file_size;
		
		if (file != nullptr)
		{
			delete[] file;
		}
		
		file = new string[file_size];

		for (int i = 0; i < file_size; i++)
		{
			file[i] = other.file[i];
		}

		return *this;
	}

	Txt::Txt(Txt&& other) noexcept
	{
		file_size = other.file_size;
		file = other.file;

		other.file_size = 0;
		other.file = nullptr;
	}

	Txt& Txt::operator=(Txt&& other) noexcept
	{
		file_size = other.file_size;

		if (file != nullptr)
		{
			delete[] file;
		}

		file = other.file;

		other.file_size = 0;
		other.file = nullptr;

		return *this;
	}

	size_t Txt::size() const
	{
		return file_size;
	}
}