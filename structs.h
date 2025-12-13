//Estrutura para armazenar uma entrada de diretorio.
typedef struct
{
    char nome[15];
    int num_inode;
} tp_entrada_diretorio;

//N� da arvore de diret�rios
typedef struct no
{
    tp_entrada_diretorio info;
    struct no *esq;
    struct no *dir;
}
tp_no;

//Estrutura na qual armazenara os dados de um inode.
typedef struct
{
    int num;
    int qtd_blocos;
    int tamanho;
    char tipo;
    int blocos[10];
    int prox_livre; //Armazena o num do proximo inode livre
} tp_inode;
