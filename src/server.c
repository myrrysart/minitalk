/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:37:01 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/20 18:52:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile t_message	g_message = {0};

static void		handle_signal(int signum, siginfo_t *info, void *context);
static int		handle_client(siginfo_t *info);
static void		handle_message(int signum);

int	main(void)
{
	struct sigaction	sa;
	pid_t				pid;
	int					last_activity;
	int					current_activity;

	last_activity = 0;
	current_activity = 0;
	pid = getpid();
	ft_printf("Server PID: %d\n", pid);
	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) ==
		-1)
	{
		ft_printf("Error: Failed to set up signal handlers\n");
		return (1);
	}
	while (1)
	{
		current_activity = g_message.bit_position + g_message.length * 8;
		if (g_message.client_pid != 0)
		{
			last_activity = current_activity;
			sleep(50);
			current_activity = g_message.bit_position + g_message.length * 8;
			if (last_activity == current_activity)
			{
				ft_printf("Client timed out, server reset.");
				g_message.length = 0;
				g_message.client_pid = 0;
				g_message.current_byte = 0;
				g_message.bit_position = 0;
			}
		}
		else
			pause();
	}
	return (0);
}

static void	handle_signal(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (info->si_pid <= 0)
		return ;
	if (handle_client(info))
		return ;
	handle_message(signum);
}

static int	handle_client(siginfo_t *info)
{
	if (g_message.client_pid != info->si_pid)
	{
		if (g_message.length > 0)
		{
			kill(info->si_pid, SIGUSR2);
			return (1);
		}
		g_message.client_pid = info->si_pid;
		g_message.length = 0;
		g_message.current_byte = 0;
		g_message.bit_position = 0;
	}
	return (0);
}

static void	handle_message(int signum)
{
	if (signum == SIGUSR1)
		g_message.current_byte |= (1 << (7 - g_message.bit_position));
	g_message.bit_position++;
	if (g_message.bit_position == 8)
	{
		if (g_message.current_byte == 0)
		{
			g_message.buffer[g_message.length] = '\0';
			ft_printf("%s\n", g_message.buffer);
			kill(g_message.client_pid, SIGUSR2);
			g_message.length = 0;
			g_message.current_byte = 0;
			g_message.bit_position = 0;
			g_message.client_pid = 0;
			return ;
		}
		g_message.buffer[g_message.length++] = g_message.current_byte;
		g_message.buffer[g_message.length] = '\0';
		g_message.current_byte = 0;
		g_message.bit_position = 0;
	}
	kill(g_message.client_pid, SIGUSR1);
}
