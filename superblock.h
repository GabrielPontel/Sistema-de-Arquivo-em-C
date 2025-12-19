// Grava inicialmente os dados do superblock.
void gravar_superblock()
{
    FILE *arq;
    arq = fopen("fs/superblock.dat", "r");
    if (arq == NULL)
    {
        arq = fopen("fs/superblock.dat", "w");
        if (validar_abertura_arquivo(arq,"fs/superblock.dat","gravar_superblock"))
        {
            fputs("filesystem=togabifs\n"
                  "blocksize=128\n"
                  "partitionsize=10240\n"
                  "next=00",
                  arq);

            fclose(arq);
        }
    }
}

//Grava no superblock o inode livre, sendo o primeiro da "lista de livres".
void gravar_inode_livre_superblock(int num_inode)
{
    FILE *arq;
    char linha[256], conversor[5];
    int pos;
    sprintf(conversor, "%d", num_inode);
    if (strlen(conversor) == 1)
    {
        conversor[1] = conversor[0];
        conversor[0] = '0';
        conversor[2] = '\0';
    }

    arq = fopen("fs/superblock.dat", "r+"); // abre para leitura e escrita
    if (validar_abertura_arquivo(arq,"fs/superblock.dat","gravar_inode_livre_superblock"))
    {
        fseek(arq, 0, SEEK_END);
        pos = ftell(arq) - 2;
        fseek(arq, pos, SEEK_SET);
        fputs(conversor, arq);
        fclose(arq);
    }
}

//Retorna o num do primeiro inode livre da "lista de livres".
int retornar_inode_livre_superblock()
{
    FILE *arq;
    char linha[100];
    int num_inode = -1;

    arq = fopen("fs/superblock.dat", "r");
    if (validar_abertura_arquivo(arq,"fs/superblock.dat","retornar_inode_livre_superblock"))
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
        {
            if (strncmp(linha, "next=", 5) == 0)
            {
                sscanf(linha + 5, "%d", &num_inode);
            }
        }
        fclose(arq);
    }
    return num_inode;
}

// Retorna o tamanho do blocksize, ou seja, o tamanho de cada bloco do sistema de arquivo.
int retornar_blocksize()
{
    FILE *arq;
    char linha[100];
    int blocksize = -1;

    arq = fopen("fs/superblock.dat", "r");
    if (validar_abertura_arquivo(arq,"fs/superblock.dat","retornar_blocksize"))
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
        {
            if (strncmp(linha, "blocksize=", 10) == 0)
            {
                sscanf(linha + 10, "%d", &blocksize);
            }
        }
        fclose(arq);
    }
    return blocksize;
}

// Retorna o tamanho da partition, ou seja, o tamanho da partição dedicada ao sistema de arquivo.
int retornar_partition()
{
    FILE *arq;
    char linha[100];
    int partition = -1;

    arq = fopen("fs/superblock.dat", "r");

    if (validar_abertura_arquivo(arq,"fs/superblock.dat","retornar_partition"))
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
        {
            if (strncmp(linha, "partitionsize=", 14) == 0)
            {
                sscanf(linha + 14, "%d", &partition);
            }
        }

        fclose(arq);
    }
    return partition;
}

// Retorna a quantidade de blocos o sistema de arquivos contem.
int calcular_quantidade_blocos()
{
    int qtd_blocos;

    qtd_blocos = retornar_partition() / retornar_blocksize();

    return qtd_blocos;
}
