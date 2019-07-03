#include "MathematicalParser.h"

MathematicalParser::MathematicalParser()
{
	
}

void MathematicalParser::setVariables()
{
	symbol_table.add_variable("x", x);
	symbol_table.add_variable("y", y);
	symbol_table.add_variable("r", r);
	symbol_table.add_variable("theta", theta);
	symbol_table.add_variable("P", P);

	symbol_table.add_constants();
	expression.register_symbol_table(symbol_table);
}

void MathematicalParser::update(double gx, double gy, double gP)
{
	x = gx;
	y = gy;
	r = hypot(x, y);
	theta = atan2(y, x);
	P = gP;
}

bool MathematicalParser::parse()
{
	return parser.compile(calculation, expression);
}

double MathematicalParser::getValue()
{
	return expression.value();
}
