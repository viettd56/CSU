#ifndef server_execute_h
#define server_execute_h

void execute(int, int); /* server execute */
void write_data(FILE *fr, int sock, char *buff, const int LENGTH);

#endif