/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:37:13 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/16 19:15:29 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	g_state = 0;

void	sig_handler(int signum);
void	display_error(char *message);
int	wait_for_handshake(void);
int	send_byte(char c, int server_pid);

int	main(int ac, char **av)
{
	int					server_pid;
	char				*message;
	int					i;
	struct sigaction	sa;

	if (ac != 3)
		display_error("Usage: ./client [server PID] [message]");
	server_pid = ft_atoi(av[1]);
	if (server_pid <= 0)
		display_error("Invalid PID");
	g_state = server_pid;
	sa.sa_handler = sig_handler;
	sig_handler(-1);
	g_state = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	message = av[2];
	i = 0;
	while (message[i])
	{
		if (send_byte(message[i++], server_pid))
			display_error("Server did not respond");
	}
	if (send_byte('\0', server_pid))
		display_error("Server did not respond");
	while (g_state != 2)
		usleep(100000);
	ft_printf("Message sent\n");
	return (0);
}

void	sig_handler(int signum)
{
	int	server_pid;

	if (signum == -1)
		server_pid = g_state;
	if (signum == SIGUSR1)
		g_state = 1;
	if (signum == SIGUSR2)
		g_state = 2;
	if (signum == SIGINT)
	{
		ft_printf("User interrupt.\n");
		send_byte('\0', server_pid);
		exit(130);
	}
}

void	display_error(char *message)
{
	ft_printf("%s\n", message);
	exit(1);
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
