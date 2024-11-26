#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

/**
 *@brief the status code of link list's operation
 */
enum LINK_LIST_STATUS_CODES {
	SUCCESS_ADD,            /* add new node successfully */
	FAILED_ALLOCATE_MEMORY, /* can't allocate memory for new node */
	DUPLICATED_NODE         /* the node item (category) exists */
};

#endif // !_CONSTANTS_H_