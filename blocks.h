// Cria inicialmente os arquivos blocksXX.dat que representa os blocos do sistema de arquivos.
void criar_blocos()
{
    FILE *arq;
    int i, quantidade_blocos;
    char caminho[30];
    char nomearq[10];
    quantidade_blocos = calcular_quantidade_blocos();
    for (i = 0; i < quantidade_blocos; i++)
    {
        strcpy(caminho, "fs/blocks/");
        sprintf(nomearq, "%d", i);
        strcat(caminho, nomearq);
        strcat(caminho, ".dat");
        arq = fopen(caminho, "r");
        if (arq == NULL)
        {
            arq = fopen(caminho, "w");
            if (arq == NULL)
            {
                printf("\nERRO AO ABRIR O ARQUIVO(BLOCK.DAT) - FUNCAO CRIAR_BLOCOS\n");
            }
        }
        fclose(arq);
    }
}

// Limpa um bloco, apagando fisicamente os arquivos.
void reseta_bloco(int num_bloco)
{
    FILE *origem, *destino;
    char nomearq[10];
    char caminho[30];

    strcpy(caminho, "fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho, nomearq);
    strcat(caminho, ".dat");
    origem = fopen(caminho, "rb");
    destino = fopen("auxiliar.dat", "wb");

    if (origem == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO RESETA BLOCO\n");
    }
    else if (destino == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(AUXILIAR.DAT) - FUNCAO RESETA BLOCO\n");
    }
    else
    {
        fclose(origem);
        fclose(destino);

        remove(caminho);
        rename("auxiliar.dat", caminho);
    }
}

// Grava o texto em um bloco
void gravar_bloco(int num, char string[])
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "w");
    if(arq == NULL)
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO GRAVAR BLOCO\n");
    else
    {
        fputs(string, arq);
        fclose(arq);
    }
}

// Apresenta o conteudo de um determinado bloco do sistema.
void apresentar_bloco(int num)
{
    FILE *arq;
    int tam = retornar_blocksize();
    char nomearq[10], caminho[30], linha[tam];


    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "r");

    if(arq == NULL)
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO APRESENTAR BLOCO\n");
    else
    {
        while(fgets(linha, sizeof(linha), arq) != NULL)
            printf("%s", linha);
        fclose(arq);
    }
}


//Grava a entrada de diretório no final de um determinado bloco, tendo seu numero passado por parametro.
void gravar_entrada_diretorio(int num, tp_entrada_diretorio entrada_diretorio)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "ab");

    if(arq == NULL)
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO GRAVAR ENTRADA DIRETORIO\n");
    else
    {
        fwrite(&entrada_diretorio,sizeof(tp_entrada_diretorio), 1,arq);
        fclose(arq);
    }
}

// Retorna a entrada de diretório de acordo com seu inode e bloco onde se encontra.
tp_entrada_diretorio retornar_entrada_diretorio(int num_bloco, int num_inode_procurado)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio aux;
    int quantidade_entradas;


    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");
    aux.num_inode = -1;

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO RETORNAR ENTRADA DIRETORIO\n");
    }
    else
    {
        fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        while(!feof(arq) && aux.num_inode != num_inode_procurado)
        {
            fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        }
        if(aux.num_inode != num_inode_procurado)
            aux.num_inode = -1;

        fclose(arq);
    }
    return aux;
}

// Retornar a ultima entrada de diretorio do bloco.
tp_entrada_diretorio retornar_ultima_entrada_diretorio(int num_bloco)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio aux,ant;
    int quantidade_entradas;

    ant.num_inode = -1;

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO RETORNAR ENTRADA DIRETORIO\n");
    }
    else
    {
        while(fread(&aux, sizeof(tp_entrada_diretorio), 1, arq))
        {
            ant = aux;

        }
        fclose(arq);
    }
    return ant;
}

// Retorna a quantidade de entradas de um bloco
int retornar_quantidade_entrada_diretorio(int num_bloco)
{
    tp_entrada_diretorio entrada_diretorio;
    char nomearq[10];
    char caminho[30];
    int cont = 0;
    FILE *arq;


    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");

    if(arq == NULL)
    {
        printf("\n ERRO AO ABRIR O ARQUIVO(BLOCO.DAT) - FUNCAO RETORNAR QUANTIDADE ENTRADA DIRETORIO\n");
    }
    else
    {

        fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, arq);
        while(!feof(arq))
        {
            cont++;
            fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, arq);
        }

        fclose(arq);
    }

    return cont;
}

// Exclui uma entrada de um bloco
int excluir_entrada_diretorio(int num_bloco, int num_inode_procurado)
{
    FILE *origem, *destino;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio entrada_diretorio;
    int valida = -1;

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    origem = fopen(caminho, "rb");
    destino = fopen("auxiliar.dat", "wb");

    if(origem == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO EXCLUIR ENTRADA DIRETORIO\n");

    }
    else if(destino == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(AUXILIAR.DAT) - FUNCAO EXCLUIR ENTRADA DIRETORIO\n" );
    }
    else
    {
        while(fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, origem))
        {
            if(entrada_diretorio.num_inode == num_inode_procurado)
            {
                valida = 1;
            }
            else{
                fwrite(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, destino);
            }
        }

        fclose(origem);
        fclose(destino);

        remove(caminho);
        rename("auxiliar.dat", caminho);
    }
    return valida;
}

// Apresenta as entradas de diretório de um bloco.
void apresentar_entrada(int num_bloco)
{
    FILE *arq;
    char nomearq[10];
    char caminho[30];
    tp_entrada_diretorio aux;

    strcpy(caminho,"fs/blocks/");
    sprintf(nomearq, "%d", num_bloco);
    strcat(caminho,nomearq);
    strcat(caminho,".dat");
    arq = fopen(caminho, "rb");

    if(arq == NULL)
    {
        printf("\nERRO AO ABRIR ARQUIVO(BLOCO.DAT) - FUNCAO APRESENTAR ENTRADA\n");

    }
    else
    {
        fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        while(!feof(arq) )
        {
            printf("\n\n%d", aux.num_inode);
            printf("\n%s", aux.nome);
            fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        }
    }
}
