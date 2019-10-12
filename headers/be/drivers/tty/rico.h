typedef unsigned char uchar;

#ifndef TRUE
#define	bool		char
#define	TRUE		(0 == 0)
#define	FALSE		(!TRUE)
#endif

#define	until(expr)	while (!(expr))
#define	unless(expr)	if (!(expr))

#define	nel( a)		(sizeof( a) / sizeof( (a)[0]))
#define	endof( a)	((a) + nel( a))
#define	bitsof( a)	(8 * sizeof( a))
#define	ctrl( c)	((c) - 0100)
#define	tab( col)	(((col)|7) + 1)
#define	streq( s0, s1)	(strcmp( s0, s1) == 0)
#define	trunc( n, r)	((n) - (n)%(r))
#define	roundup( n, r)	((n) - 1 - ((n)+(r)-1)%(r) + (r))
