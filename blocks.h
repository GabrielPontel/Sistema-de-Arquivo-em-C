// Cria inicialmente os arquivos blocksXX.dat que representa os blocos do sistema de arquivos.
void criar_blocos()
{
    FILE *arq;
    int i, quantidade_blocos;
    char caminho[30];
    quantidade_blocos = calcular_quantidade_blocos();
    for (i = 0; i < quantidade_blocos; i++)
    {
        converter_inteiro_block(i, caminho);
        arq = fopen(caminho, "r");
        if (arq == NULL)
        {
            arq = fopen(caminho, "w");
            if(validar_abertura_arquivo(arq,caminho,"criar_blocos"));
        }
        fclose(arq);
    }
}

// Limpa um bloco, apagando fisicamente os arquivos.
void reseta_bloco(int num_bloco)
{
    FILE *origem, *destino;
    char caminho[30];
    converter_inteiro_block(num_bloco, caminho);
    origem = fopen(caminho, "rb");
    destino = fopen("auxiliar.dat", "wb");

    if (validar_abertura_arquivo(origem,caminho,"reseta_bloco")&&validar_abertura_arquivo(destino,"auxiliar.dat","reseta_bloco"))
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
    char caminho[30];
    converter_inteiro_block(num, caminho);
    arq = fopen(caminho, "w");
    if (validar_abertura_arquivo(arq,caminho,"gravar_bloco"))
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
    char caminho[30], linha[tam];

    converter_inteiro_block(num, caminho);
    arq = fopen(caminho, "r");

    if (validar_abertura_arquivo(arq,caminho,"apresentar_bloco"))
    {
        while (fgets(linha, sizeof(linha), arq) != NULL)
            printf("%s", linha);
        fclose(arq);
    }
}
