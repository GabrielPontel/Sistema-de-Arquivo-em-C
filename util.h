// Cria o diretorio fs e blocks no windows e linux
void criar_diretorio()
{
#ifdef __linux__
    mkdir("fs", 0755);
    mkdir("fs/blocks", 0755);
#elif _WIN32
    _mkdir("fs");
    _mkdir("fs/blocks");
#endif
}

// Cria o diretório raiz caso seja necessário.
void criar_diretorio_raiz()
{
    int valida;
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;

    inode = retornar_inode_livre();
    if(inode.num == 0)
    {
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
            inode.tipo = 'd';
            inode.qtd_blocos = 1;
            inode.tamanho = retornar_blocksize();

            strcpy(entrada_diretorio.nome, ".");
            entrada_diretorio.num_inode = inode.num;
            gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);

            strcpy(entrada_diretorio.nome, "..");
            entrada_diretorio.num_inode = inode.num;
            gravar_entrada_diretorio(inode.blocos[0], entrada_diretorio);
            atualiza_inode(inode);
        }
    }
}

// inicializa todos os arquivos necessários ao sistema de arquivos.
void inicializar_dados()
{
    criar_diretorio();
    gravar_superblock();
    inicializar_mapa_bits();
    inicializar_inodes();
    criar_blocos();
    criar_diretorio_raiz();
}

// Realiza uma leitura até o usuário digitar enter ou atingir o tamanho máximo.
void realizar_leitura(char string[], int tam)
{
    int i=-1;
    char c;
    do
    {
        i++;
        scanf("%c", &c);
        string[i] = c;
    }
    while(i < tam && c != '\n');
    string[i]='\0';
}

// Separa o texto digitado pelo usuário em comando e restante.
void separar_prompt_usuario(char texto_digitado[], char comando[])
{
    int i = 0, j;
    comando[0]='\0';
    while( i < strlen(texto_digitado) && texto_digitado[i] != ' ' && texto_digitado[i] != '/')
    {
        comando[i] = texto_digitado[i];
        i++;
    }
    comando[i]='\0';
    if(texto_digitado[i] != '\0')
        i++;
    for(j=0; j<strlen(texto_digitado); j++)
    {
        texto_digitado[j] = texto_digitado[i];
        i++;
    }
    texto_digitado[j]='\0';
}
