tp_no *pesquisa_entrada_diretorio(tp_no *raiz, char nome_arquivo[]);
tp_no *ler_todas_entradas_inode(int num_inode);
void reseta_bloco(int num_bloco);

// Converte todas as letras para maiusculas
void trocar_para_maiusculo(char string[])
{
    int i, tam;
    tam = strlen(string);
    for (i = 0; i < tam; i++)
    {
        string[i] = toupper(string[i]);
    }
}

// Converte um inteiro para o nome do arquivo block desse inteiro.
// Ex : int = X nomearq = fs/blocks/X.dat.
void converter_inteiro_block(int numero, char caminho[])
{
    char nomearq[10];
    caminho[0] = '\0';
    strcpy(caminho, "fs/blocks/");
    sprintf(nomearq, "%d", numero);
    strcat(caminho, nomearq);
    strcat(caminho, ".dat");
}

// Valida se o arquivo foi aberto corretamente, caso contrario imprime uma mensagem de erro.
int validar_abertura_arquivo(FILE *arq, char nome_arq[], char nome_funcao[])
{
    int valida = 1;
    if (arq == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO %s NA FUNCAO %s.", arq, nome_funcao);
        valida = 0;
    }
    return valida;
}

// Verifica o tamanho do nome do arquivo, retornando 1 se conter entre 0 e 15 caracteres
// E 0 caso contrario.
int validar_nome_arquivo(char nome_arq[])
{
    int valida = 0;
    if (strlen(nome_arq) > 14)
    {
        printf("O NOME DO ARQUIVO EXECEDE 14 CARACTERES OU ESTA VAZIO.\n");
    }
    else if (strlen(nome_arq) == 0)
    {
        printf("O NOME DO ARQUIVO NAO PODE SER NULO.");
    }
    else
    {
        valida = 1;
    }
    return valida;
}

// Valida inode considerado valido se seu tipo for diferente de '-' retornando 1, caso contrario 0.
int valida_inode(tp_inode inode)
{
    int valida = 1;
    if (inode.tipo == '-')
    {
        printf("\nNAO FOI POSSIVEL ALOCAR UM INODE.");
        valida = 0;
    }
    return valida;
}

// Valida a alocação de um novo bloco retornando 1 o bloco for diferente de -1, caso contrario 0.
int valida_novo_bloco(int num_bloco)
{
    int valida = 1;
    if (num_bloco != -1)
    {
        reseta_bloco(num_bloco);
    }
    else
    {
        valida = 0;
        printf("\nNAO FOI POSSIVEL ALOCAR UM BLOCO.");
    }
    return valida;
}

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

// Realiza uma leitura até o usuário digitar enter ou atingir o tamanho máximo.
void realizar_leitura(char string[], int tam)
{
    int i = -1;
    char c;
    do
    {
        i++;
        scanf("%c", &c);
        string[i] = c;
    } while (i < tam && c != '\n');
    string[i] = '\0';
}

// Separa o texto digitado pelo usuário em comando e restante.
void separar_prompt_usuario(char texto_digitado[], char comando[])
{
    int i = 0, j;
    comando[0] = '\0';
    while (i < strlen(texto_digitado) && texto_digitado[i] != ' ' && texto_digitado[i] != '/')
    {
        comando[i] = texto_digitado[i];
        i++;
    }
    comando[i] = '\0';
    if (texto_digitado[i] != '\0')
        i++;
    for (j = 0; j < strlen(texto_digitado); j++)
    {
        texto_digitado[j] = texto_digitado[i];
        i++;
    }
    texto_digitado[j] = '\0';
}

// Verificar se o nome escontra-se no diretório atual, se sim retorna 0, caso contrario 1.
int verificar_nome_diretorio(int num_dir, char *nome_arq)
{
    int valida;
    tp_no *raiz;

    valida = 1;
    raiz = ler_todas_entradas_inode(num_dir);
    if (pesquisa_entrada_diretorio(raiz, nome_arq) != NULL)
    {
        valida = 0;
        printf("\nJA EXISTE ARQUIVO COM ESSE NOME NO DIRETORIO!!");
    }
    return valida;
}
