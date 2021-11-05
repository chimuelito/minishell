#include "execution.h"

int	cd_with_args(t_command *cmd)
{
	struct stat	stat_ret;

	if (find_key_index(g_global.envp, "OLDPWD") != -1)
		replace_pwd(OLDPWD);
	stat(cmd->args[0], &stat_ret);
	if (!S_ISDIR(stat_ret.st_mode))
		return (ret_msg("Directory doesn't exist\n", 0));
	chdir(cmd->args[0]);
	if (find_key_index(g_global.envp, "PWD") != -1)
		replace_pwd(PWD);
	return (1);
}

int	cd(t_command *cmd)
{
	int			home_index;
	char		**split;

	if (cmd->number_args > 0)
	{
		if (cd_with_args(cmd) == 0)
			return (0);
	}
	else
	{
		if (find_key_index(g_global.envp, "OLDPWD") != -1)
			replace_pwd(OLDPWD);
		home_index = find_key_index(g_global.envp, "HOME");
		if (home_index != -1)
		{
			split = ft_split(g_global.envp[home_index], "=");
			chdir(split[1]);
			free_splits(split, number_of_split(split));
			if (find_key_index(g_global.envp, "PWD") != -1)
				replace_pwd(PWD);
		}
	}
	g_global.ret = 0;
	return (1);
}
