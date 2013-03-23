//
// netsoul_define.hpp for camelsoul in /home/post_l/Dropbox/share_epitech/camelsoul
//
// Made by ludovic post
// Login   <post_l@epitech.net>
//
// Started on  Thu Nov 22 00:55:06 2012 ludovic post
// Last update Sat Mar 23 22:10:09 2013 ludovic post
//

// Thanks to gossel_j


#ifndef	__NETSOUL_DEFINE_HPP__
#define	__NETSOUL_DEFINE_HPP__

// --- server ---

#define NS_NO_SUCH_CMD	"rep 001 -- no such cmd"
#define NS_CMD_END	"rep 002 -- cmd end"
#define NS_LOGIN_FAIL	"rep 033 -- ext user identification fail"
#define NS_PING		"ping"
#define NS_SALUT	"salut"
#define NS_CMD_USR	"user_cmd"
#define NS_CMD_USR_MSG	"msg"
#define NS_CMD_USR_TYP	"dotnetSoul_UserTyping"
#define NS_CMD_USR_STYP	"dotnetSoul_UserCancelledTyping"

// --- client ---

#define NS_STATE	"state"
#define NS_LOGOUT	"logout"
#define NS_LOGIN	"login"
#define NS_USR_LOG	"ext_user_log"
#define NS_ASK_AUTH	"auth_ag ext_user none none"
#define NS_CMD_MSG_USR	"msg_user"
#define NS_CMD_EXIT	"exit"
#define NS_CMD_WATCH	"watch_log_user"
#define NS_CMD_WHO	"who"

// --- commande nb ---

#define NO_CMD		0
#define NB_ASK_AUTH	1
#define NB_USR_LOG	2

#endif
