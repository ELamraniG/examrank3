#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int match_space(FILE *f)
{
        // You may insert code here
	char c = fgetc(f);
	while (!isspace(c))
	{
		if (c == EOF)
			return -1;
		c = fgetc(f);
		if (c == EOF)
			return -1;
	}
	ungetc(c, f);
	return (1);
}

int match_char(FILE *f, char c)
{
        // You may insert code here
	return (c == fgetc(f));


    return (-1);
}

int scan_char(FILE *f, va_list ap)
{
        // You may insert code here
	char c = fgetc(f);
	if (c == EOF)
		return -1;
	*(va_arg(ap, int *)) = c;
	return 1;
    return (0);
}

int scan_int(FILE *f, va_list ap)
{
        // You may insert code here
	long i = 0;
	int counter = 0;
	int *arg = va_arg(ap, int*);
	char c = fgetc(f);
	while (c >= '0' && c <= '9')
	{
		counter++;
		i = (i * 10) + c - '0';
		c = fgetc(f);
	}
	*arg = i;
	return counter;
    return (0);
}

int scan_string(FILE *f, va_list ap)
{
        // You may insert code here

	char c;
	char *str = (char *)va_arg(ap, int *);
	int counter = 0;
	c = fgetc(f);
	while (c != EOF && c != '\n')
	{
		counter++;
		*str = c;
		str++;
		c = fgetc(f);
	}
	*str = 0;
	return counter;
    return (0);
}


int	match_conv(FILE *f, const char **format, va_list ap)
{
	switch (**format)
	{
		case 'c':
			return scan_char(f, ap);
		case 'd':
			match_space(f);
			return scan_int(f, ap);
		case 's':
			match_space(f);
			return scan_string(f, ap);
		case EOF:
			return -1;
		default:
			return -1;
	}
}

int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int nconv = 0;

	int c = fgetc(f);
	if (c == EOF)
		return EOF;
	ungetc(c, f);

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (match_conv(f, &format, ap) != 1)
				break;
			else
				nconv++;
		}
		else if (isspace(*format))
		{
			if (match_space(f) == -1)
				break;
		}
		else if (match_char(f, *format) != 1)
			break;
		format++;
	}
	if (ferror(f))
		return EOF;
	return nconv;
}


int ft_scanf(const char *format, ...)
{
	// ...
	va_list ap;
	va_start(ap, format);
	int ret = ft_vfscanf(stdin, format, ap);
	// ...
	return ret;
}