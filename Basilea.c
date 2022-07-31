/*
 *										   AVE · IESV · CHRISTE
 *
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *								  ######################################
 *								  ######################################
 *								  ######################################
 *								  ######################################
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *												 ########
 *
 *									   In Honorem Mathematici Euleri, 
 *                                 pius Christianus summae intelligentiae
 *                                ad quosvis problemates logicos solvendum
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

inline static size_t count_char(const char* subject, char target) {
	size_t count = 0;
	for(size_t x = 0; x < strlen(subject); ++x)
		if(subject[x] == target) ++count;
		
	return count;
}

// Returns true if the string is a fully qualified long double
static bool isdouble(const char* str) {
	bool found_period = false;
	for(size_t x = 0; x < strlen(str); ++x) {
		if(str[x] == '.') {
			if(found_period) return false; // More than one period in the string
			found_period = true;
		}
		else if((str[x] == '-' || str[x] == '+') && x == 0) continue;
		else if(!isdigit(str[x])) return false;
	}
	return true;
}

#define FUNCS_COUNT 20
#define LONGEST_FUNC 5
static const char functions[FUNCS_COUNT][LONGEST_FUNC] = {"sin", "asin", "cos", "acos", "tan", "atan", "cosh",
														"sinh", "tanh", "acosh", "asinh", "atanh", "ln", "log",
														"sqrt", "cbrt", "round", "ceil", "abs", "floor"};
				
// Looks for functions in the term and computes them (example: "sqrt 100" turns into 10)
static long double parse_functions(const char* term) {
	if(strlen(term) == 0) return 0;
	if(isdouble(term)) return strtold(term, NULL);
	
	char* term_func = calloc(LONGEST_FUNC, 1);
	
	long double term_number = 0, result;
	
	// Throughout this function, we will be ignoring the negative sign at the beginning of the expression
	bool is_negative = term[0] == '-';
	
	// Gets the function in the term (example: "acos" in "acos-48")
	for(int x = 0; x < FUNCS_COUNT; ++x) {
		if(strlen(term) < strlen(functions[x])) continue;

		if(is_negative ? !strncmp(&term[1], functions[x], strlen(functions[x])) : !strncmp(term, functions[x], strlen(functions[x]))) {
			strcpy(term_func, functions[x]);
			break;
		}
	}

	// Parses constants
	const char* term_number_str = is_negative ? &term[strlen(term_func)+1] : &term[strlen(term_func)];
	if(strcmp(term_number_str, "PI") == 0)
		term_number = M_PI;
		
	else if(strcmp(term_number_str, "E") == 0)
		term_number = M_E;
	
	else
		term_number = strtold(term_number_str, NULL);
	
	
	// Calculates the result
	if(strlen(term_func) == 0)
		result = term_number;
		
	else if(strcmp(term_func, "sin") == 0)
		result = sinl(term_number);
		
	else if(strcmp(term_func, "asin") == 0)
		result = asinl(term_number);
		
	else if(strcmp(term_func, "cos") == 0)
		result = cosl(term_number);
		
	else if(strcmp(term_func, "acos") == 0)
		result = acosl(term_number);
		
	else if(strcmp(term_func, "tan") == 0)
		result = tanl(term_number);
		
	else if(strcmp(term_func, "atan") == 0)
		result = atanl(term_number);
		
	else if(strcmp(term_func, "cosh") == 0)
		result = coshl(term_number);
		
	else if(strcmp(term_func, "sinh") == 0)
		result = sinhl(term_number);
		
	else if(strcmp(term_func, "tanh") == 0)
		result = tanhl(term_number);
		
	else if(strcmp(term_func, "acosh") == 0)
		result = acoshl(term_number);
		
	else if(strcmp(term_func, "asinh") == 0)
		result = asinhl(term_number);
		
	else if(strcmp(term_func, "atanh") == 0)
		result = atanhl(term_number);
		
	else if(strcmp(term_func, "ln") == 0)
		result = logl(term_number);
		
	else if(strcmp(term_func, "log") == 0)
		result = log10l(term_number);
		
	else if(strcmp(term_func, "sqrt") == 0)
		result = sqrtl(term_number);
		
	else if(strcmp(term_func, "cbrt") == 0)
		result = cbrtl(term_number);
		
	else if(strcmp(term_func, "roundl") == 0)
		result = roundl(term_number);
		
	else if(strcmp(term_func, "ceil") == 0)
		result = ceill(term_number);
		
	else if(strcmp(term_func, "abs") == 0)
		result = term_number < 0 ? -1 * term_number : term_number;
		
	else if(strcmp(term_func, "floor") == 0)
		result = floorl(term_number);
		
	else
		result = term_number; // When no function is found, the result is set to the number
	
	result *= is_negative ? -1 : 1;
	
	//free(func_candidate);
	free(term_func);
	
	return result;
}


// The order of the operators matters, as it keeps the hierarchy of mathematical operations
static const char operators[] = "+-^*/";

// Finds the first operator in a un-nested mathematical expression at which the expression should be split to individually calculate
// its component expressions. For instance, the splitting operator in "5*4-10/8" is '-', since we have to first work out "5*4" and "10/8" 
// separately
ssize_t find_splitting_operator(const char* expr) {
	for(int x = 0; x < sizeof(operators); ++x) {
		// Yes, we have to look for the operator reading the string backwards. Otherwise, this happens: 1-1-1-1 = 1-1-0 = 1-1 = 0
		for(size_t y = strlen(expr)-1; y > 0; --y) {
			if(expr[y] == operators[x]) {
				// If we encounter expressions such as 5*-2, we want to split the expression at the '*', NOT at the '-' since in this case, 
				// instead of working as an operator among two numbers it's more of an "attribute" of one of them, if you will; splitting in 
				// the '-' will lead to a broken expression
				if(expr[y] == '-') {
					if(islower(expr[y-1])) // Don't split expressions like cos-3
						continue;
					
					if(expr[y-1] == '-' || expr[y-1] == '^' || expr[y-1] == '*' || expr[y-1] == '/')
						continue;
					
					return y;
				}
				if(expr[y] == '+') {
					if(islower(expr[y-1])) // Don't split expressions like sin-3
						continue;	
					
					if(expr[y-1] == '+' || expr[y-1] == '-' || expr[y-1] == '^' || expr[y-1] == '*' || expr[y-1] == '/')
						continue;
					
					return y;
				}
				return y;
			}
		}
	}
	return -1;
}

// This method by itself cannot do substractions nor interpret constants
long double compute(const char* expr) {
	//printf("Computing '%s'\n", expr);
	
	// Looks for brackets in the expression
	ssize_t continue_count = 0, f_bracket = -1, l_bracket = -1;
	bool is_f_bracket = true;
	
	for(size_t x = 0; x < strlen(expr); ++x) {
		if(expr[x] == '(') {
			if(is_f_bracket) {
				f_bracket = x;
				is_f_bracket = false;
			}
			else ++continue_count;
		}
		else if(expr[x] == ')') {
			if(continue_count == 0) {
				l_bracket = x;
				break;
			}
			--continue_count;
		}
	}
	
	// If present, compute the sub-expression
	if(f_bracket != -1 && l_bracket != -1) {
		char* expr_before_bracket = calloc(f_bracket+1, 1);
		strncpy(expr_before_bracket, expr, f_bracket);
		
		char* expr_after_bracket = calloc(strlen(expr) - l_bracket - 1, 1);
		strncpy(expr_after_bracket, expr + l_bracket + 1, strlen(expr) - l_bracket - 1);
		
		// Gets the sub-expression (delimited by the outermost parentheses)
		char* subexpr = calloc(l_bracket - f_bracket - 1, 1);
		strncpy(subexpr, expr + f_bracket + 1, l_bracket - f_bracket - 1);
		// Computes it
		long double value = compute(subexpr);
		
		/*
		printf("(P)Expression before bracket is: %s\n", expr_before_bracket);
		printf("(P)Expression after bracket is: %s\n", expr_after_bracket);
		printf("(P)Sub-expression is %Lf\n", value); //*/
		// The final processed expression will be this long: length of the expression before the opening bracket + length of the computed
		// expression wrapped by the brackets + length of the expression after the closing bracket
		
		char* processed_expr = calloc(strlen(expr_before_bracket) + snprintf(NULL, 0, "%0.15Lf", value) + strlen(expr_after_bracket), 1);
		sprintf(processed_expr, "%s%0.15Lf%s", expr_before_bracket, value, expr_after_bracket);
		
		free(expr_before_bracket);
		free(expr_after_bracket);
		free(subexpr);

		//printf("(P)Processed expression is: %s\n", processed_expr);
		
		// Pass through compute again just in case there are more parentheses to parse
		value = compute(processed_expr);
		return value;
		free(processed_expr);
	}

	// Finds the first splitting operator inside the expression (e.g. '-' in 5*5-3)
	size_t splitting_op_pos = find_splitting_operator(expr);
	if(splitting_op_pos == -1) return parse_functions(expr); // No operator
	const char splitting_op = expr[splitting_op_pos];
	
	//printf("Splitting operator: %c, located at %lu\n", splitting_op, splitting_op_pos);
	
	// The first term before the operator
	char* first_term_str = calloc(splitting_op_pos, 1);
	strncpy(first_term_str, expr, splitting_op_pos);
	
	// The second term after the operator
	char* second_term_str = calloc(strlen(expr) - splitting_op_pos - 1, 1);
	strncpy(second_term_str, expr + splitting_op_pos + 1, strlen(expr) - splitting_op_pos - 1);
	
	//printf("First term is '%s', second is '%s'\n", expr, first_term_str, second_term_str);
	
	long double first_term = compute(first_term_str), second_term = compute(second_term_str);
	
	free(first_term_str);
	free(second_term_str);
	
	long double value = 0;
	switch(splitting_op) {
		case '*':
			value = first_term * second_term;
			break;
		case '/':
			value = first_term / second_term;
			break;
		case '+':
			value = first_term + second_term;
			break;
		case '-':
			value = first_term - second_term;
			break;
		case '^':
			value = pow(first_term, second_term);
			break;
	}
	return value;
}

// Performs compute() but first stripping the expression clean of whitespaces first (nws = no whitespace) and
// also adding ommitted asterisks ( 4E-2(8^9) becomes 4*E-2*(8^9) )
long double compute_nws(const char* expr) {
	// Calculates the length of the processed expression
	size_t expression_len = 0;
	for(size_t x = 0; x < strlen(expr); ++x) {
		if(expr[x] != ' ') {
			++expression_len;
			if(x != strlen(expr)-1) {
				if(isdigit(expr[x]) && (isalpha(expr[x+1]) || expr[x+1] == '('))
					++expression_len;
			}
		}
	}
	char* expression = calloc(expression_len, 1);
	
	// Removes whitespaces that mess up with the parsing and adds ommitted asterisks
	size_t y = 0;
	for(size_t x = 0; x < strlen(expr); ++x) {
		if(expr[x] != ' ') {
			expression[y++] = expr[x];
			if(x != strlen(expr)-1) {
				if(isdigit(expr[x]) && (isalpha(expr[x+1]) || expr[x+1] == '(')) {
					expression[y++] = '*';
				}
			}
		}
	}
	
	long double val = compute(expression);
	free(expression);
	return val;
}

// Replaces the variable in a function by its value a calculates it
// Example: apply("x^2-sin(x)", 3.424, 'x') returns 3.424^2-sin(3.424) = 12.002444
long double apply_function(const char* function, long double value, char variable) {
	// For whatever reason, dynamic allocation leads to random crashes when calling free()
	static char value_str[256], func_expr[2048];
	memset(value_str, '\0', sizeof(value_str));
	memset(func_expr, '\0', sizeof(func_expr));
	
	// Gets the length of the input value and creates a string with its content
	ssize_t value_len = snprintf(value_str, sizeof(value_str), "%0.15Lf", value);
	//char* value_str = calloc(value_len, 1);
	//sprintf(value_str, "%0.15Lf", value);
	
	// Calculates the buffer length to then initialize it
	/*
	size_t func_expr_len = 0;
	for(size_t x = 0; x < strlen(function); ++x) {
		if(function[x] == variable)
			func_expr_len += value_len-1;
		else if(function[x] != ' ') {
			++func_expr_len;
			if(x != strlen(function)-1) {
				if(isdigit(function[x]) && (isalpha(function[x+1]) || function[x+1] == '('))
					++func_expr_len;
			}
		}
	}*/
	// The final form of the function with all its occurrences of variable replaced by the given value
	//char* func_expr = calloc(func_expr_len, 1);
	
	size_t y = 0;
	for(size_t x = 0; x < strlen(function); ++x) {
		if(function[x] == variable) {
			strcat(func_expr, value_str);
			y += value_len - 1;
		} else if(function[x] != ' ') {
			func_expr[y++] = function[x];
			if(x != strlen(function)-1) {
				if(isdigit(function[x]) && (isalpha(function[x+1]) || function[x+1] == '('))
					func_expr[y++] = '*';
			}
		}
	}

	return compute(func_expr);
	/*
	long double res = compute(func_expr);
	free(value_str); value_str = NULL;
	free(func_expr); func_expr = NULL;
	return res;*/
}

#ifdef EXECUTABLE
int main(int argc, const char* argv[]) {
	char expression[10000];
	if(argc < 2) {
		while(true) {
			printf("Enter an arithmetic expression: ");
			if(!scanf("%s", expression)) {
				printf("Could not read from stdin. You may enter an arithmetic expression as a command line argument.\n");
				return 1;
			}
			printf("ans = %0.15Lf\n", compute_nws(expression));
		}
	} else strcpy(expression, argv[1]);
	
	printf("Calculating %s\n", expression);
	
	printf("ans = %0.15Lf\n", compute_nws(expression));
	
	/*
	const char function[] = "sinx";
	printf("Applying 10000 to %s: ", function);
	printf("%Lf\n", apply_function(function, 10000, 'x'));
	printf("Numeric integral of %s from -100 to 100: %Lf\n", function, numeric_integral(function, -100, 100, 'x'));*/
	return 0;
}
#endif
