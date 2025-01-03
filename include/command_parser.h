#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

/**
 * Clears the console screen
 */
void clear_screen(void);

/**
 * Parses and executes a command string
 * @param command The command string to parse and execute
 */
void parse_command(char *command);

#endif /* COMMAND_PARSER_H */