#ifndef LOGIC_H
#define LOGIC_H

typedef enum logic_conn {
	CONN_AND,
	CONN_OR,
	CONN_NOT
} logic_conn;

char logic_conn_symbol(logic_conn conn);

#endif
