#ifndef H_BONE_ALIAS
#define H_BONE_ALIAS

#include <module.h>

#ifndef _ALIAS_H_
#define _ALIAS_H_

/* Alias link representative (incomplete struct) */
struct alias_link;

/********************** Mode flags ********************/
/* Set these flags using PacketAliasSetMode() */

/* If PKT_ALIAS_LOG is set, a message will be printed to
	/var/log/alias.log every time a link is created or deleted.  This
	is useful for debugging */
#define PKT_ALIAS_LOG 0x01

/* If PKT_ALIAS_DENY_INCOMING is set, then incoming connections (e.g.
	to ftp, telnet or web servers will be prevented by the aliasing
	mechanism.  */
#define PKT_ALIAS_DENY_INCOMING 0x02

/* If PKT_ALIAS_SAME_PORTS is set, packets will be attempted sent from
	the same port as they originated on.  This allows e.g. rsh to work
	*99% of the time*, but _not_ 100%.  (It will be slightly flakey
	instead of not working at all.)  This mode bit is set by
        PacketAliasInit(), so it is a default mode of operation. */
#define PKT_ALIAS_SAME_PORTS 0x04

/* If PKT_ALIAS_USE_SOCKETS is set, then when partially specified
	links (e.g. destination port and/or address is zero), the packet
	aliasing engine will attempt to allocate a socket for the aliasing
	port it chooses.  This will avoid interference with the host
	machine.  Fully specified links do not require this.  This bit
        is set after a call to PacketAliasInit(), so it is a default
        mode of operation. */
#define PKT_ALIAS_USE_SOCKETS 0x08

/* If PKT_ALIAS_UNREGISTERED_ONLY is set, then only packets with
	unregistered source addresses will be aliased.  Private
	addresses are those in the following ranges:
		10.0.0.0     ->   10.255.255.255
		172.16.0.0   ->   172.31.255.255
		192.168.0.0  ->   192.168.255.255  */
#define PKT_ALIAS_UNREGISTERED_ONLY 0x10

/* If PKT_ALIAS_RESET_ON_ADDR_CHANGE is set, then the table of dynamic
	aliasing links will be reset whenever PacketAliasSetAddress()
        changes the default aliasing address.  If the default aliasing
        address is left unchanged by this function call, then the
        table of dynamic aliasing links will be left intact.  This
        bit is set after a call to PacketAliasInit(). */
#define PKT_ALIAS_RESET_ON_ADDR_CHANGE 0x20

#ifndef NO_FW_PUNCH
/* If PKT_ALIAS_PUNCH_FW is set, active FTP and IRC DCC connections
   will create a 'hole' in the firewall to allow the transfers to
   work.  Where (IPFW "line-numbers") the hole is created is
   controlled by PacketAliasSetFWBase(base, size). The hole will be
   attached to that particular alias_link, so when the link goes away
   so do the hole.  */
#define PKT_ALIAS_PUNCH_FW 0x100
#endif

/* If PKT_ALIAS_PROXY_ONLY is set, then NAT will be disabled and only
      transparent proxying performed */
#define PKT_ALIAS_PROXY_ONLY 0x40

/* If PKT_ALIAS_REVERSE is set, the actions of PacketAliasIn()
      and PacketAliasOut() are reversed */
#define PKT_ALIAS_REVERSE 0x80

/* Return Codes */
#define PKT_ALIAS_ERROR -1
#define PKT_ALIAS_OK 1
#define PKT_ALIAS_IGNORED 2
#define PKT_ALIAS_UNRESOLVED_FRAGMENT 3
#define PKT_ALIAS_FOUND_HEADER_FRAGMENT 4

#endif

typedef struct bone_alias_info {

	struct module_info module;

/* Lock */
	void (*Lock)(void);
	void (*Unlock)(void);

/* Initialization and Control */
	void (*PacketAliasSetAddress)(struct in_addr);
	unsigned int (*PacketAliasSetMode)(unsigned int, unsigned int);

/* Packet Handling */
	int (*PacketAliasIn)(char *, int maxpacketsize);
	int (*PacketAliasOut)(char *, int maxpacketsize);
	int (*PacketUnaliasOut)(char *, int maxpacketsize);
	struct alias_link * (*PacketAliasRedirectPort)(struct in_addr, u_short, 
                            struct in_addr, u_short,
                            struct in_addr, u_short,
                            u_char);
	int (*PacketAliasAddServer)(struct alias_link *link,
                         struct in_addr addr,
                         u_short port);
	struct alias_link * (*PacketAliasRedirectProto)(struct in_addr,
                             struct in_addr,
                             struct in_addr,
                             u_char);
	struct alias_link *	(*PacketAliasRedirectAddr)(struct in_addr,
                            struct in_addr);
	void (*PacketAliasRedirectDelete)(struct alias_link *);

/* Fragment Handling */
	int (*PacketAliasSaveFragment)(char *);
	char * (*PacketAliasGetFragment)(char *);
	void (*PacketAliasFragmentIn)(char *, char *);

/* Miscellaneous Functions */
	void (*PacketAliasSetTarget)(struct in_addr addr);
	int (*PacketAliasCheckNewLink)(void);
	u_short (*PacketAliasInternetChecksum)(u_short *, int);
/* Transparent Proxying */
	int (*PacketAliasProxyRule)(const char *); 
} bone_alias_info_t;

#define BONE_ALIAS_MODULE "network/bone_alias"

#endif /* H_BONE_ALIAS */
