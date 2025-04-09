#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int match_space(FILE *f)
{
    char c;
	c = fgetc(f);
	while(isspace(c))
		c = fgetc(f);
	if (c == EOF)
		return -1;
	ungetc(c,f);
    return (1);
}

int match_char(FILE *f, char c)
{
	char p;
	p = fgetc(f);
	if (p == EOF)
		return -1;
	if (p != c)
		return -1;
	
    return (1);
}

int scan_char(FILE *f, va_list ap)
{
    char c = fgetc(f);
	if (c == EOF)
		return -1;
	char *p = va_arg(ap, char *);
	*p = c;
    return (1);
}

int scan_int(FILE *f, va_list ap)
{
	char buffer[100000];
	int i = 0;
	int c = fgetc(f);
	if (c == '-' || c == '+')
	{
		buffer[i++] = c;
		c = fgetc(f);
	}
	if (!isdigit(c))
		return -1;
	while (isdigit(c))
	{
		buffer[i++] = c;
		c = fgetc(f);
	}
	if (c == EOF)
		return -1;
	buffer[i] = 0;
	int *n = va_arg(ap,int *);
	*n = atoi(buffer);
	ungetc(c,f);
    return (1);
}

int scan_string(FILE *f, va_list ap)
{
    char buffer[100000];
	int i = 0;
	char tmp = fgetc(f);
	while (tmp != EOF && !isspace(tmp))
	{
		buffer[i++] = tmp;
		tmp = fgetc(f);
	}
	if (i == 0)
		return -1;
	buffer[i] = 0;
	char *p = va_arg(ap,char *);
	i = 0;
	while (buffer[i])
	{
		p[i] = buffer[i];
		i++;
	}
	buffer[i] = 0;
	ungetc(tmp,f);
    return (1);
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
	{
		return EOF;
	}
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
	va_list ap;
	va_start(ap,format);
	int ret = ft_vfscanf(stdin, format, ap);
	va_end(ap);
	return ret;
}
