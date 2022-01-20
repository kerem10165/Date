#include <iostream>
#include <iomanip>
#include <sstream>
#include<ctime>
#include <chrono>
#include <ostream>
#include "Date.h"

using namespace std;

int main()
{
	project::Date d{"12/01/2023"};
	project::Date d1{ "27/10/2019" };
	cout << project::Date::random_date() << endl;

	project::Date::Weekday w = project::Date::Weekday::Sunday;

	cout << (d += 10) << " " << "\n";

}