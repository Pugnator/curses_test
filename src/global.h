#ifndef DAEMON_H
#define DAEMON_H

#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#ifndef __x86_64__
#define _FILE_OFFSET_BITS 64
#endif

#define _BSD_SOURCE
#include "tui.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
//#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>


#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
//#include <curl/curl.h>

cursed *hosts_window;
cursed *if_window;

void xmlerror(void * ctx, const char * msg, ...);
void xml_edit_properties(char *filename, char *node_path, char *prop_name,
                         char *new_prop_val);
void xml_create_new_file(char *filename, char *root_name);
int xml_verify_file(char *filename);
void xml_add_child(char *filename, char *node_name, char *node_val);
xmlChar *xml_get_value_from_buf_by_xpath(xmlDocPtr doc, char *nodepath);
xmlChar *xml_get_value_from_file_by_xpath(char *filename, char *nodepath);
xmlChar *xml_get_prop_by_xpath(xmlDocPtr doc, char *nodepath, char *propname);
void xml_edit_key(char *filename, char *node_path, char *new_val);
int xml_remove_node_and_childs(char *filename, char *nodepath);
xmlDocPtr getdoc(char *docname);
xmlChar *xml_doc_to_string(xmlDocPtr doc);
void xml_list_all_key(xmlDocPtr doc);

#endif
