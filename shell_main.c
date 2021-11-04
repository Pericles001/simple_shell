#include "shell.h"

/**
 * Auth: Emma Udeji
 *	Pericles Adjovi
 * 
 * Description:
 * the basic_shell building block
 */


	char **commands = NULL;
	char *line = NULL;
	char *shell_name = NULL;
	int status = 0;

/**
 * main - the main shell code
 * @argc: number of arguments passed
 * @argv: program arguments to be parsed
 *
 * applies the functions in utils and helpers
 * implements EOF
 * Prints error on Failure
 * Return: 0 on success
 */


int main(int argc __attribute__((unused)), char **argv)
{
	char **current_command = NULL;
	int i, type_command = 0;
	size_t n = 0;

	/**
	 * initializing the prompt signal
	 *
	 * Description:
	 * Display a prompt and wait for the user to type a command
	 * prompt is displayed again each time a command has been executed
	 *
	 * implements the Ctrl_c function
	 */


	signal(SIGINT, ctrl_c_handler);
	shell_name = argv[0];
	while (1)
	{
		non_interactive();

		/* displaying the prompt */
		print(" ($) ", STDOUT_FILENO);

		/* reading the command line */
		if (getline(&line, &n, stdin) == -1)
		{
			free(line);
			exit(status);
		}
		/**
		 * parsing the line arguments
		 *
		 * tokenizer - tokenizes input and stores it into an array
		 * parse_command - determines the type of command
		 *
		 */
			remove_newline(line);
			remove_comment(line);
			commands = tokenizer(line, ";");

		for (i = 0; commands[i] != NULL; i++)
		{
			current_command = tokenizer(commands[i], " ");
			if (current_command[0] == NULL)
			{
				free(current_command);
				break;
			}
			type_command = parse_command(current_command[0]);

			/* initializer -   */
			initializer(current_command, type_command);
			free(current_command);
		}
		free(commands);
	}
	free(line);

	return (status);
}

		/**
		 * initializer - starts executing everything
		 * @current_command: try to check current token
		 * @type_command: parse token
		 *
		 * Return: void function
		 */

		void initializer(char **current_command, int type_command)
		{
			pid_t PID;

			if (type_command == EXTERNAL_COMMAND || type_command == PATH_COMMAND)
			{
				PID = fork();
				if (PID == 0)
					execute_command(current_command, type_command);
				else
				{
					waitpid(PID, &status, 0);
					status >>= 8;
				}
			}
			else
				execute_command(current_command, type_command);
		}

		/**
		 * non_interactive - handles non_interactive mode
		 *
		 * Return: void
		 */

		void non_interactive(void)
		{
			char **current_command = NULL;
			int i, type_command = 0;
			size_t n = 0;

			if (!(isatty(STDIN_FILENO)))
			{
				while (getline(&line, &n, stdin) != -1)
				{
					remove_newline(line);
					remove_comment(line);
					commands = tokenizer(line, ";");
					for (i = 0; commands[i] != NULL; i++)
					{
						current_command = tokenizer(commands[i], " ");
						if (current_command[0] == NULL)
						{
							free(current_command);
							break;
						}
						type_command = parse_command(current_command[0]);
						initializer(current_command, type_command);
						free(current_command);
					}
					free(commands);
				}
				free(line);
				exit(status);
			}
		}

