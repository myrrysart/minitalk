/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:37:01 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/15 17:26:21 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <minitalk.h>

volatile sig_atomic_t	g_state = 0;

pid_t	get_client_pid(void)
{
	return (g_state >> 11);
}

unsigned char	get_current_byte(void)
{
	return (g_state & 0xFF);
}

unsigned char	get_bit_position(void)
{
	return ((g_state >> 8) & 0x7);
}

void	update_state(pid_t client_pid, unsigned char byte,
		unsigned char bit_pos)
{
	g_state = (client_pid << 11) | (bit_pos << 8) | byte;
}

void	handle_signal(int signum, siginfo_t *info, void *context)
{
	unsigned char	current_byte;
	unsigned char	bit_position;
	pid_t			client_pid;

	current_byte = get_current_byte();
	bit_position = get_bit_position();
	client_pid = info->si_pid;
	(void)context;
	if (bit_position == 0 && current_byte == 0)
		update_state(client_pid, 0, 0);
	if (signum == SIGUSR1)
		current_byte |= (1 << (7 - bit_position));
	bit_position++;
	if (bit_position == 8)
	{
		write(1, &current_byte, 1);
		update_state(client_pid, 0, 0);
	}
	else
	{
		update_state(client_pid, current_byte, bit_position);
	}
	kill(client_pid, SIGUSR1);
	if (bit_position == 8 && current_byte == 0)
		write(1, "\n", 1);
}

int	main(void)
{
	struct sigaction	sa;
	pid_t				pid;

	pid = getpid();
	ft_printf("Server PID: %d\n", pid);
	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
