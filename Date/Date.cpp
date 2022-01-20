#include "Date.h"
#include <cstring>
#include <iostream>
#include <ostream>
#include <istream>
#include <string>
#include <stdlib.h>


project::Date::Date() :m_time{ transform(1,1,year_base) }, m_tm{ create_tm(localtime(&m_time)) }
{
	if (m_tm == nullptr)
		throw BadDate();
}


project::Date::Date(int d, int m, int y) : m_time{ transform(d,m,y) }, m_tm{ create_tm(localtime(&m_time)) }
{
	if (m_tm == nullptr)
		throw BadDate();
}

project::Date::Date(const char* p) : m_time{ transform(p) }, m_tm{ create_tm(localtime(&m_time)) }
{
	if (m_tm == nullptr)
		throw BadDate();
}

project::Date::Date(time_t timer) : m_time{ timer }, m_tm{ create_tm(localtime(&m_time)) }
{
	if (m_tm == nullptr)
		throw BadDate();
}


project::Date::Date(Date&& other) noexcept : m_time{ other.m_time }, m_tm{ other.m_tm }
{
	other.m_tm = nullptr;
	other.m_time = -1;
}

project::Date& project::Date::operator=(const Date& other)
{
	m_time = other.m_time;
	*m_tm = *other.m_tm;
	return *this;
}

project::Date& project::Date::operator=(Date&& other) noexcept
{
	m_time = other.m_time;
	m_tm = other.m_tm;

	other.m_time = -1;
	other.m_tm = nullptr;

	return *this;
}

int project::Date::get_month_day() const
{
	return m_tm->tm_mday;
}

int project::Date::get_month() const
{
	return m_tm->tm_mon + 1;
}

int project::Date::get_year() const
{
	return m_tm->tm_year + 1900;
}

int project::Date::get_year_day() const
{
	int day{};
	for (size_t i = 0; i <= m_tm->tm_mon; ++i)
	{
		if (i != m_tm->tm_mon && i != 1)
			day += months[i];
		else if (i != m_tm->tm_mon && i == 1)
		{
			if (isleap(m_tm->tm_year + 1900))
				day += 29;
			else
				day += 28;
		}

		else
			day += m_tm->tm_mday;
	}

	return day;
}

project::Date::Weekday project::Date::get_week_day() const
{
	return Weekday{ static_cast<int>(m_tm->tm_wday) };
}

project::Date& project::Date::set_month_day(int day)
{
	set_general(day, get_month(), get_year());
	return *this;
}

project::Date& project::Date::set_month(int month)
{
	set_general(get_month_day(), month, get_year());
	return *this;
}

project::Date& project::Date::set_year(int year)
{
	set_general(get_month_day(), get_month(), year);
	return *this;
}

project::Date& project::Date::set(int day, int mon, int year)
{
	set_general(day, mon, year);
	return *this;
}

project::Date project::Date::operator-(int day) const
{
	Date d{ *this };
	d.operator_calculate(d.m_tm->tm_mday - day);
	return d;
}

project::Date project::Date::operator+(int day) const
{
	Date d{ *this };
	d.operator_calculate(d.m_tm->tm_mday + day);
	return d;
}

project::Date& project::Date::operator+=(int day)
{
	operator_calculate(m_tm->tm_mday + day);
	return *this;
}

project::Date& project::Date::operator-=(int day)
{
	operator_calculate(m_tm->tm_mday - day);
	return *this;
}

project::Date& project::Date::operator++()
{
	operator_calculate(m_tm->tm_mday + 1);
	return *this;
}

project::Date project::Date::operator++(int)
{
	Date d{ *this };
	operator_calculate(d.m_tm->tm_mday + 1);
	return d;
}

project::Date& project::Date::operator--()
{
	operator_calculate(m_tm->tm_mday - 1);
	return *this;
}

project::Date project::Date::operator--(int)
{
	Date d{ *this };
	operator_calculate(d.m_tm->tm_mday - 1);
	return d;
}

project::Date project::Date::random_date()
{
	srand(static_cast<unsigned int>(time(NULL)));
	int y = (rand() % ((random_max_year - random_min_year +1))) + random_min_year;
	int m = (rand() % 12) + 1;
	int d{};

	if (m != 2)
		d = (rand() % months[m - 1]) + 1;

	else
		d = isleap(y) ? (rand() % 29) + 1 : (rand() % 28) + 1;


	return Date(d,m,y);
}


std::time_t project::Date::transform(int dd, int mm, int yy)
{
	if (!(isvalid_year(yy) && isvalid_month(mm), isvalid_day(dd, mm, yy)))
		throw BadDate();

	std::tm t{};
	t.tm_hour = 12;
	t.tm_year = yy - 1900;
	t.tm_mon = mm - 1;
	t.tm_mday = dd;
	return mktime(&t);
}

std::time_t project::Date::transform(const char* p)
{
	char* copy = new char[strlen(p) + 1];

	strcpy(copy, p);

	char* token = nullptr;

	token = strtok(copy, "/");

	int ddyymm[3]{};
	int i = 0;
	while (token != nullptr) {
		ddyymm[i++] = atoi(token);

		token = strtok(nullptr, "/");
	}

	delete[] copy;

	return transform(ddyymm[0], ddyymm[1], ddyymm[2]);
}

std::tm* project::Date::create_tm(const std::tm* p)
{
	std::tm* ptr = nullptr;
	if (p != nullptr)
		ptr = new std::tm(*p);

	return ptr;
}

void project::Date::set_general(int day, int mon, int year)
{
	m_time = transform(day, mon, year);
	delete m_tm;
	m_tm = create_tm(localtime(&m_time));
}

void project::Date::operator_calculate(int day)
{
	m_tm->tm_mday = day;
	m_time = mktime(m_tm);
	std::tm* temp = localtime(&m_time);
	if (temp == nullptr)
		throw BadDate();
	else
		*m_tm = *temp;
}

bool project::Date::isvalid_day(int dd, int mm, int yy)
{
	if (mm != 2)
		return dd >= 1 && dd <= months[mm - 1];

	int day{ months[mm - 1] };
	if (isleap(yy))
		day++;
	return dd >= 1 && dd <= day;
}

bool project::Date::isvalid_month(int mm)
{
	if (mm < 1 || mm > 12)
		return false;

	return true;
}

bool project::Date::isvalid_year(int yy)
{
	if (yy < year_base)
		return false;

	return true;
}

int project::operator-(const Date& d1, const Date& d2)
{
	int d2_yy = d2.get_year(), d1_yy = d1.get_year();
	int d2_dd = d2.get_year_day() , d1_dd = d1.get_year_day();

	if (d2_yy == d1_yy)
	{
		return d1_dd - d2_dd;
	}

	int days{};

	for (int i = d2_yy > d1_yy ? d2_yy - 1 : d1_yy - 1; i >= (d2_yy < d1_yy ? d2_yy + 1 : d1_yy + 1); i--)
	{
		days += Date::isleap(i) ? 366 : 365;
	}

	if (d2_yy > d1_yy)
	{
		days += d2_dd;
		days += Date::isleap(d1_yy) ? 366 - d1_dd : 365 - d1_dd;
		return -days;
	}
	
	else
	{
		days += d1_dd;
		days += Date::isleap(d2_yy) ? 366 - d2_dd : 365 - d2_dd;
		return days;
	}
}

project::Date project::operator+(int n, const Date& date)
{
	return date + n;
}

project::Date::Weekday& project::operator++(Date::Weekday& r)
{
	switch (r)
	{
	case Date::Weekday::Sunday:
		r = Date::Weekday::Monday;
		break;
	case Date::Weekday::Monday:
		r = Date::Weekday::Tuesday;
		break;
	case Date::Weekday::Tuesday:
		r = Date::Weekday::Wednesday;
		break;
	case Date::Weekday::Wednesday:
		r = Date::Weekday::Thursday;
		break;
	case Date::Weekday::Thursday:
		r = Date::Weekday::Friday;
		break;
	case Date::Weekday::Friday:
		r = Date::Weekday::Saturday;
		break;
	case Date::Weekday::Saturday:
		r = Date::Weekday::Sunday;
		break;
	}

	return r;
}

project::Date::Weekday project::operator++(Date::Weekday& r, int)
{
	Date::Weekday t = r;
	++r;
	return t;
}

project::Date::Weekday& project::operator--(Date::Weekday& r)
{
	switch (r)
	{
	case Date::Weekday::Sunday:
		r = Date::Weekday::Saturday;
		break;
	case Date::Weekday::Monday:
		r = Date::Weekday::Sunday;
		break;
	case Date::Weekday::Tuesday:
		r = Date::Weekday::Monday;
		break;
	case Date::Weekday::Wednesday:
		r = Date::Weekday::Tuesday;
		break;
	case Date::Weekday::Thursday:
		r = Date::Weekday::Wednesday;
		break;
	case Date::Weekday::Friday:
		r = Date::Weekday::Thursday;
		break;
	case Date::Weekday::Saturday:
		r = Date::Weekday::Friday;
		break;
	}

	return r;
}

project::Date::Weekday project::operator--(Date::Weekday& r, int)
{
	Date::Weekday t = r;
	--r;
	return t;
}

bool project::operator<(const Date& lhs, const Date& rhs)
{
	return (lhs - rhs) < 0;
}

bool project::operator<=(const Date& lhs, const Date& rhs)
{
	return !(rhs < lhs);
}

bool project::operator>(const Date& lhs, const Date& rhs)
{
	return rhs < lhs;
}

bool project::operator>=(const Date& lhs, const Date& rhs)
{
	return !(lhs < rhs);
}

bool project::operator==(const Date& lhs, const Date& rhs)
{
	return (lhs - rhs) == 0;
}

bool project::operator!=(const Date& lhs, const Date& rhs)
{
	return !(lhs == rhs);
}

std::ostream& project::operator<<(std::ostream& os, const Date& date)
{
	return os << "Gun: " << date.day_name[date.m_tm->tm_wday] << " Tarih: " << date.get_month_day() << "/"
		<< date.months_name[date.get_month() - 1] << "/" << date.get_year();
}

std::istream& project::operator>>(std::istream& is, Date& date)
{
	std::string t{};
	getline(is, t);
	date = Date(t.c_str());
	return is;
}