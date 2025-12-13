// Determinar quantos caracteres seram necessarios para representar os blocos no mapa de bits.
int calcular_quantidade_caracteres()
{
    int quantidade_blocos, quantidade_caracteres;

    quantidade_blocos = calcular_quantidade_blocos();
    // Lembrando que utilizarei unsigned cgar, sendo assim cada caracter representa 8 bits
    quantidade_caracteres = quantidade_blocos / 8;
    if (quantidade_blocos % 8 != 0)
        quantidade_caracteres++;
    return quantidade_caracteres;
}

// 1 - ocupado
// 0 - livre
// Grava inicialmente os bits que representa os blocos como limpos no arquivo freespace.dat.
void inicializar_mapa_bits()
{
    FILE *arq;
    int i, quantidade_caracteres;
    unsigned char zero = 0;

    quantidade_caracteres = calcular_quantidade_caracteres();
    arq = fopen("fs/freespace.dat", "rb");
    if (arq == NULL)
    {
        arq = fopen("fs/freespace.dat", "wb");
        if (arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO (FREESPACE.DAT) - FUNCAO INICIALIZAR MAPA BITS\n");
        }
        else
        {
            for (i = 0; i < quantidade_caracteres; i++)
            {
                fwrite(&zero, sizeof(unsigned char), 1, arq);
            }
            fclose(arq);
        }
    }
}

// Retorna o mapa de bits, ou seja, um vetor de unsigned char.
void obter_mapa_bits(unsigned char mapa[])
{
    int tam, i = 0;
    FILE *arq;
    tam = calcular_quantidade_blocos();
    arq = fopen("fs/freespace.dat", "rb");
    if (arq == NULL)
    {
        printf("\nERRO AO ABRIR O ARQUIVO (freespace.dat) - funcao obter_mapa_bits\n");
    }
    else
    {
        while (fread(&mapa[i], sizeof(unsigned char), 1, arq) == 1)
        {
            i++;
        }
        fclose(arq);
    }
}

// Libera um bloco no mapa de bits, ou seja, coloca 0 no bit que o representa.
void libera_bloco_mapa(int num_bloco)
{
    FILE *arq;
    int quantidade_blocos, posicao_caracter, posicao_bit;
    unsigned char desejado, validar;

    quantidade_blocos = calcular_quantidade_blocos();
    if (num_bloco < quantidade_blocos && num_bloco > -1)
    {
        arq = fopen("fs/freespace.dat", "rb+");
        if (arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO (FREESPACE.DAT) - FUNCAO LIBERA BLOCO MAPA\n");
        }
        else
        {
            posicao_caracter = num_bloco / 8;
            posicao_bit = (((num_bloco % 8) - 8) * -1) - 1;

            fseek(arq, posicao_caracter, SEEK_SET);
            fread(&validar, sizeof(unsigned char), 1, arq);

            desejado = validar ^ (1 << posicao_bit);

            if (validar & (1 << posicao_bit))
            {
                fseek(arq, posicao_caracter, SEEK_SET);
                fwrite(&desejado, sizeof(unsigned char), 1, arq);
            }
            else
            {
                printf("\nO BLOCO DESEJADO JA ESATVA LIBERADO");
            }
            fclose(arq);
        }
    }
    else
    {
        printf("\nBLOCO INVALIDO, DEVE ESTAR NO INTERVALO DE 0 A %d\n", quantidade_blocos);
    }
}

// Ocupa um bloco no mapa de bits, ou seja, coloca 1 no bit que o representa.
void grava_bloco_ocupado_mapa(int num_bloco)
{
    FILE *arq;
    int quantidade_blocos, posicao_caracter, posicao_bit;
    unsigned char desejado, validar;

    quantidade_blocos = calcular_quantidade_blocos();
    if (num_bloco < quantidade_blocos && num_bloco > -1)
    {
        arq = fopen("fs/freespace.dat", "rb+");
        if (arq == NULL)
        {
            printf("\nERRO AO ABRIR O ARQUIVO (FREESPACE.DAT) - FUNCAO GRAVA BLOCO OCUPADO MAPA\n");
        }
        else
        {
            posicao_caracter = num_bloco / 8;
            posicao_bit = (((num_bloco % 8) - 8) * -1) - 1;

            fseek(arq, posicao_caracter, SEEK_SET);
            fread(&validar, sizeof(unsigned char), 1, arq);
            if ((validar & (1 << posicao_bit)) == 0)
            {
                desejado = validar | (1 << posicao_bit);
                fseek(arq, posicao_caracter, SEEK_SET);
                fwrite(&desejado, sizeof(unsigned char), 1, arq);
            }
            else
            {
                printf("\nO BLOCO DESEJADO JA ESTAVA OCUPADO");
            }
            fclose(arq);
        }
    }
    else
    {
        printf("\nBLOCO INVALIDO, DEVE ESTAR NO INTERVALO DE 0 A %d\n", quantidade_blocos);
    }
}

// Busca em memória no mapa de bits, qual o primeiro bit livre para ocupa-lo.
int ocupa_bloco_mapa()
{
    int num, i, tam, j;
    tam = calcular_quantidade_blocos();
    unsigned char mapa[tam];

    i = 0;
    num = -1;
    obter_mapa_bits(mapa);
    while (i < tam && num == -1)
    {
        j = 7;
        while (j > 0 && (mapa[i] & (1 << j)) != 0)
        {
            j--;
        }
        if (j > 0 && (mapa[i] & (1 << j)) == 0)
        {
            num = i * 8 + ((j - 8) * -1) - 1;
            grava_bloco_ocupado_mapa(num);
        }
        i++;
    }
    return num;
}

// Retorna a quantidade de blocos livres, ou seja, quantidade de bits em 0 no mapa de bits.
int calcular_quant_blocos_livres()
{
    int qtd = 0, i, j, tam;
    unsigned char *mapa;

    tam = calcular_quantidade_caracteres();
    mapa = (unsigned char *)malloc(sizeof(unsigned char) * tam);
    obter_mapa_bits(mapa);

    for (i = 0; i < tam; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((mapa[i] & (1 << j)) == 0)
                qtd++;
        }
    }
    free(mapa);
    return qtd;
}
