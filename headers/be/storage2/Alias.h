/***************************************************************************
//
//	File:			Alias.h
//
//	Description:	path->alias->path functions
//
//	Copyright 1992-98, Be Incorporated, All Rights Reserved.
//
***************************************************************************/

#ifndef _ALIAS_H
#define _ALIAS_H

#ifndef _BE_BUILD_H
#include <BeBuild.h>
#endif
#include <support2/SupportDefs.h>

namespace B {
namespace Storage2 {

using namespace Support2;

class	BDataIO;

status_t resolve_link(const char *path, BString *result,
							bool block = false);
status_t write_alias(const char *path, BDataIO *s,
							size_t *len = NULL);
status_t write_alias(const char *path, void *buf, size_t *len);

status_t read_alias(BDataIO *s, BString *result, size_t *len = NULL,
						bool block = false);
status_t read_alias(const void *buf, BString *result, size_t *len,
						bool block = false);

} }	// namespace B::Storage2

#endif
