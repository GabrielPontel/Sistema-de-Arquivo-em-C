//Fun��o para criar um diretorio
void mkdir_(int num_ant, char *nome_arq)
{
    int valida;
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;
    if(strlen(nome_arq) < 15 && strlen(nome_arq)!=0)
    {
        valida = verificar_nome_diretorio(num_ant,nome_arq);
        if(valida == 1)
        {
            printf("\nJA EXISTE ARQUIVO COM ESSE NOME NO DIRETORIO, POR ISSO NAO FOI POSSSIVEL CRIAR ESSE DIRETORIO!!");
        }
        else
        {
            inode = retornar_inode_livre();

            inode.blocos[0] = ocupa_bloco_mapa();

            if(inode.tipo == '-')
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM INODE CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
            }
            else if(inode.blocos[0] == -1)
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
            }
            else
            {
                reseta_bloco(inode.blocos[0]);

                inode.tipo = 'd';
                inode.qtd_blocos = 1;

                inode.tamanho = retornar_blocksize();

                atualiza_inode(inode);

                strcpy(entrada_diretorio.nome, ".");
                entrada_diretorio.num_inode = inode.num;
                gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);

                strcpy(entrada_diretorio.nome, "..");
                entrada_diretorio.num_inode = num_ant;
                gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);

                strcpy(entrada_diretorio.nome, nome_arq);
                entrada_diretorio.num_inode = inode.num;
                valida = gravar_entrada_diretorio_pai(num_ant, entrada_diretorio);
                if(valida == -1)
                {
                    libera_bloco_mapa(inode.blocos[0]);
                    libera_inode(inode);
                }
            }
        }
    }
    else
        printf("O NOME DO ARQUIVO EXECEDE 14 CARACTERES OU ESTA VAZIO\n");
}

// Funcao touch cria um arquivo e grava o conteudo
void touch(int num_pai, char nome_arquivo[])
{
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;
    int valida, verifica_pai = 1, i, j, blocksize, num_blocos_necessarios, inicio, fim, tamanho_parte,tam;
    int c, k, tamanho_total, c1, c2;
    blocksize = retornar_blocksize();
    tam = 10 * blocksize;
    char parte[blocksize + 1];
    char conteudo[tam];

    if (strlen(nome_arquivo) < 15 && strlen(nome_arquivo) != 0)
    {
        valida = verificar_nome_diretorio(num_pai, nome_arquivo);
        if (valida == 1)
        {
            printf("\nJA EXISTE ARQUIVO COM ESSE NOME NO DIRETORIO, POR ISSO NAO FOI POSSSIVEL CRIAR ESSE ARQUIVO!!");
        }
        else
        {
            inode = retornar_inode_livre();

            if (inode.tipo == '-')
            {
                printf("ERRO NAO HA INODE LIVRE DISPONIVEL.\n");
            }
            else
            {
                inode.tipo = 'f';
                printf("DIGITE O CONTEUDO DO ARQUIVO: \n");
                i = 0;

                #ifdef __linux__
                    printf("USE CTRL+D NA PROXIMA LINHA PARA ENCERRAR \n");
                    while ((c = getchar()) != EOF && i < tam - 1)
                    {
                        if(c == 27){
                            if(i+3<tam-1){
                                conteudo[i++] = '^';
                                c = getchar();
                                conteudo[i++] = (char)c;
                                conteudo[i++] = (char)c;
                            }
                        }
                        else{
                            conteudo[i++] = (char)c;
                        }
                    }
                    i--;
                    conteudo[i] = '\0';
                    clearerr(stdin);
                #elif _WIN32
                    printf("USE CTRL+D PARA ENCERRAR \n");
                    do
                    {
                        c = getchar();
                        conteudo[i] = c;
                        i++;
                    }
                    while (c != 4);
                    getchar();
                    i--;
                    conteudo[i] = '\0';
                #endif

                tamanho_total = strlen(conteudo);
                num_blocos_necessarios = (tamanho_total) / blocksize;
                if (tamanho_total % blocksize != 0)
                    num_blocos_necessarios++;

                if (num_blocos_necessarios > 10)
                {
                    printf("ERRO O ARQUIVO EXECEDE A QUANTIDADE DE 10 BLOCOS DO INODE\n");
                }
                else
                {
                    valida = 1;
                    i = 0;
                    while (i < num_blocos_necessarios && valida == 1)
                    {
                        inode.blocos[i] = ocupa_bloco_mapa();
                        if (inode.blocos[i] == -1)
                        {
                            printf("ERRO NAO A QUANTIDADE NECESSARIA DE BLOCOS DISPONIVEIS\n");
                            valida = -1;
                        }
                        i++;
                    }
                    if (valida == 1)
                    {
                        inode.qtd_blocos = num_blocos_necessarios;
                        for (i = 0; i < num_blocos_necessarios; i++)
                        {

                            inicio = i * blocksize;
                            fim = inicio + blocksize;
                            if (fim > tamanho_total)
                                fim = tamanho_total;

                            tamanho_parte = fim - inicio;

                            for (j = 0; j < tamanho_parte; j++)
                            {
                                parte[j] = conteudo[inicio + j];
                            }
                            parte[tamanho_parte] = '\0';

                            gravar_bloco(inode.blocos[i], parte);
                        }

                        inode.tamanho = tamanho_total;
                        atualiza_inode(inode);

                        strcpy(entrada_diretorio.nome, nome_arquivo);
                        entrada_diretorio.num_inode = inode.num;

                        verifica_pai = gravar_entrada_diretorio_pai(num_pai, entrada_diretorio);
                    }
                    if (valida == -1 || verifica_pai == -1)
                    {
                        j = 0;
                        while (j < i)
                        {
                            libera_bloco_mapa(inode.blocos[i]);
                        }
                        libera_inode(inode);
                        if (verifica_pai == -1)
                            printf("ERRO NO DIRETORIO PAI ESTA CHEIO \n");
                    }
                }
            }
        }
    }
    else
        printf("O NOME DO ARQUIVO EXECEDE 14 CARACTERES OU ESTA VAZIO\n");
}

//Fun��o para separar as instru��es do cd, se retornar -1 significa que tem alguem diretorio
int separar_comando_cd(char texto[], char matriz_dicionarios[][15])
{
    int tl, i, j, tamanho_nome;
    tl = 0;
    i = 0;
    if(texto[0] == '/')
        i++;
    while(i < strlen(texto) && tl>-1)
    {
        j=0;
        while(i<strlen(texto) && texto[i] != '/')
        {
            matriz_dicionarios[tl][j] = texto[i];
            j++;
            i++;
        }
        matriz_dicionarios[tl][j] = '\0';
        if(j<=15)
            tl++;
        else
            tl=-1;
        i++;
    }
    return tl;
}

//Fun��o cd
int cd(int num_inode, char caminho[], char comando[])
{
    char matriz_diretorios[15][15];
    int tl, i, verifica, auxiliar;
    tp_no *raiz;
    tp_inode inode;

    inicializar_arvore(&raiz);
    if(strcmp(comando, "..")==0)
    {
        raiz = ler_todas_entradas_inode(num_inode, raiz);
        raiz = pesquisa_entrada_diretorio(raiz,"..");
        num_inode = raiz->info.num_inode;
        if(strcmp(caminho, "/")!=0)
        {
            i = strlen(caminho);
            while(i>0 && caminho[i]!='/')
            {
                i--;
            }
            caminho[i] = '\0';
        }
        if(strcmp(caminho,"")==0)
            strcpy(caminho,"/");
    }
    else if(strcmp(comando, "/") == 0)
    {
        num_inode = 0;
        strcpy(caminho,"/");
    }
    else if(strcmp(comando, ".")!=0)
    {
        tl = separar_comando_cd(comando, matriz_diretorios);
        if(tl == -1)
        {
            printf("\nNAO FOI POSSIVEL REALIZAR O COMANDO CD JA QUE CONTEM UM NOME DE DIRETORIO QUE EXCEDE O TAMANHO\n");
        }
        else
        {

            if(comando[0] == '/')
            {
                num_inode = 0;
            }
            i=0;
            do
            {
                verifica = 0;
                raiz = ler_todas_entradas_inode(num_inode, raiz);
                raiz = pesquisa_entrada_diretorio(raiz,matriz_diretorios[i]);
                if(raiz == NULL)
                {
                    printf("NAO FOI POSSIVEL REALIZAR O COMANDO CD, JA QUE NAO FOI ENCONTRADO O DIRETORIO: %s\n", matriz_diretorios[i]);
                }
                else
                {
                    inode = retornar_inode(raiz->info.num_inode);
                    if(inode.tipo != 'd')
                    {
                        printf("NAO FOI POSSIVEL REALIZAR O COMANDO CD, JA QUE %s NAO E UM DIRETORIO\n", matriz_diretorios[i]);
                    }
                    else
                    {
                        num_inode = raiz->info.num_inode;
                        verifica = 1;
                        if(comando[0]=='/')
                        {
                            strcpy(caminho, comando);
                        }
                        else
                        {
                            if(caminho[strlen(caminho)-1]!='/')
                            {
                                strcat(caminho, "/");
                            }
                            strcat(caminho, comando);
                        }
                    }
                }
                i++;
            }
            while(verifica == 1 && i<tl);
        }
    }
    return num_inode;
}



//Listar todos os arquivos do diret�rio atual
void ls(int num_diretorio)
{

    tp_no *raiz;

    inicializar_arvore(&raiz);

    raiz = ler_todas_entradas_inode(num_diretorio, raiz);

    if (raiz == NULL)
    {
        printf("DIRETORIO VAZIO\n");
    }
    else
    {
        listar_em_ordem(raiz);
    }
}

// Busca em �rvore bin�ria pelo nome do diret�rio com o inode
int buscar_nome_por_inode(tp_no *raiz, int num_inode, char *nome_encontrado)
{
    int valida=0;
    if (raiz != NULL)
    {
        if (raiz->info.num_inode == num_inode && strcmp(raiz->info.nome, ".") != 0 && strcmp(raiz->info.nome, "..") != 0)
        {
            strcpy(nome_encontrado, raiz->info.nome);
            valida = 1;
        }

        if(valida ==0)
        {
            if (buscar_nome_por_inode(raiz->esq, num_inode, nome_encontrado))
                valida = 1;
            if (buscar_nome_por_inode(raiz->dir, num_inode, nome_encontrado))
                valida = 1;
        }
    }
    return valida;
}

//Fun�ao pwd mostra em que diretorio esta
void pwd(int num_inode_atual)
{
    tp_inode inode_atual, inode_pai;
    tp_no *raiz, *raiz_pai, *atual, *pai;
    char caminho[200], aux[200];
    char nome[50];
    int num_pai, achou;

    inicializar_arvore(&raiz);

    caminho[0] = '\0';
    inode_atual = retornar_inode(num_inode_atual);
    raiz = ler_todas_entradas_inode(inode_atual.num, NULL);
    atual = pesquisa_entrada_diretorio(raiz, ".");
    pai   = pesquisa_entrada_diretorio(raiz, "..");

    while (atual != NULL && pai != NULL && atual->info.num_inode != pai->info.num_inode)
    {
        num_pai = pai->info.num_inode;
        inode_pai = retornar_inode(num_pai);
        raiz_pai = ler_todas_entradas_inode(num_pai, NULL);

        nome[0] = '\0';
        achou = buscar_nome_por_inode(raiz_pai, num_inode_atual, nome);

        aux[0] = '\0';
        if (achou && strlen(nome) > 0)
        {
            strcat(aux, "/");
            strcat(aux, nome);
        }
        strcat(aux, caminho);
        strcpy(caminho, aux);
        num_inode_atual = num_pai;
        inode_atual = inode_pai;
        raiz = raiz_pai;
        atual = pesquisa_entrada_diretorio(raiz, ".");
        pai   = pesquisa_entrada_diretorio(raiz, "..");
    }

    if (strlen(caminho) == 0)
        strcpy(caminho, "/");

    printf("%s\n", caminho);
}

//Apresentar o conte�do de um arquivo (no diret�rio atual)
void cat(int num_diretorio_atual, char nome_arquivo[])
{
    tp_no *raiz;
    tp_no *no_arquivo;
    tp_inode inode;
    int i, bloco;
    char linha[128];
    char nomearq[15];

    inicializar_arvore(&raiz);

    raiz = ler_todas_entradas_inode(num_diretorio_atual, raiz);

    no_arquivo = pesquisa_entrada_diretorio(raiz, nome_arquivo);

    if(no_arquivo == NULL)
    {
        printf("ARQUIVO NAO ENCONTRADO NO DIRETORIO ATUAL\n");
    }
    else
    {

        inode = retornar_inode(no_arquivo->info.num_inode);

        if(inode.tipo != 'f')
        {
            printf("ESSE NOME NAO E DE UM ARQUIVO\n");
        }
        else
        {

            for(i = 0; i < inode.qtd_blocos; i++)
            {
                apresentar_bloco(inode.blocos[i]);
            }
        }
    }

    printf("\n");
}
//Fun��o para excluir um arquivo ou diret�rio do atual
void rm(int inode_pai, char nome_arq[])
{
    tp_no *raiz;
    tp_no *desejado;
    tp_inode inode;

    inicializar_arvore(&raiz);
    raiz = ler_todas_entradas_inode(inode_pai,raiz);
    desejado = pesquisa_entrada_diretorio(raiz,nome_arq);
    if(desejado != NULL)
    {
        if(strcmp(nome_arq,".")!=0 && strcmp(nome_arq,"..")!=0)
        {
            inode = retornar_inode(desejado->info.num_inode);
            if(inode.tipo == 'f')
            {
                apagar_arquivo(inode.num);
            }
            else
            {
                apagar_diretorio(inode.num);
            }
            remover_entrada_do_diretorio(inode_pai, desejado->info.num_inode);
        }
        else
            printf("\nNAO SE PODE EXCLUIR AS ENTRADAS '.' E '..'\n");
    }
    else
    {
        printf("\nESSE ARQUIVO OU DIRETORIO NAO ESTA PRESENTE NO DIRETORIO ATUAL\n");
    }
}

//Fun��o stat que vai apresentar a situa��o do sistema de arquivo
void stat()
{
    int qtd_livres_bytes,qtd_blocos_livres,tam_bloco;

    qtd_blocos_livres = calcular_quant_blocos_livres();
    tam_bloco = retornar_blocksize();

    qtd_livres_bytes = qtd_blocos_livres * tam_bloco;

    printf("\nESPACO LIVRE: %d BYTES", qtd_livres_bytes);
    printf("\nBLOCOS LIVRES: %d BLOCOS", qtd_blocos_livres);
    printf("\nTAMANHO DO BLOCO: %d BYTES", tam_bloco);
}

void help(){
    printf("============================ COMANDOS DISPONIVEIS ============================\n"
       "- cat <arquivo>\n"
       "    EXIBE O CONTEUDO DO ARQUIVO INFORMADO.\n"
       "- cd <diretório>\n"
       "    ALTERA O DIRETORIO ATUAL PELO O INFORMADO.\n"
       "- help\n"
       "    MOSTRA A LISTA DE COMANDOS DISPONIVEIS E SUAS DESCRICOES.\n"
       "- ls\n"
       "    LISTA TODOS OS ARQUIVOS E DIRETORIOS PRESENTES NO DIRETORIO ATUAL.\n"
       "- mkdir <nome>\n"
       "    CRIA UM NOVO DIRETORIO DENTRO DO ATUAL.\n"
       "- pwd\n"
       "    EXIBE O CAMINHO COMPLETO DO DIRETORIO ATUAL.\n"
       "- rm <nome>\n"
       "    REMOVE O ARQUIVO OU DIRETORIO DO DIRETORIO ATUAL.\n"
       "- stat\n"
       "    EXIBE INFORMACOES DO SISTEMA DE ARQUIVOS, COMO ESTATISTICAS GERAIS.\n"
       "- touch <arquivo>\n"
       "    CRIA UM NOVO ARQUIVO VAZIO NO DIRETORIO ATUAL.\n"
       "==============================================================================\n"
    );

}
