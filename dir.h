void apagar_diretorio(int num_inode);

// Grava a entrada de diretório no final de um determinado bloco, tendo seu numero passado por parametro.
void gravar_entrada_diretorio(int num, tp_entrada_diretorio entrada_diretorio)
{
    FILE *arq;
    char caminho[30];
    converter_inteiro_block(num, caminho);
    arq = fopen(caminho, "ab");

    if (validar_abertura_arquivo(arq, caminho, "gravar_entrada_diretorio"))
    {
        fwrite(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, arq);
        fclose(arq);
    }
}

// Retorna a entrada de diretório de acordo com seu inode e bloco onde se encontra.
tp_entrada_diretorio retornar_entrada_diretorio(int num_bloco, int num_inode_procurado)
{
    FILE *arq;
    char caminho[30];
    tp_entrada_diretorio aux;
    int quantidade_entradas;

    converter_inteiro_block(num_bloco, caminho);
    arq = fopen(caminho, "rb");
    aux.num_inode = -1;

    if (validar_abertura_arquivo(arq, caminho, "retornar_entrada_diretorio"))
    {
        fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        while (!feof(arq) && aux.num_inode != num_inode_procurado)
        {
            fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        }
        if (aux.num_inode != num_inode_procurado)
            aux.num_inode = -1;

        fclose(arq);
    }
    return aux;
}

// Retornar a ultima entrada de diretorio do bloco.
tp_entrada_diretorio retornar_ultima_entrada_diretorio(int num_bloco)
{
    FILE *arq;
    char caminho[30];
    tp_entrada_diretorio aux, ant;
    int quantidade_entradas;

    ant.num_inode = -1;

    converter_inteiro_block(num_bloco, caminho);
    arq = fopen(caminho, "rb");

    if (validar_abertura_arquivo(arq, caminho, "retornar_ultima_entrada_diretorio"))
    {
        while (fread(&aux, sizeof(tp_entrada_diretorio), 1, arq))
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
    char caminho[30];
    int cont, tamanho_arquivo;
    FILE *arq;

    converter_inteiro_block(num_bloco, caminho);
    arq = fopen(caminho, "rb");

    if (validar_abertura_arquivo(arq, caminho, "retornar_quantidade_entrada_diretorio"))
    {
        // Vai para o final do arquivo e retorna o tamanho.
        fseek(arq, 0, SEEK_END);
        tamanho_arquivo = ftell(arq);
        // Calcula a quantidade de entradas.
        cont = tamanho_arquivo / sizeof(tp_entrada_diretorio);

        fclose(arq);
    }
    return cont;
}

// Exclui uma entrada de um bloco
int excluir_entrada_diretorio(int num_bloco, int num_inode_procurado)
{
    FILE *origem, *destino;
    char caminho[30];
    tp_entrada_diretorio entrada_diretorio;
    int valida = -1;

    converter_inteiro_block(num_bloco, caminho);
    origem = fopen(caminho, "rb");
    destino = fopen("auxiliar.dat", "wb");

    if (validar_abertura_arquivo(origem, caminho, "excluir_entrada_diretorio") && validar_abertura_arquivo(destino, "auxiliar.dat", "excluir_entrada_diretorio"))
    {
        while (fread(&entrada_diretorio, sizeof(tp_entrada_diretorio), 1, origem))
        {
            if (entrada_diretorio.num_inode == num_inode_procurado)
            {
                valida = 1;
            }
            else
            {
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
    char caminho[30];
    tp_entrada_diretorio aux;

    converter_inteiro_block(num_bloco, caminho);
    arq = fopen(caminho, "rb");

    if (validar_abertura_arquivo(arq, caminho, "apresentar_entrada"))
    {
        fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        while (!feof(arq))
        {
            printf("\n\n%d", aux.num_inode);
            printf("\n%s", aux.nome);
            fread(&aux, sizeof(tp_entrada_diretorio), 1, arq);
        }
    }
}

// Grava uma entrada de diretório no diretório pai.
int gravar_entrada_diretorio_pai(int num_inode, tp_entrada_diretorio entrada_filho)
{
    tp_inode inode_pai;
    int i, quantidade_entrada, max, verifica = 1, valida = 1;
    i = 0;
    inode_pai = retornar_inode(num_inode);
    max = retornar_blocksize() / sizeof(tp_entrada_diretorio);
    quantidade_entrada = retornar_quantidade_entrada_diretorio(inode_pai.blocos[i]);
    while (i < inode_pai.qtd_blocos && quantidade_entrada >= max)
    {
        i++;
        if (i < inode_pai.qtd_blocos)
            quantidade_entrada = retornar_quantidade_entrada_diretorio(inode_pai.blocos[i]);
    }
    if (i == 10)
    {
        printf("\nNAO FOI POSSIVEL ARMAZENAR ESSE DIRETORIO JA QUE SEU PAI ESTA CHEIO\n");
        verifica = -1;
    }
    else
    {
        if (i == inode_pai.qtd_blocos)
        {
            inode_pai.blocos[i] = ocupa_bloco_mapa();
            if (inode_pai.blocos[i] == -1)
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
                verifica = -1;
            }
            reseta_bloco(inode_pai.blocos[i]);
            inode_pai.qtd_blocos++;
            inode_pai.tamanho += retornar_blocksize();
        }
        if (verifica == 1)
            gravar_entrada_diretorio(inode_pai.blocos[i], entrada_filho);
    }
    atualiza_inode(inode_pai);
    return verifica;
}

// Organiza todas entradas de diretório de um diretório em uma árvore binária.
tp_no *ler_todas_entradas_inode(int num_inode)
{
    tp_entrada_diretorio auxiliar;
    tp_inode inode;
    tp_no *raiz;
    int bloco_desejado, i;
    char nomearq[15];
    char caminho[30];
    FILE *arq;

    inicializar_arvore(&raiz);
    inode = retornar_inode(num_inode);

    if (inode.qtd_blocos <= 0)
    {
        raiz = NULL;
    }
    else
    {
        // Abre cada bloco do arquivo e armazena a suas entradas
        for (i = 0; i < inode.qtd_blocos; i++)
        {
            bloco_desejado = inode.blocos[i];
            converter_inteiro_block(bloco_desejado, caminho);
            arq = fopen(caminho, "rb");
            if (validar_abertura_arquivo(arq, caminho, "ler_todas_entradas_inode"))
                while (fread(&auxiliar, sizeof(tp_entrada_diretorio), 1, arq) == 1)
                {
                    raiz = inserir_arvore(raiz, auxiliar);
                }
            fclose(arq);
        }
    }

    return raiz;
}

// Percorre um diretório apagando os arquivos e diretóriuos de dentro pela árvore.
void apagar_dentro_diretorio(tp_no *raiz)
{
    tp_inode inode;
    if (raiz != NULL)
    {
        if (strcmp(raiz->info.nome, ".") != 0 && strcmp(raiz->info.nome, "..") != 0)
        {
            inode = retornar_inode(raiz->info.num_inode);
            if (inode.tipo == 'd')
            {
                apagar_diretorio(inode.num);
            }
            else
            {
                apagar_arquivo(inode.num);
            }
        }
        apagar_dentro_diretorio(raiz->dir);
        apagar_dentro_diretorio(raiz->esq);
    }
}

// Apaga um diretório.
void apagar_diretorio(int num_inode)
{
    tp_no *raiz;
    tp_inode inode;
    tp_entrada_diretorio *vetor_entradas;
    int i = 0, tl;

    raiz = ler_todas_entradas_inode(num_inode);
    apagar_dentro_diretorio(raiz);
    inode = retornar_inode(num_inode);
    for (i = 0; i < inode.qtd_blocos; i++)
    {
        libera_bloco_mapa(inode.blocos[i]);
    }
    libera_inode(inode);
}

// Remover uma entrada de diretorio do diretório.
void remover_entrada_do_diretorio(int num_inode_atual, int num_inode_apagar)
{
    tp_no *raiz;
    tp_inode inode;
    tp_entrada_diretorio aux;
    int i, valida = -1;

    inode = retornar_inode(num_inode_atual);

    i = 0;
    while (i < inode.qtd_blocos && valida == -1)
    {
        valida = excluir_entrada_diretorio(inode.blocos[i], num_inode_apagar);
        if (valida == -1)
            i++;
    }
    if (i != inode.qtd_blocos - 1)
    {
        if (retornar_quantidade_entrada_diretorio(inode.blocos[inode.qtd_blocos - 1]) > 0)
        {
            aux = retornar_ultima_entrada_diretorio(inode.blocos[inode.qtd_blocos - 1]);
            gravar_entrada_diretorio(inode.blocos[i], aux);
            excluir_entrada_diretorio(inode.blocos[inode.qtd_blocos - 1], aux.num_inode);
        }
    }
    if (retornar_quantidade_entrada_diretorio(inode.blocos[inode.qtd_blocos - 1]) == 0)
    {
        libera_bloco_mapa(inode.blocos[inode.qtd_blocos - 1]);
        inode.qtd_blocos -= 1;
        inode.tamanho -= retornar_blocksize();
        atualiza_inode(inode);
    }
}

// Cria o diretório raiz caso seja necessário.
void criar_diretorio_raiz()
{
    int valida;
    tp_inode inode;
    tp_entrada_diretorio entrada_diretorio;

    inode = retornar_inode_livre();
    if (inode.num == 0)
    {
        inode.blocos[0] = ocupa_bloco_mapa();
        if (valida_novo_bloco(inode.blocos[0]))
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
