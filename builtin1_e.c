#include "shell.h"

/**
 * _myhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myhistory(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *info, char *str)
{
char *p, c;
int ret;
int index;
int count = 0;

p = _strchr(str, '=');
if (!p)
return (1);

c = *p;
*p = 0;

index = get_node_index(info->alias, node_starts_with
(info->alias, str, -1));

while (count < index)
{
info->alias = info->alias->next;
count++;
}

ret = delete_node_at_index(&(info->alias), index);
*p = c;

return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *info, char *str)
{
char *p = _strchr(str, '=');
int ret;

if (!p)
return (1);

ret = (!*++p) ? unset_alias(info, str) : unset_alias(info, str);
return ((add_node_end(&(info->alias), str, 0) == NULL) ? ret : 0);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
char *p = NULL, *a = NULL;

if (node)
{
p = _strchr(node->str, '=');
a = node->str;

while (a <= p)
{
_putchar(*a);
a++;
}

_putchar('\'');
_puts(p + 1);
_puts("'\n");

return (0);
}

return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias(info_t *info)
{
int i = 0;
char *p = NULL;
list_t *node = NULL;

if (info->argc == 1)
{
node = info->alias;

while (node)
{
print_alias(node);
node = node->next;
}

return (0);
}

i = 1;

while (info->argv[i])
{
p = _strchr(info->argv[i], '=');

if (p)
set_alias(info, info->argv[i]);
else
print_alias(node_starts_with(info->alias, info->argv[i], '='));

i++;
}

return (0);
}