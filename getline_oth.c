#include "shell.h"

/**
 * input_buf - buffers chained
 * @info: struct
 * @buf: address
 * @len: address of var
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0';
				r--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			if (_strchr(*buf, ';'))
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line
 * @info: struct
 * Return: read bytes
 */
ssize_t get_input(info_t *info)
{
	static char *buf = NULL;
	static size_t len = 0;
	static int i = 0;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1)
		return (-1);

	if (len)
	{
		p = buf + i;
		check_chain(info, buf, &i, 0, len);

		if (i >= len)
		{
			i = len = 0;
			info->cmd_buf_type = CMD_NORM;
			info->cmd_buf = NULL;
		}

		*buf_p = p;
		return (_strlen(p));
	}

	*buf_p = buf;
	return (r);
}

/**
 * read_buf - read the buffer
 * @info: struct
 * @buf: buffer
 * @j: size
 * Return: ssize
 */
ssize_t read_buf(info_t *info, char *buf, size_t *j)
{
	ssize_t r = 0;

	if (*j)
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*j = r;
	return (r);
}

/**
 * _getline - get the next line
 * @info: struct
 * @ptr: address of a pointer
 * @l: the size
 * Return: ssize
 */
int _getline(info_t *info, char **ptr, size_t *l)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && l)
		s = *l;
	if (i == len)
		i = len = 0;
	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);
	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p)
		return (p ? free(p), -1 : -1);
	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);
	s += k - i;
	i = k;
	p = new_p;
	if (l)
		*l = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - it stops the Ctrl-C press
 * @sig_num: number
 * Return: nothing
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
