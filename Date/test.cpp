#include <iostream>
#include <ctime>
#include <ostream>
#include "Date.h"

using namespace std;

int main()
{
	project::Date d{ "12/01/2023" };
	project::Date d1{ "27/10/2019" };

	cout << project::Date::random_date() << '\n';

	cout << "Gecen gun: " << (d - d1) << '\n';

	d1 = 100 + d1;

	cout << d1 << '\n';

	cout << ++d1 << '\n';

	project::Date d2 = d1;

	cout << boolalpha << (d1 == d2) << '\n';
}