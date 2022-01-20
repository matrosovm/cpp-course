#pragma once

#include <string>

class calculate_exception : public std::exception {};

double calculate(const std::string& expr);