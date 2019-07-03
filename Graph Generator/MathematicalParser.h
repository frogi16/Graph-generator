#pragma once
#include <string>
#include <exprtk.hpp>
#include <vector>

class MathematicalParser
{
public:
	MathematicalParser();
	void setCalculation(const std::string operation) { calculation = operation; setVariables(); }
	void update(double gx, double gy, double gP);
	bool parse();
	double getValue();
private:
	void setVariables();
	double x, y, r, theta, P;
	std::string calculation;
	exprtk::symbol_table<double> symbol_table;
	exprtk::expression<double> expression, pExpression;
	exprtk::parser<double> parser;
};

