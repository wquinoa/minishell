/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalvaro <jalvaro@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/07 05:40:40 by wquinoa           #+#    #+#             */
/*   Updated: 2020/07/17 22:53:19 by jalvaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_shell *shell)
{
	char	**words;
	int		i;
	int		flag;

	words = shell->split;
	flag = 0;
	if (words[1])
	{
		flag = ft_strncmp(words[1], "-n", 3);
		i = (flag == 0);
		while (words[++i])
		{
			ft_putstr_fd(words[i], 1);
			words[i + 1] ? write(1, " ", 1) : 0;
		}
	}
	return (write(1, flag ? "\n" : "", 1));
}

int	cd(t_shell *shell)
{
	char		*res;
	const int	err[8] = {EACCES, EFAULT, EIO, ELOOP, ENAMETOOLONG, ENOENT, ENOTDIR};

	if (!shell->split[1])
		return (write(1, "\n", 1));
	else if (shell->split[1][0] == '~')
		res = ft_find_env(shell->envir, "HOME")->value;
	else
		res = shell->split[1];
	chdir(res);
	if (ft_memchr(err, errno, sizeof(err)))
		perror("cd");
	free(shell->cwd);
	shell->cwd = getcwd(NULL, 42);
	return (0);
}

int	pwd(t_shell *shell)
{
	if (shell->cwd)
		free(shell->cwd);
	shell->cwd = getcwd(NULL, 42);
	ft_putendl_fd(shell->cwd, 1);
	return (0);
}

int	export(void)
{
	return (write(1, "ecport\n", 7));
}

int	unset(void)
{
	return (write(1, "unset\n", 6));
}

int	env(t_shell *shell)
{
	t_env *env;

	env = shell->envir;
	while (env)
	{
		ft_printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	return (0);
}

int	ft_exit(void)
{
	write(1, "exit\n", 5);
	exit(0);
	return (0);
}

void	search(t_shell *shell)
{
	DIR			*dirp;
	t_dirent	*entry;
	int			pid;

	pid = fork();
	wait(NULL);
	if (pid == 0)
	{
		if (ft_isalnum(shell->split[0][0]))
		{
			while (*shell->path)
			{
				dirp = opendir(*shell->path);
				while ((entry = readdir(dirp)))
					if (!ft_strcmp(entry->d_name, shell->split[0]))
						if (!(shell->cmd = ft_strjoin_dlm(*shell->path, "/" ,shell->split[0])))
							return ;
				shell->path++;
				closedir(dirp);
			}
		}
		else if (!ft_strncmp("./", shell->split[0], 2))
		{
			dirp = opendir(shell->cwd);
			while ((entry = readdir(dirp)))
				if (ft_strcmp(entry->d_name, shell->split[0]))
					shell->cmd = ft_strjoin(shell->cwd, &shell->split[0][1]);
			closedir(dirp);
		}
		execve(shell->cmd, &shell->split[0], shell->environ);
		perror(shell->split[0]);
		if (shell->cmd)
			free (shell->cmd);
		shell->cmd = NULL;
		ft_printf("b42h: %s: command not found\n", shell->split[0]);
		exit(1);
	}
}


void	parse_args(char **tab, char *str, t_shell *shell)
{
	static int	(*funcs[8])() = {&echo, &cd, &pwd, &export, &unset, &env, &ft_exit};
	const char	*ar[9] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", 0};
	int			i;

	if (!tab || !*tab)
		return ;
	(void)str;
	i = -1;
	while (ar[++i])
	{
		if (!ft_strncmp(ar[i], tab[0], 5))
		{
			funcs[i](shell);
			return ;
		}
	}
	search(shell);
	i = -1;
}

void	minishell(t_shell *shell)
{
	char	*str;
	t_prs	*prs;

	if (!(prs = parse_start(shell->envir)))
		exit (0);
	/*while (1)
	{
		ft_putstr_fd(SHELL, 1);
		if (get_next_line(1, &str))
		{
			parse_args((shell->split = ft_split(str, ' ')), str, shell);
			ft_tabclear(shell->split);
			free(str);
		}
	}*/
}

int		main(int ac, char **av, char **environ)
{
	t_shell	shell;
	char	**tmp;

	tmp = environ;
	if (ac)
		ft_printf("\n%s by wquinoa and jalvaro\n\n", ft_strrchr(av[0], '/') + 1);
	ft_bzero(&shell, sizeof(shell));
	shell.environ = ft_tabmap(environ, &ft_strdup);
	while (*environ)
	{
		ft_env_push_back(&shell.envir, ft_envnew(*(environ++)));
	}
	shell.cwd = getcwd(NULL, 42);
	minishell(&shell);
}