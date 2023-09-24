#include <cs50.h>
#include <stdio.h>
using namespace std;

int count_sentences(char[]);
int count_letters(char[]);

int main(void){
    char cadena[200];
    int z, espace=0,letters=0;

    cout<<"Enter String :\n";
    cin.getline(cadena,200);
    cout<<"The entered text string contains "<<count_sentences(cadena)<<" works.";
    //cout<<"The entered text string contains "<<count_letters(cadena)<<" Letters.";
    return 0;
}

int count_sentences(char cad[]){
    int i,words = 0;
    for(i=1; cad[i]!='\0'; ++i){
        if(cad[i-1]==' '&&isalpha(cad[i])){
            ++words;
        }
    }

    if(isalpha(cad[0])) ++words;
    return words;
}