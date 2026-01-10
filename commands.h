#ifndef COMMANDS_H
#define COMMANDS_H

void inicializar_dados();
void mkdir_(int num_ant, char *nome_arq);
void touch(int num_pai, char nome_arquivo[]);
int separar_comando_cd(char texto[], char matriz_dicionarios[][15]);
int cd(int num_inode, char caminho[], char comando[]);
void ls(int num_diretorio);
void pwd(int num_inode_atual);
void cat(int num_diretorio_atual, char nome_arquivo[]);
void rm(int inode_pai, char nome_arq[]);
void t_stat();
void help();
#endif
