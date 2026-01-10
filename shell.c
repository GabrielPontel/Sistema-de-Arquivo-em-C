#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "commands.h"

void main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int num_inode;
    char texto[256], comando[256], caminho[256];
    inicializar_dados();

    strcpy(caminho, "/");
    num_inode = 0;
    comando[0] = '\0';

    while (strcmp(comando, "exit") != 0)
    {
        printf("\n%s$ ", caminho);
        realizar_leitura(texto, 256);
        separar_prompt_usuario(texto, comando);
        if (strcmp(comando, "mkdir") == 0)
        {
            mkdir_(num_inode, texto);
        }
        else if (strcmp(comando, "cd") == 0)
        {
            num_inode = cd(num_inode, caminho, texto);
        }
        else if (strcmp(comando, "touch") == 0)
        {
            touch(num_inode, texto);
        }
        else if (strcmp(comando, "cat") == 0)
        {
            cat(num_inode, texto);
        }
        else if (strcmp(comando, "rm") == 0)
        {
            rm(num_inode, texto);
        }
        else if (strlen(texto) == 0)
        {
            if (strcmp(comando, "ls") == 0)
            {
                ls(num_inode);
            }
            else if (strcmp(comando, "pwd") == 0)
            {
                pwd(num_inode);
            }
            else if (strcmp(comando, "help") == 0)
            {
                help();
            }
            else if (strcmp(comando, "stat") == 0)
            {
                t_stat();
            }
            else if (strcmp(comando, "exit") == 0)
            {
                printf("\nATE BREVE!!");
            }
            else
            {
                printf("\nCOMANDO INVALIDO!!");
            }
        }
        else
        {
            printf("\nCOMANDO INVALIDO!!");
        }
    }
}
