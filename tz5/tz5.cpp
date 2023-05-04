#include <iostream>
#include "Matrix.h"

int main()
{
	MatrixI m(3, 3);	// Implicit instantiation
	MatrixF f;			// Explicit instantiation
	MatrixS s;			// Explicit Specialization 

	m[0][0] = 1;
	m[0][1] = -1;
	m[0][2] = 2;
	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = -1;
	m[2][0] = 1;
	m[2][1] = 1;
	m[2][2] = 1;

	//std::cin >> m;
	std::cout << "m:" << std::endl << m;
	
	// Rank of constant
	const Matrix mc = m;
	std::cout << std::endl;
	//std::cout << "Rank of constant m: " << m.calculateRank() << std::endl; => error
	std::cout << "Rank of constant m: " << mc.calculateRank() << std::endl;

	// copy constructor
	Matrix m1(m);
	std::cout << std::endl;
	std::cout << "m1:" << std::endl << m1;

	// move constructor
	Matrix m2(std::move(m));
	std::cout << std::endl;
	std::cout << "m2:" << std::endl << m2;

	// copy operator= 
	Matrix m3(2, 2);
	m3 = m1;
	std::cout << std::endl;
	std::cout << "m3:" << std::endl << m3;

	// move operator=
	Matrix m4(2, 2);
	m4 = std::move(m2);
	std::cout << std::endl;
	std::cout << "m4:" << std::endl << m4;

	// operator-
	m3 = m4 - m3;
	std::cout << std::endl;
	std::cout << "m3(sub):" << std::endl << m3;

	// operator+
	m4 = m4 + m1;
	std::cout << std::endl;
	std::cout << "m4(sum):" << std::endl << m4;

	// sort
	m4.sortColumns(std::greater<int>());
	std::cout << "Sorted(COL):" << std::endl << m4;
	m4.sortRows(std::less<int>());
	std::cout << "Sorted(COL+ROW): " << std::endl << m4;
	std::cout << std::endl;

	MatrixS s1(5, 5);

	s1[0][0] = "he";
	s1[0][1] = "wo";
	std::cout << s1;

	std::cout << std::endl;

	Matrix s2 = s1;
	s2[0][1] = "h";
	std::cout << s2;

	std::cout << std::endl;

	// Resize
	s2.Resize(2, 2);
	std::cout << s2;

	std::cout << std::endl;
}
