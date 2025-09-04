static bool	check_extension(const char *extension)
{
	size_t	len;
	if (extension == NULL)
		return (false);
	len = ft_strlen(extension);
	if (len < 4)
		return (false);
	if (extension[len - 4] != '.')
		return (false);
	if (extension[len - 3] != 'c')
		return (false);
	if (extension[len - 2] != 'u')
		return (false);
	if (extension[len - 1] != 'b')
		return (false);
	return (true);
}
