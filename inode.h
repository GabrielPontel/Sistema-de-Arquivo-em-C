// Grava inicialmente os inodes limpos no arquivo inodes.dat.
void inicializar_inodes()
{
    FILE *arq;
    tp_inode inode;
    int i, quantidade_inode;
    quantidade_inode = calcular_quantidade_blocos() / 2;
    arq = fopen("fs/inodes.dat", "rb");
    if (arq == NULL)
    {
        arq = fopen("fs/inodes.dat", "wb");
        if (validar_abertura_arquivo(arq, "fs/inodes.dat", "inicializar_inodes"))
        {
            inode.tipo = 'u';
            for (i = 0; i < quantidade_inode - 1; i++)
            {
                inode.num = i;
                inode.prox_livre = i + 1;
                fwrite(&inode, sizeof(tp_inode), 1, arq);
            }
            inode.num = quantidade_inode - 1;
            inode.prox_livre = -1;
            fwrite(&inode, sizeof(tp_inode), 1, arq);
            fclose(arq);
        }
    }
}

// Retorna uma struct do primeiro inode livre na lista, caso a lista não esteja vazia.
tp_inode retornar_inode_livre()
{
    FILE *arq;
    int quantidade_inode, num_inode;
    tp_inode inode;
    arq = fopen("fs/inodes.dat", "rb");
    quantidade_inode = calcular_quantidade_blocos() / 2;
    inode.tipo = '-';
    num_inode = retornar_inode_livre_superblock();
    if (num_inode != -1)
    {
        if (validar_abertura_arquivo(arq, "fs/inodes.dat", "retornar_inode_livre"))
        {
            fseek(arq, num_inode * sizeof(tp_inode), SEEK_SET);
            fread(&inode, sizeof(tp_inode), 1, arq);
            fclose(arq);
        }
        gravar_inode_livre_superblock(inode.prox_livre);
    }
    return inode;
}

// Retorna um inode a partir de seu num.
tp_inode retornar_inode(int num)
{
    FILE *arq;
    tp_inode inode;
    arq = fopen("fs/inodes.dat", "rb");

    inode.tipo = '-';
    if (validar_abertura_arquivo(arq, "fs/inodes.dat", "retornar_inode"))
    {
        fseek(arq, num * sizeof(tp_inode), SEEK_SET);
        fread(&inode, sizeof(tp_inode), 1, arq);
        fclose(arq);
    }
    return inode;
}

// Atualiza um inode no arquivo.
void atualiza_inode(tp_inode inode)
{
    FILE *arq;
    arq = fopen("fs/inodes.dat", "rb+");

    if (validar_abertura_arquivo(arq, "fs/inodes.dat", "atualiza_inode"))
    {
        fseek(arq, inode.num * sizeof(tp_inode), SEEK_SET);
        fwrite(&inode, sizeof(tp_inode), 1, arq);
        fclose(arq);
    }
}

// Libera o inode mandado por parametro.
void libera_inode(tp_inode inode)
{
    FILE *arq;
    arq = fopen("fs/inodes.dat", "rb+");
    inode.prox_livre = retornar_inode_livre_superblock();
    inode.qtd_blocos = 0;
    inode.tipo = 'u';
    inode.tamanho = 0;
    gravar_inode_livre_superblock(inode.num);
    if (validar_abertura_arquivo(arq, "fs/inodes.dat", "libera_inode"))
    {
        fseek(arq, inode.num * sizeof(tp_inode), SEEK_SET);
        fwrite(&inode, sizeof(tp_inode), 1, arq);
        fclose(arq);
    }
}
