#include <cassert>
#include <cstring>
#include "enum.hpp"

using namespace wdu;

int main() {
  assert(!std::strcmp(GetDayOfWeekName(WeekDay::MONDAY), "Monday"));
  assert(!std::strcmp(GetDayOfWeekName(static_cast<WeekDay>(2)), "Tuesday"));
  assert(!std::strcmp(GetDayOfWeekName(WeekDay::WEDNESDAY), "Wednesday"));
  assert(!std::strcmp(GetDayOfWeekName(static_cast<WeekDay>(4)), "Thursday"));
  assert(!std::strcmp(GetDayOfWeekName(static_cast<WeekDay>(5)), "Friday"));
  assert(!std::strcmp(GetDayOfWeekName(static_cast<WeekDay>(6)), "Saturday"));
  assert(!std::strcmp(GetDayOfWeekName(static_cast<WeekDay>(7)), "Sunday"));

  assert(!IsWeekend(WeekDay::THURSDAY));
  assert(IsWeekend(WeekDay::SATURDAY));

  assert(GetDayOfWeek(2020, 10, 13) == WeekDay::TUESDAY);
  try {
    WeekDay impossible_day = GetDayOfWeek(2020, 2, 30);
    assert(false);
  } catch (const std::logic_error& e) {
    assert(!std::strcmp(e.what(), "Impossible date"));
  }

  return 0;
}
