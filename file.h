//Fun��o para apagar um arquivo
void apagar_arquivo(int num_inode)
{
    int i;
    tp_inode inode;

    inode = retornar_inode(num_inode);
    for(i = 0; i<inode.qtd_blocos; i++)
    {
        libera_bloco_mapa(inode.blocos[i]);
    }
    libera_inode(inode);
}
