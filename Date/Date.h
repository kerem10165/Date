#ifndef DATE_H
#define DATE_H
#include <iosfwd>
#include <iostream>
#include <ctime>

namespace project {
	/*exception handlingten sonra tamamlanacak*/
	class BadDate
	{
	};

	class Date {
	public:
		static constexpr int year_base = 1970;  //time_t min
		static constexpr int random_min_year = 1970;
		static constexpr int random_max_year = 2050;
		enum class Weekday { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday }; //4
		Date();
		Date(int d, int m, int y); 
		Date(const Date& other) : m_time{ other.m_time }, m_tm{ new std::tm(*other.m_tm) }	{}
		Date(Date&& other) noexcept;
		explicit Date(const char* p); 
		explicit Date(time_t timer);
		
		~Date()
		{
			if (m_tm)
				delete m_tm;
		}

		Date& operator=(const Date& other);
		Date& operator=(Date&& other) noexcept;

		int get_month_day()const;
		int get_month()const;
		int get_year()const; 
		int get_year_day()const; 
		Weekday get_week_day()const;

		Date& set_month_day(int day);
		Date& set_month(int month); 
		Date& set_year(int year); 
		Date& set(int day, int mon, int year);
		
		Date operator-(int day)const;
		Date operator+(int day)const;
		Date& operator+=(int day);
		Date& operator-=(int day);
		Date& operator++();
		Date operator++(int);
		Date& operator--();
		Date operator--(int);
		
		friend bool operator<(const Date&, const Date&);
		friend bool operator==(const Date&, const Date&);
		
		static Date random_date();

		friend std::ostream& operator<<(std::ostream&, const Date&);
		friend std::istream& operator>>(std::istream& is, Date& date);
		
		constexpr static bool isleap(const int y)
		{
			if (y % 4 == 0)
			{
				if (y % 100 != 0)
					return true;

				else if (y % 400 == 0)
				{
					if (y % 4000 == 0)
						return false;
					else
						return true;
				}

				else
					return false;
			}

			return false;
		}
	private:
		std::time_t m_time;
		std::tm* m_tm;
		inline static const int months[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		inline static const char* months_name[12] = { "Ocak" , "Subat" , "Mart" , "Nisan" , "Mayis" , "Haziran" 
			, "Temmuz" , "Agustos" , "Eylul" , "Ekim" , "Kasim" , "Aralik"};
		inline static const char* day_name[7] = { "Pazar","Pazartesi","Sali" , "Carsamba" , "Persembe" , "Cuma" , "Cumartesi" };
		/*bu fonksiyon kendisine verilen tarih deðerlerini kontrol eder ve þimdilik hata olduðunda BadDate türünden bir 
		nesne throw eder. eðer tarihler geçerli ise verilen tarihlerle time_t türünden bir nesne döndürür*/
		std::time_t transform(int dd, int mm, int yy);
		/*uygun formattaki stringi split edip std::time_t transform(int dd, int mm, int yy) fonksiyonuna gönderir*/
		std::time_t transform(const char* p);
		std::tm* create_tm(const std::tm* p);
		/*set iþlemleri için kullanýlacak genelleþtirilmiþ fonksiyon*/
		void set_general(int day, int mon, int year);
		/*operator- operator+ gibi fonksiyonlarýn tarih hesaplarý için kullanýldý*/
		void operator_calculate(int day);

		bool isvalid_day(int dd, int mm, int yy);
		bool isvalid_month(int mm);
		bool isvalid_year(int yy);
	};

	bool operator<(const Date&, const Date&); 
	bool operator<=(const Date&, const Date&);
	bool operator>(const Date&, const Date&); 
	bool operator>=(const Date&, const Date&);
	bool operator==(const Date&, const Date&);
	bool operator!=(const Date&, const Date&);

	int operator-(const Date& d1, const Date& d2); 
	Date operator+(int n, const Date&); 
	Date::Weekday& operator++(Date::Weekday& r); 
	Date::Weekday operator++(Date::Weekday& r, int); 
	Date::Weekday& operator--(Date::Weekday& r); 
	Date::Weekday operator--(Date::Weekday& r, int); 
}

#endif