/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:37:01 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/16 17:29:47 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile t_message	g_message = {0};

void	handle_signal(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (info->si_pid <= 0)
		return ;
	if (g_message.client_pid != info->si_pid)
	{
		if (g_message.length > 0)
			ft_printf("Incomplete message from:%d\n", g_message.client_pid);
		g_message.client_pid = info->si_pid;
		g_message.length = 0;
		g_message.current_byte = 0;
		g_message.bit_position = 0;
		ft_printf("New client connected: %d\n", g_message.client_pid);
	}
	if (signum == SIGUSR1)
		g_message.current_byte |= (1 << (7 - g_message.bit_position));
	g_message.bit_position++;
	if (g_message.bit_position == 8)
	{
		if (g_message.current_byte == 0)
		{
			if (g_message.length < MAX_MESSAGE_SIZE)
				g_message.buffer[g_message.length] = '\0';
			ft_printf("%s\n", g_message.buffer);
			kill(g_message.client_pid, SIGUSR2);
			g_message.length = 0;
			g_message.current_byte = 0;
			g_message.bit_position = 0;
			return ;
		}
		if (g_message.length < MAX_MESSAGE_SIZE - 1)
		{
			g_message.buffer[g_message.length++] = g_message.current_byte;
			g_message.buffer[g_message.length] = '\0';
		}
		else if (g_message.length == MAX_MESSAGE_SIZE - 1)
		{
			ft_printf("Message from: %d over max size\n", g_message.client_pid);
			g_message.buffer[g_message.length] = '\0';
		}
		g_message.current_byte = 0;
		g_message.bit_position = 0;
	}
	kill(g_message.client_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;
	pid_t				pid;

	pid = getpid();
	ft_printf("Server PID: %d\n", pid);
	ft_printf("Waiting for client connection...\n");
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
		pause();
	return (0);
}
