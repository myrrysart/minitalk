/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:15:30 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/20 18:10:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	g_state = 0;

static void					sig_handler(int signum);
static int					wait_for_handshake(void);
static int					send_byte(char c, int server_pid);
static void					setup_signals(void);

int	main(int ac, char **av)
{
	int		server_pid;
	char	*message;
	int		i;

	if (ac != 3)
		ft_err_exit("Usage: ./client [server PID] [message]");
	server_pid = ft_atoi(av[1]);
	if (server_pid <= 0)
		ft_err_exit("Invalid PID");
	setup_signals();
	message = av[2];
	i = 0;
	while (message[i])
	{
		if (send_byte(message[i++], server_pid))
			ft_err_exit("Server did not respond");
		if (g_state == 2)
			ft_err_exit("Server busy with another client, try again later");
	}
	if (send_byte('\0', server_pid))
		ft_err_exit("Server did not respond");
	if (g_state != 2)
		usleep(500000);
	if (g_state != 2)
		ft_err_exit("Final acknowledgement missing.");
	return (0);
}

void	setup_signals(void)
{
	struct sigaction	sa;

	g_state = 0;
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
}

void	sig_handler(int signum)
{
	if (signum == SIGUSR1)
		g_state = 1;
	if (signum == SIGUSR2)
		g_state = 2;
}

int	wait_for_handshake(void)
{
	int	timeout;

	timeout = 0;
	while (g_state == 0)
	{
		usleep(500);
		timeout++;
		if (timeout > 200)
			return (0);
	}
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
		if (g_state == 2)
			return (0);
		g_state = 0;
		bit--;
	}
	return (0);
}
