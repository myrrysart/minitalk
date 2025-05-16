/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:05:33 by jyniemit          #+#    #+#             */
/*   Updated: 2025/05/16 12:32:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H
# include <signal.h>
# include <unistd.h>
# include <libft.h>
# define MAX_MESSAGE_SIZE 2097152

typedef struct s_message
{
	unsigned char	buffer[MAX_MESSAGE_SIZE];
	size_t			length;
	pid_t			client_pid;
	unsigned char	current_byte;
	unsigned char	bit_position;
} t_message;

#endif
