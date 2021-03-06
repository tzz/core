


#ifndef CFENGINE_ACCESS_H
#define CFENGINE_ACCESS_H


#include <platform.h>

#include <map.h>                                          /* StringMap */
#include "strlist.h"                                      /* StrList */


/**
 * Access control list referring to one resource, e.g. path, class, variable,
 * literal.
 *
 * @note: Each strlist might be NULL, which is equivalent to having 0
 *        elements.
 *
 * @note: Currently these lists are binary searched, so after filling them up
 *        make sure you call StrList_Sort() to sort them.
 */
struct resource_acl
{
    StrList *admit_ips;
    StrList *admit_hostnames;
    StrList *admit_keys;
    StrList *deny_ips;
    StrList *deny_hostnames;
    StrList *deny_keys;
};

enum acl_type
{
    ACL_TYPE_PATH,
    ACL_TYPE_CONTEXT,
    ACL_TYPE_VARIABLE,
    ACL_TYPE_LITERAL,
    ACL_TYPE_QUERY
//    ACL_TYPE_ROLES
};

/**
 * This is a list of all resorce ACLs for one resource. E.g. for resource_type
 * == path, this should contain a list of all paths together with a list of
 * ACLs (struct resource_acl) referring to the relevant path.
 *
 * @note Currently this list of resource_names may be binary searched so it
 *       must be sorted once populated.
 *
 * @WARNING Remember to store directories *always* with traling '/', else they
 *          won't match for children dirs (on purpose, and this functionality
 *          was built into StrList_SearchLongestPrefix()).
 */
struct acl
{
//TODO    enum acl_type resource_type;
    size_t len;                        /* Length of the following arrays */
    size_t alloc_len;                  /* Used for realloc() economy  */
    StrList *resource_names;    /* paths, class names, variables etc */
    struct resource_acl acls[];
};


/* These acls are set on server startup or when promises change, and are
 * read-only for the rest of their life, thus are thread-safe. */

/* The paths_acl should be populated with directories having a trailing '/'
 * to be able to tell apart from files. */
struct acl *paths_acl;

/* TODO we need the following for optimal ACL lookups in all cases. But first
 * we need to stop accepting regexes as allowed/denied mathes. */
struct acl *classes_acl, *vars_acl, *literals_acl;
struct acl *query_acl;                                         /* reporting */
//extern struct acl *roles_acl;                                /* cf-runagent */


size_t ReplaceSpecialVariables(char *buf, size_t buf_size,
                               const char *find1, const char *repl1,
                               const char *find2, const char *repl2,
                               const char *find3, const char *repl3);


bool access_CheckResource(const struct resource_acl *acl,
                          const char *ipaddr, const char *hostname,
                          const char *key);


size_t acl_SortedInsert(struct acl **a, const char *handle);
void acl_Free(struct acl *a);
bool acl_CheckExact(const struct acl *acl, const char *req_string,
                    const char *ipaddr, const char *hostname,
                    const char *key);
bool acl_CheckPath(const struct acl *acl, const char *reqpath,
                   const char *ipaddr, const char *hostname,
                   const char *key);


#endif
