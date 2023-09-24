#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long numCartao = get_long("Nº of Card: ");
    int cont = 0;
    int produtoSub = 0;
    int addProd = 0;
    int addSub = 0;
    int somaNSub = 0;
    int fDigit = 0;
    int sDigit = 0;
    int ultimoDigito;
    while (numCartao != 0)
    {
        cont++;
        sDigit = ultimoDigito;
        ultimoDigito = numCartao % 10;
        fDigit = ultimoDigito;
        if (cont % 2 == 0)
        {
            produtoSub = ultimoDigito * 2;
            if (produtoSub >= 10)
            {
                addProd = produtoSub % 10 + 1;
            }
            else
            {
                addProd = produtoSub;
            }
            addSub = addSub + addProd;
        }
        else
        {
            somaNSub = somaNSub + ultimoDigito;
        }
        numCartao = (numCartao - ultimoDigito) / 10;
    }
    int somaTot = addSub + somaNSub;
    if (cont == 15 && fDigit == 3 && (sDigit == 4 || sDigit == 7) && somaTot % 10 == 0)
    {
        printf("AMEX\n");
    }
    else if (cont == 16 && fDigit == 5 && sDigit >= 1 && sDigit <= 5 && somaTot % 10 == 0)
    {
        printf("MASTERCARD\n");
    }
    else if ((cont == 13 || cont == 16) && fDigit == 4 && somaTot % 10 == 0)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}