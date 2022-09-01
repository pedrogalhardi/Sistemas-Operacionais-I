#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct vetor{
    int valor;
    int distancia;
}TmoduloVetor;

typedef struct numero{
    int valor;
    int flag;
}TmoduloNumero;

typedef struct paginas{
    TmoduloNumero *vetor;
    int numeroPaginas;
}TmoduloPagina;


void contarPaginas(char *nomeArquivo, int *numero);
void lerArquivo(char *nomeArquivo, TmoduloPagina *auxiliar, int *quadros, int numero);
int FIFO(TmoduloPagina auxiliar, int quadros);
void procuraOTM(TmoduloPagina auxiliar, TmoduloVetor *vetor, int quadros, int x);
int OTM(TmoduloPagina auxiliar, int quadros);
int LRU(TmoduloPagina auxiliar, int quadros);

//--- fun��o auxiliar para contar as linnhas do arquivo fornecido, ou seja, conta quantas p�ginas tem no arquivo
void contarPaginas(char *nomeArquivo, int *numero){
    char c;
    char letra = '\n';

    FILE *file_descriptor;
	file_descriptor = fopen(nomeArquivo, "r");
	if(file_descriptor == NULL){
        printf("Problemas na abertura do arquivo ou arquivo inexistente!\n");
        exit(1);
	}
    while(fread(&c, sizeof(char), 1, file_descriptor)){
        if(c == letra){
            *numero = *numero+1;
        }
	}
	fclose(file_descriptor);
}

//--- fun��o que faz a leitura do arquivo e insere os valores lidos em suas respectivas vari�veis, primeiro valor n� de quadros e o restante p�ginas
void lerArquivo(char *nomeArquivo, TmoduloPagina *auxiliar, int *quadros, int numero){
    int pagina, i = 0;
    auxiliar->vetor = malloc(numero *sizeof(TmoduloNumero));
    memset(auxiliar->vetor, 0, numero *sizeof(auxiliar->vetor[0]));
    auxiliar->numeroPaginas = numero;

    FILE *file_descriptor;
	file_descriptor = fopen(nomeArquivo, "r");
	if(file_descriptor == NULL){
        printf("Problemas na abertura do arquivo ou arquivo inexistente!\n");
        exit(1);
	}
    fscanf(file_descriptor, "%d", quadros);
	while((fscanf(file_descriptor, "%d", &pagina)) != EOF){
        auxiliar->vetor[i].valor = pagina;
        auxiliar->vetor[i].flag = 0;
        i++;
	}

	fclose(file_descriptor);
}

//--- fun��o de implementa��o do algoritmo de substitui��o de p�ginas FIFO (First In First Out)
int FIFO(TmoduloPagina auxiliar, int quadros){
    TmoduloVetor *vetorQuadros;
    vetorQuadros = malloc(quadros *sizeof(TmoduloVetor));

    int i = 0, j = 0, k = 0, numeroFaltas = 0, flag = 0, cont = 0, n = 0;

    for(i = 0; i < auxiliar.numeroPaginas; i++){
        if(cont < quadros){
            for(k = 0; k < n; k++){
                if(auxiliar.vetor[i].valor == vetorQuadros[k].valor){
                    flag = 1;
                    break;
                }
            }
            if(flag == 1){
                flag = 0;
            }
            else{
                vetorQuadros[cont].valor = auxiliar.vetor[i].valor;
                numeroFaltas++;
                cont++;
                n++;
            }
        }
        else{
            for(k = 0; k < quadros; k++){
                if(auxiliar.vetor[i].valor == vetorQuadros[k].valor){
                    flag = 1;
                    break;
                }
            }
            if(flag == 1){
                flag = 0;
            }
            else{
                vetorQuadros[j].valor = auxiliar.vetor[i].valor;
                numeroFaltas++;
                j++;
                if(j == quadros){
                    j = 0;
                }
            }
        }
    }
    free(vetorQuadros);

    return numeroFaltas;
}

//--- fun��o auxiliar para contar qual p�gina do vetor de quadros est� mais distante, ou seja, a que ficar� mais tempo sem ser usada
void procuraOTM(TmoduloPagina auxiliar, TmoduloVetor *vetor, int quadros, int x){
    int i = 0, j = 0;
    for(i = 0; i < quadros; i++){
        for(j = x; j < auxiliar.numeroPaginas; j++){
            if(vetor[i].valor == auxiliar.vetor[j].valor){
                vetor[i].distancia = j+1;
                break;
            }
        }
    }

}

//--- fun��o de implementa��o do algoritmo de substitui��o de p�ginas OTM (�timo)
int OTM(TmoduloPagina auxiliar, int quadros){
    TmoduloVetor *vetorQuadro;
    vetorQuadro = malloc(quadros *sizeof(TmoduloVetor));

    int i = 0, j = 0, k = 0, numeroFaltas = 0, cont = 0, flag = 0, n = 0;

    for(i = 0; i < auxiliar.numeroPaginas; i++){
        if(cont < quadros){
            for(k = 0; k < n; k++){
                if(auxiliar.vetor[i].valor == vetorQuadro[k].valor){
                    flag = 1;
                    break;
                }
            }
            if(flag == 1){
                flag = 0;
            }
            else{
                vetorQuadro[cont].valor = auxiliar.vetor[i].valor;
                numeroFaltas++;
                cont++;
                n++;
            }
        }
        else{
            for(k = 0; k < quadros; k++){
                if(auxiliar.vetor[i].valor == vetorQuadro[k].valor){
                    flag = 1;
                    break;
                }
            }
            if(flag == 1){
                flag = 0;
            }
            else{
                for(k = 0; k < quadros; k++){
                    vetorQuadro[k].distancia = 0;
                }
                procuraOTM(auxiliar, vetorQuadro, quadros, i);

                int d = 0;
                for(k = 0; k < quadros; k++){
                    if(vetorQuadro[k].distancia == 0){
                        j = k;
                        break;
                    }
                    else if(d < vetorQuadro[k].distancia){
                        d = vetorQuadro[k].distancia;
                        j = k;
                    }
                }
                vetorQuadro[j].valor = auxiliar.vetor[i].valor;
                numeroFaltas++;
            }
        }
    }
    free(vetorQuadro);
    return numeroFaltas;
}

//--- fun��o de implementa��o do algoritmo de substitui��o de p�ginas LRU (Least Recently Used)
int LRU(TmoduloPagina auxiliar, int quadros){
    TmoduloVetor *vetorQuadros;
    vetorQuadros = malloc(quadros *sizeof(TmoduloVetor));

    int i = 0, j = 0, k = 0, numeroFaltas = 0, cont = 0, flag = 0, n = 0;
    for(k = 0; k < quadros; k++){
        vetorQuadros[k].distancia = 0;
    }
    for(i = 0; i < auxiliar.numeroPaginas; i++){
        if(cont < quadros){
            for(k = 0; k < n; k++){
                if(auxiliar.vetor[i].valor == vetorQuadros[k].valor){
                    flag = 1;
                    vetorQuadros[k].distancia = 0;
                }
                else{
                    vetorQuadros[k].distancia++;
                }
            }
            if(flag == 1){
                flag = 0;
            }
            else{
                vetorQuadros[cont].valor = auxiliar.vetor[i].valor;
                numeroFaltas++;
                cont++;
                n++;
            }
        }
        else{
            for(k = 0; k < quadros; k++){
                if(auxiliar.vetor[i].valor == vetorQuadros[k].valor){
                    flag = 1;
                    vetorQuadros[k].distancia = 0;
                }
                else{
                    vetorQuadros[k].distancia++;
                }
            }
            if(flag == 1){
                flag = 0;
            }
            else{
                int d = 0;
                for(k = 0; k < quadros; k++){
                    if(vetorQuadros[k].distancia == 0){
                        j = k;
                        break;
                    }
                    else if(d < vetorQuadros[k].distancia){
                        d = vetorQuadros[k].distancia;
                        j = k;
                    }
                }
                vetorQuadros[j].distancia = 0;
                vetorQuadros[j].valor = auxiliar.vetor[i].valor;
                numeroFaltas++;
            }
        }
    }
    free(vetorQuadros);
    return numeroFaltas;
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    TmoduloPagina auxiliar;
    int numeroQuadros = 0, numeroFalta = 0, numeroLinha = 0;
    char nome[50];

    printf("> Digite o nome do arquivo com extensao (Ex.: arquivo.txt)\n> ");
    scanf("%s", nome);
    fflush(stdin);

    contarPaginas(nome, &numeroLinha);
    lerArquivo(nome, &auxiliar, &numeroQuadros, numeroLinha);

    numeroFalta = FIFO(auxiliar, numeroQuadros);
    printf("\nFIFO %d\n", numeroFalta);
    fflush(stdin);

    numeroFalta = OTM(auxiliar, numeroQuadros);
    printf("OTM %d\n", numeroFalta);
    fflush(stdin);

    numeroFalta = LRU(auxiliar, numeroQuadros);
    printf("LRU %d\n", numeroFalta);
    fflush(stdin);
    
    return 0;
}
