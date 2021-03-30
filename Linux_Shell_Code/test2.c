#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include<curses.h>

#define MAX_LINE 80

typedef struct lsComandos
{
    char param[50];
    struct lsComandos *prox;
} lsComandos;

typedef lsComandos *lista;

void inicializa(lista *ls)
{
    for (int i = 0; i < MAX_LINE; i++)
        ls[i] = NULL;
}

lsComandos *aloca(char *param)
{
    lsComandos *novo = (lsComandos *)malloc(sizeof(lsComandos));
    strcpy(novo->param, param);
    novo->prox = NULL;
    return novo;
}

void insere(lista *ls, char *param, int indice)
{

    lsComandos *p = ls[indice];
    lsComandos *ant = NULL;

    while (p != NULL)
    {
        ant = p;
        p = p->prox;
    }

    if (p)
        return;

    lsComandos *novo = aloca(param);

    if (!ant)
        ls[indice] = novo;
    else
        ant->prox = novo;
    puts(novo->param);
}

void separaArg(lista *ls, char *args, int indice)
{
    int j = 0;
    for (int i = 0; i < (int)strlen(args) - 1; i++)
    {
        if (args[i + 1] == ' ')
        {
            int tam = i - j + 1;
            char *param;
            param = (char *)malloc(tam * sizeof(char));
            strncpy(param, args + j, tam);

            insere(ls, param, indice);

            j = i + 2;
            free(param);
        }
    }
}

void testaExec(char **pa, char **args)
{
    int fd[2]; //processo pai cria um pipe()

    if (pipe(fd) < 0)
    {
        printf("\nErro na criacao do pipe!");
        exit(0);
    }

    //printf("Teste1");
    //processo pai cria o primeiro processo filho
    pid_t pid1;
    if ((pid1 = fork()) == -1)
    {
        printf("\nErro no primeiro fork!");
        exit(0);
    }
    // printf("Teste2");
    printf("\n[1]%d\n",pid1);

    if (pid1 == 0) //filho 1
    {
        printf("Teste3");

        close(fd[0]);   //fecha o descritor de leitura do pipe
        dup2(fd[1], 1); //redireciona a saida padrao para o pipe
        //"ls","-l","-a"

        if (execvp("/bin/ls", args) < 0)
        {
            printf("\nErro exec() processo 1");
            exit(0);
        }

    }
    else
    { //pai
        pid_t pid2;
        if ((pid2 = fork()) == -1)
        {
            printf("\nErro no segundo fork!");
            exit(0);
        }
        printf("\n[2]%d\n",pid2);

        if (pid2 == 0)
        {             
            printf("Teste4");
                    //filho 2
            close(fd[1]);   //fecha o descritor de escrita do pipe
            dup2(fd[0], 0); //redireciona a entrada padrao para o pipe
            if (execl("/bin/grep", "grep", "Mar", NULL) < 0)
            {
                printf("\nErro exec() processo 2");
                exit(0);
            }
        }
        else
        { //pai
            close(fd[0]);
            close(fd[1]);
            int status;
            for (int i = 0; i < 2; i++)
            {
                wait(&status);
            }
        }
    }
}
// void readCo(char *str)
// {
//     char line[1024];
//     int count = 0, i = 0, j = 0;
//     char *arr[100], *pch;

//     for (;;)
//     {
//         int c = fgetc(stdin);
//         line[count++] = (char) c;
//         if ( c=='\n') 
//             break;
//     }

//     if(count == 1)
//         return; 
//     pch = strtok(line, " \n");

//     while(pch != NULL)
//     {
//         arr[i++] = strdup(pch);
//         pch = strtok(NULL, " \n");
//     } 

//     strcpy(cmd, arr[0]);

//     for(int j=0; j < i; j++)
//         par[j] = arr[j];
//     par[i] = NULL;
// }

int readInput(char *str)
{
    char *buf;

    fgets(buf, 1024, stdin);
    if(strlen(buf) != 0)
    {
        strcpy(str, buf);
        return 0;
    }
    else
        return 1;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
	int i;
	for (i = 0; i < 2; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i] == NULL)
			break;
	}

	if (strpiped[1] == NULL)
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
	}
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	int i;

	for (i = 0; i < 100; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed, char** parsedpipe)
{

	char* strpiped[2];
	int piped = 0;

	piped = parsePipe(str, strpiped);

	if (piped) {
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);

	} else {

		parseSpace(str, parsed);
	}

	
	return 1 + piped;
}

void type_prompt()
{
    static int firstT  = 1;
    if(firstT)
    {
        const char* CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI,12);
        firstT = 0;
    }
    printf("shell> ");
}

void execArgs(char** parsed)
{
	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command..");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printf("\nPipe could not be initialized");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\nCould not fork");
		return;
	}

	if (p1 == 0) {
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command 1..");
			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printf("\nCould not fork");
			return;
		}

		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0) {
				printf("\nCould not execute command 2..");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}



int main(void)
{

    //char texto[100];

    char cmd[100], *parsedArgs[100]; //Número máximo de argumentos
    char* parsedArgsPiped[100];
	int execFlag = 0;
    //char *envp[] = {(char*) "PATH=/bin",0};
    // int executar = 1;            //flag para sair do programa
    // char *texto;
    // texto = (char *) malloc(40 * sizeof(char));

    // lista *ls;
    // inicializa(ls);
    while (1)
    {
        type_prompt();
        initscr();
        int c = getch();
        endwin();
        printf("%d",&c);
        execFlag = processString(cmd,
		parsedArgs, parsedArgsPiped);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.

		// execute

		if (execFlag == 2)
			execArgsPiped(parsedArgs, parsedArgsPiped);        

        
    }
    
    return 0;


    // while (executar)
    // {
    //     printf("shell>");
    //     fflush(stdout); //limpa o buffer de stdout

    //     fgets(texto,MAX_LINE / 2 + 1,stdin);
        
    //     strcat(texto," ");
    //     separaArg(ls,texto,0);
    //     //puts(texto);
    // }
}
