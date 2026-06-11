#include <stdio.h>
#include <ctype.h>

int err;

void unexpected(char c)
{
	err = 1;
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return 1;
	}
	return 0;
}

int parse_expr(char **s);

int parse_factor(char **s)
{
	int val;

	if (isdigit(**s))
	{
		val = **s - '0';
		(*s)++;
		return val;
	}
	if (accept(s, '('))
	{
		val = parse_expr(s);
		if (err)
			return 0;
		if (!accept(s, ')'))
			unexpected(**s);
		return val;
	}
	unexpected(**s);
	return 0;
}

int parse_term(char **s)
{
	int val = parse_factor(s);

	while (!err && accept(s, '*'))
		val *= parse_factor(s);
	return val;
}

int parse_expr(char **s)
{
	int val = parse_term(s);

	while (!err && accept(s, '+'))
		val += parse_term(s);
	return val;
}

int main(int argc, char **argv)
{	
	char *s;
	int result;

	if (argc != 2)
		return 1;
	s = argv[1];
	result = parse_expr(&s);
	if (!err && *s)
		unexpected(*s);
	if (err)
		return 1;
	printf("%d\n", result);
	return 0;
}
