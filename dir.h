// Grava uma entrada de diretório no diretório pai.
int gravar_entrada_diretorio_pai(int num_inode, tp_entrada_diretorio entrada_filho)
{
    tp_inode inode_pai;
    int i, quantidade_entrada, max,verifica=1,valida=1;
    i=0;
    inode_pai = retornar_inode(num_inode);
    max = retornar_blocksize()/sizeof(tp_entrada_diretorio);
    quantidade_entrada = retornar_quantidade_entrada_diretorio(inode_pai.blocos[i]);
    while(i < inode_pai.qtd_blocos && quantidade_entrada >= max)
    {
        i++;
        if(i<inode_pai.qtd_blocos)
            quantidade_entrada = retornar_quantidade_entrada_diretorio(inode_pai.blocos[i]);
    }
    if(i==10)
    {
        printf("\nNAO FOI POSSIVEL ARMAZENAR ESSE DIRETORIO JA QUE SEU PAI ESTA CHEIO\n");
        verifica=-1;
    }
    else
    {
        if(i == inode_pai.qtd_blocos)
        {
            inode_pai.blocos[i]=ocupa_bloco_mapa();
            if(inode_pai.blocos[i] == -1)
            {
                printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO CONSEQUENTEMENTE NAO FOI POSSIVEL CRIAR UM DIRETORIO");
                verifica = -1;
            }
            reseta_bloco(inode_pai.blocos[i]);
            inode_pai.qtd_blocos++;
            inode_pai.tamanho += retornar_blocksize();

        }
        if(verifica == 1)
            gravar_entrada_diretorio(inode_pai.blocos[i], entrada_filho);
    }
    atualiza_inode(inode_pai);
    return verifica;
}

//Fun��o que le as entradas de diretorio de um arquivo e coloca em uma arvore binaria
tp_no* ler_todas_entradas_inode(int num_inode, tp_no *raiz)
{
    tp_entrada_diretorio auxiliar;
    tp_inode inode;
    int bloco_desejado, i;
    char nomearq[15];
    char caminho[30];
    FILE *arq;

    inode = retornar_inode(num_inode);

    if (inode.qtd_blocos <= 0)
    {
        raiz = NULL;
    }
    else
    {
        for (i = 0; i < inode.qtd_blocos; i++)
        {
            bloco_desejado = inode.blocos[i];
            strcpy(caminho,"fs/blocks/");
            sprintf(nomearq, "%d", bloco_desejado);
            strcat(caminho, nomearq);
            strcat(caminho,".dat");
            arq = fopen(caminho, "rb");
            if (arq == NULL)
            {
                printf("\nERRO AO ABRIR ARQUIVO (%s) - funcao ler_todas_entradas_inode\n", nomearq);
            }
            else
            {
                while (fread(&auxiliar, sizeof(tp_entrada_diretorio), 1, arq) == 1)
                {
                    raiz = inserir_arvore(raiz,auxiliar);
                }
                fclose(arq);
            }
        }
    }
    return raiz;
}

//Fun��o que vai verificar se o nome esta ou nao no diretorio
int verificar_nome_diretorio(int num_dir, char *nome_arq)
{
    int valida;
    tp_no *raiz;

    valida = 1;
    inicializar_arvore(&raiz);
    raiz = ler_todas_entradas_inode(num_dir, raiz);
    if(pesquisa_entrada_diretorio(raiz,nome_arq) == NULL)
        valida = 0;
    return valida;
}

//Fun��o para apagar um diret�rio
void apagar_diretorio(int num_inode)
{
    tp_no *raiz;
    tp_inode inode;
    tp_entrada_diretorio *vetor_entradas;
    int i=0, tl;

    inicializar_arvore(&raiz);

    raiz = ler_todas_entradas_inode(num_inode,raiz);
    tl = contar_nos(raiz);
    vetor_entradas = (tp_entrada_diretorio*) malloc(sizeof(tp_entrada_diretorio)*tl);
    arvore_para_vetor(raiz,vetor_entradas,&i);
    for(i=0; i<tl; i++)
    {

        if(strcmp(vetor_entradas[i].nome,".")!=0 && strcmp(vetor_entradas[i].nome,"..")!=0)
        {
            inode = retornar_inode(vetor_entradas[i].num_inode);
            if(inode.tipo == 'd')
            {
                apagar_diretorio(inode.num);
            }
            else
            {
                apagar_arquivo(inode.num);
            }
        }
    }
    free(vetor_entradas);
    inode = retornar_inode(num_inode);
    for(i=0; i<inode.qtd_blocos; i++)
    {
        libera_bloco_mapa(inode.blocos[i]);
    }
    libera_inode(inode);
}

//Fun��o para remover corretamente a entrada de diretorio do arquivo
void remover_entrada_do_diretorio(int num_inode_atual, int num_inode_apagar)
{
    tp_no *raiz;
    tp_inode inode;
    tp_entrada_diretorio aux;
    int i,valida=-1;

    inode = retornar_inode(num_inode_atual);

    i=0;
    while(i<inode.qtd_blocos && valida == -1)
    {
        valida = excluir_entrada_diretorio(inode.blocos[i], num_inode_apagar);
        if(valida==-1)
            i++;
    }
    if(i != inode.qtd_blocos-1)
    {
        if(retornar_quantidade_entrada_diretorio(inode.blocos[inode.qtd_blocos-1])>0)
        {
            aux = retornar_ultima_entrada_diretorio(inode.blocos[inode.qtd_blocos-1]);
            gravar_entrada_diretorio(inode.blocos[i],aux);
            excluir_entrada_diretorio(inode.blocos[inode.qtd_blocos-1], aux.num_inode);
        }

    }
    if(retornar_quantidade_entrada_diretorio(inode.blocos[inode.qtd_blocos-1])==0)
    {
        libera_bloco_mapa(inode.blocos[inode.qtd_blocos-1]);
        inode.qtd_blocos-=1;
        inode.tamanho -= retornar_blocksize();
        atualiza_inode(inode);
    }
}
