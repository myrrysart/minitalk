/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:37:13 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/15 17:34:52 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	g_state = 0;

void	sig_handler(int signum)
{
	if (signum == SIGUSR1 || signum == SIGUSR2)
		g_state = 1;
}

int	display_error(char *message)
{
	while (*message)
		write(1, message++, 1);
	write(1, "\n", 1);
	return (1);
}

int	wait_for_handshake(void)
{
	int	timeout;

	timeout = 0;
	while (g_state == 0)
	{
		usleep(100);
		timeout++;
		if (timeout > 100)
			return (0);
	}
	g_state = 0;
	return (1);
}

int	send_byte(char c, int server_pid)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		if ((c >> bit) & 1)
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		if (!wait_for_handshake())
			return (1);
		bit--;
	}
	return (0);
}

int	main(int ac, char **av)
{
	int					server_pid;
	char				*message;
	int					i;
	struct sigaction	sa;

	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	if (ac != 3)
		return (display_error("Usage: ./client [server PID] [message]"));
	server_pid = ft_atoi(av[1]);
	if (server_pid <= 0)
		return (display_error("Invalid PID"));
	message = av[2];
	i = 0;
	while (message[i])
	{
		if (send_byte(message[i++], server_pid))
			return (display_error("Server did not respond"));
	}
	if (send_byte('\0', server_pid))
		return (display_error("Server did not respond"));
	return (0);
}
