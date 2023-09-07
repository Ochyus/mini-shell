#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

// Function to tokenize a string
void tokenize(char *input, int *token_count, char ***token_vector)
{
    int count = 0;
    char **tokens = NULL;
    char *token = strtok(input, " \t\n"); // Tokenize using spaces, tabs, and newlines

    while (token != NULL)
    {
        count++;
        tokens = (char **)realloc(tokens, count * sizeof(char *));
        tokens[count - 1] = strdup(token);
        token = strtok(NULL, " \t\n");
    }

    *token_count = count;
    *token_vector = tokens;
}

// Function to execute print command
void executePrint(char **tokens, int token_count)
{
    if (token_count >= 2)
    {
        for (int i = 1; i < token_count; i++)
        {
            printf("%s", tokens[i]);
            if (i < token_count - 1)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    else
    {
        printf("Usage: print <text>\n");
    }
}

// Function to execute println command
void executePrintln(char **tokens, int token_count)
{
    if (token_count >= 2)
    {
        for (int i = 1; i < token_count; i++)
        {
            printf("%s\n", tokens[i]);
        }
    }
    else
    {
        printf("Usage: println <text>\n");
    }
}

// Function to execute run command
void executeRun(char **tokens, int token_count)
{
    if (token_count >= 2)
    {
        // Fork a new process
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            // This is the child process
            // Execute the specified program
            if (execvp(tokens[1], &tokens[1]) == -1)
            {
                perror("execvp");
                exit(1);
            }
        }
        else
        {
            // This is the parent process
            // Wait for the child to terminate
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else
    {
        printf("Usage: run <program> [arguments]\n");
    }
}

// Function to execute println command
void executeHello(char **tokens, int token_count)
{
    if (token_count == 1)
    {
        printf("Hello World!\n");
    }
    else
    {
        printf("Usage: hello\n");
    }
}

int main()
{
    bool runLoop = true;
    char input[1024];
    int token_count;
    char **token_vector;
    int status;

    while (runLoop)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s Mini-shell> ", cwd);
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            // End of input
            break;
        }

        // Tokenize the input
        tokenize(input, &token_count, &token_vector);

        if (token_count > 0)
        {
            // Check for custom commands
            if (strcmp(token_vector[0], "print") == 0)
            {
                executePrint(token_vector, token_count);
            }
            else if (strcmp(token_vector[0], "println") == 0)
            {
                executePrintln(token_vector, token_count);
            }
            else if (strcmp(token_vector[0], "run") == 0)
            {
                executeRun(token_vector, token_count);
            }
            else if (strcmp(token_vector[0], "hello") == 0)
            {
                executeHello(token_vector, token_count);
            }
            else if (strcmp(token_vector[0], "exit") == 0)
            {
                runLoop = false;
            }

            // Free the token vector
            for (int i = 0; i < token_count; i++)
            {
                free(token_vector[i]);
            }
            free(token_vector);
        }
    }

    printf("Exiting Mini-shell.\n");
    return 0;
}
