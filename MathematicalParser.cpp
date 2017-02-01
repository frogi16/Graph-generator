#include "MathematicalParser.h"

MathematicalParser::MathematicalParser()
{
	
}

void MathematicalParser::setVariables()
{
	symbol_table.add_variable("x", x);
	symbol_table.add_variable("y", y);
	symbol_table.add_variable("P", P);

	symbol_table.add_constants();
	expression.register_symbol_table(symbol_table);
}

void MathematicalParser::update(double gx, double gy)
{
	x = gx;
	y = gy;
	P = pow((10 / 9.0f), (pow(x, 2) + pow(y, 2)) / 16);
}

void MathematicalParser::parse()
{
	parser.compile(calculation, expression);
}

double MathematicalParser::getValue()
{
	return expression.value();
}
