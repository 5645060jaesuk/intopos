#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_STACK_SIZE 100

typedef char element;
typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;

void init_stack(StackType* s)
{
	s->top = -1;
}

int is_empty(StackType* s)
{
	return(s->top == -1);
}

int is_full(StackType* s)
{
	return(s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item)
{
	if (is_full(s))
	{
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		return;
	}
	else return s->data[(s->top)--];
}

element peek(StackType* s)
{
	if (is_empty(s))
	{
		fprintf(stderr, "스택 공백 에러\n");
		return;
	}
	else return s->data[s->top];
}

int eval(char exp[])
{
	int op1, op2, value, i = 0;
	int len = strlen(exp);
	char ch;
	StackType s;

	init_stack(&s);
	for (i = 0; i < len; i++)
	{
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != '(' && ch!= ')' && ch!= '[' && ch!=']' && ch !='{' && ch !='}')
		{
			if (isdigit(ch))
			{
				value = ch - '0';
				push(&s, value);
			}
			else
			{
				printf("invalid character");
				exit(0);
			}


		}
		else
		{
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch)
			{
			case '+':push(&s, op1 + op2); break;
			case '-':push(&s, op1 - op2); break;
			case '*':push(&s, op1 * op2); break;
			case '/':push(&s, op1 / op2); break;

			}
		}
	}
	return pop(&s);
}

int prec(char op)
{
	switch (op)
	{
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

int check_matching(const char* in)
{
	StackType s;
	char ch, open_ch;
	int i, n = strlen(in);
	init_stack(&s);

	for (i = 0; i < n; i++)
	{
		ch = in[i];
		switch (ch)
		{
		case '(': case '[': case '{':
			push(&s, ch);
			break;
		case ')': case ']': case '}':
			if (is_empty(&s)) return 0;
			else {
				open_ch = pop(&s);
				if ((open_ch == '(' && ch != ')') ||
					(open_ch == '[' && ch != ']') ||
					(open_ch == '{' && ch != '}')) {
					return 0;
				}
				break;
			}
		}
	}
	if (!is_empty(&s)) return 0;
	return 1;
}

void infix_to_posfix(char exp[], char k[])
{
	int i = 0;
	char ch, top_op;
	int len = strlen(exp);
	StackType s;
	int j = 0;

	init_stack(&s);

	for (i = 0; i < len; i++)
	{

		ch = exp[i];

		if (exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/')
		{
			printf("Invalid Expression");
			exit(0);
		}


		switch (ch)
		{
		case '+': case '-': case '*': case '/':
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
				k[j++] = pop(&s);
			push(&s, ch);
			break;
		case '(':
			push(&s, ch);
			break;
		case ')':
			top_op = pop(&s);
			while (top_op != '(')
			{
				k[j++] = top_op;
				top_op = pop(&s);
			}
			break;
		default:
			k[j++] = ch;
			break;
		}
	}

	while (!is_empty(&s))
		k[j++] = pop(&s);

	k[j] = '\0'; // 문자열의 끝을 나타내는 NULL 문자 삽입
}
 

int main(void)
{
	int result;
	char pos[100];
	char re[100];
	scanf_s("%s", pos, 100);
	if (check_matching(pos) != 1)
	{
		printf("Error : Mismatched parentheses");
		exit(0);
	}	
	infix_to_posfix(pos,re);
	printf("후위수식");
	printf(re);
	printf("\n");
	result = eval(re);
	printf("%d", result);
	printf("\n");
	return 0;
}