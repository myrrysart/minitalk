/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:05:33 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/20 18:59:33 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H
//for signal stuff
# include <signal.h>
//for ft_printf
# include <libft.h>
# define MAX_MESSAGE_SIZE 2097152

typedef struct s_message
{
	unsigned char	buffer[MAX_MESSAGE_SIZE];
	size_t			length;
	pid_t			client_pid;
	unsigned char	current_byte;
	unsigned char	bit_position;
}	t_message;

#endif
