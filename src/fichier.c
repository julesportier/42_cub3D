/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fichier.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <vakozhev@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:38:42 by vakozhev          #+#    #+#             */
/*   Updated: 2025/10/19 15:38:57 by vakozhev         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

t_id    parse_id_at_start(const char **pline)
 14 {
 15         const char  *p;
 16         
 17         p = skip_ws(*pline);
 18         t_id id = ID_UNKNOWN;
 19         if (*p == '\0')
 20         {
 21             *pline = p;
 22             return (ID_NONE);
 23         }
 24         if (ft_strncmp(p, "NO", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
 25         {
 26             id = ID_NO;
 27             p += 2;
 28         }
 29         else if (ft_strncmp(p, "SO", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
 30         {
 31             id = ID_SO;
 32             p += 2;
 33         }
 34         else if (ft_strncmp(p, "WE", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
 35         {
 36             id = ID_WE;
 37             p += 2;
 38         }
 39         else if (ft_strncmp(p, "EA", 2) == 0 && (p[2]=='\0' || p[2]==' ' || p[2]=='\t' || p[2]=='\r'))
 40         {
 41             id = ID_EA;
 42             p += 2;
 43         }
 44         else if (ft_strncmp(p, "F", 1) == 0 && (p[1]=='\0' || p[1]==' ' || p[1]=='\t' || p[1]=='\r'))
 45         {
 46             id = ID_F;
 47             p += 1;
 48         }
 49         else if (ft_strncmp(p, "C", 1) == 0 && (p[1]=='\0' || p[1]==' ' || p[1]=='\t' || p[1]=='\r'))
 50         {
 51             id = ID_C;
 52             p += 1;
 53         }
 54         else if (*p == ' ' || *p == '\t' || *p == '\r')
 55             id = ID_NONE;
 56         else
 57             id = ID_UNKNOWN;
 58         *pline = p;
 59         return (id);
 60 }

