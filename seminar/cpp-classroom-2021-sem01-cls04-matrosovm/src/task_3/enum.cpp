#include <stdexcept>

#include "enum.hpp"

using namespace wdu;

const char* wdu::GetDayOfWeekName(WeekDay w) {
	if (w == WeekDay::MONDAY)
		return "Monday";
	if (w == WeekDay::TUESDAY)
		return "Tuesday";
	if (w == WeekDay::WEDNESDAY)
		return "Wednesday";
	if (w == WeekDay::THURSDAY)
		return "Thursday";
	if (w == WeekDay::FRIDAY)
		return "Friday";
	if (w == WeekDay::SATURDAY)
		return "Saturday";
	return "Sunday";
}

bool wdu::IsWeekend(WeekDay w) {
	return w == WeekDay::SATURDAY || w == WeekDay::SUNDAY;  
}

WeekDay wdu::GetDayOfWeek(size_t year, size_t month, size_t day) {
	bool is_leap;
	if ((year % 400 == 0) || (year % 100 != 0  && year % 4 == 0))
		is_leap = true;

    if (year == 0 || month > 12 || day > 31 || month == 0 || day == 0 || 
    	(month == 2 && ((is_leap && day > 29) || (!is_leap && day > 28))) || 
    	(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11))) {
        throw std::logic_error("Impossible date");
    }
	size_t month_code = (month + 9) % 12;
    size_t year_code = year - month_code / 10;
    size_t tmp = (365 * year_code + year_code / 4 - 
    				year_code / 100 + year_code / 400 + 
    				(month_code * 306 + 5) / 10 + (day - 1)) % 7;
    if (tmp < 5)
        return static_cast<WeekDay>(tmp + 3);
    return static_cast<WeekDay>(tmp - 4);	
}
