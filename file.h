// Apaga um arquivo, liberando seus blocos.
void apagar_arquivo(int num_inode)
{
    int i;
    tp_inode inode;

    inode = retornar_inode(num_inode);
    for (i = 0; i < inode.qtd_blocos; i++)
    {
        libera_bloco_mapa(inode.blocos[i]);
    }
    libera_inode(inode);
}

// Le o texto digitado pelo usuário até ele digitar CTRL+D e o armazena em uma string.
void ler_arquivo(char conteudo[], int tam)
{
    int c;
    int i = 0;
    printf("DIGITE O CONTEUDO DO ARQUIVO: \n");
#ifdef __linux__
    printf("USE CTRL+D NA PROXIMA LINHA PARA ENCERRAR \n");
    while ((c = getchar()) != EOF && i < tam - 1)
    {
        if (c == 27)
        {
            if (i + 3 < tam - 1)
            {
                conteudo[i++] = '^';
                c = getchar();
                conteudo[i++] = (char)c;
                conteudo[i++] = (char)c;
            }
        }
        else
        {
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
    } while (c != 4);
    getchar();
    i--;
    conteudo[i] = '\0';
#endif
}
