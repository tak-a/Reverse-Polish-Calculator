//verification.h
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <tuple>

bool isStringNegative(std::string strVal);

void verifExprValide(std::vector<std::string> expr);

int isItFunction(std::vector<std::string> expression, std::vector<std::tuple<std::string, int>> *functions);

std::vector<std::string> splitAny(std::string expression, char delim);
