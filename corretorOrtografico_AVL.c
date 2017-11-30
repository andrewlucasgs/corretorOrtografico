/*********************************************************************************************
* EDA 2017/2 - ESTRUTURAS DE DADOS E ALGORITMOS (Prof. Fernando W. Cruz)
* Projeto  - Arvores e funcoes de hash
* Verifica corretude de palavras de um arquivo-texto segundo um dicionario carregado em RAM.
 *********************************************************************************************/
 /*                             Andrew Lucas Guedes de Souza*/
 /*                                     Matrícula: 160023921*/
 /*---------------------------------------------------------*/
 /*                              Nivaldo Pereira Lopo Junior*/
 /*                                     Matrícula: 120039460*/
 /*---------------------------------------------------------*/
 /*                        Marcelo Ferreira Magalhães Júnior*/
 /*                                     Matrícula: 150138946*/
 /***********************************************************/
#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdbool.h>

/*Bibliotecas incluidas*/
#include <string.h>
#include <stdlib.h>

/* Tamanho maximo de uma palavra do dicionario */
#define TAM_MAX 45
/* dicionario default */
#define NOME_DICIONARIO "dicioPadrao"

/* retornos desse programa */
#define SUCESSO                 0
#define NUM_ARGS_INCORRETO      1
#define ARQDICIO_NAOCARREGADO   2
#define ARQTEXTO_ERROABERTURA   3
#define ARQTEXTO_ERROLEITURA    4
#define ERRO_DICIO_NAOCARREGADO 5

#define MAX(a,b) ((a>b)? a : b )
/* Definindo estrutura de elemento da arvore*/
typedef struct dicio{
    char palavra[TAM_MAX];
    int h;
    struct dicio *esq;
    struct dicio *dir;
}Dicio;

/* Variaveis globais*/
Dicio *dict = NULL;
int cont = 0;

/*Calcula altura da arvore*/

void altura(Dicio *r){
	int a = ((r->dir!=NULL) ? r->dir->h+1 : 0);
	int b = ((r->esq!=NULL) ? r->esq->h+1: 0 );
	r->h= MAX(a,b);
}

/*Rotaciona para arvore para esquerda*/
Dicio *rotEsq (Dicio *r) {
  Dicio *b = r->dir;
  Dicio *tmp = b->esq;
  r->dir = tmp;
  b->esq = r;
  altura(r);
  altura(b);
  return b;
}

/*Rotaciona para arvore para direita*/
Dicio *rotDir(Dicio *r) {
  Dicio *b = r->esq;
  Dicio *tmp = b->dir;
  b->dir = r;
  r->esq = tmp;
  altura(r);
  altura(b);
  return b;
}



/* Nova palavra do dicionario */
Dicio *novo(char *palavra){
	Dicio *tmp = (Dicio*)malloc(sizeof(Dicio));
	tmp->h=0;
  strcpy(tmp->palavra, palavra);
	tmp->esq = tmp->dir = NULL;
	return tmp;
}

void  imprimeDicionario(Dicio *pAtual){
	if(pAtual == NULL){
    return;
	}
	else{
		imprimeDicionario(pAtual->esq);
		printf("%p: %s, esq: %p, dir: %p\n", pAtual, pAtual->palavra, pAtual->esq, pAtual->dir);
		imprimeDicionario(pAtual->dir);
	}
}

/*Insere nova palavra na arvore*/
Dicio *inserir(Dicio *r, Dicio *elemento){
	if(r==NULL)
    return elemento;
	else {
		if(strcmp(elemento->palavra, r->palavra) > 0){
      r->dir = inserir(r->dir, elemento);
    }
		else{
      r->esq = inserir(r->esq, elemento);
    }
		altura(r);
		int v = ((r->dir==NULL) ? -1 : r->dir->h) - ((r->esq==NULL)?-1 : r->esq->h);
		if(v>1 && strcmp(elemento->palavra, r->dir->palavra) > 0){
      return rotEsq(r);
    }
		if(v<-1 && strcmp(elemento->palavra, r->esq->palavra) < 0){
      return rotDir(r);
    }
		if(v>1 && strcmp(elemento->palavra, r->dir->palavra) < 0){
			r->dir=rotDir(r->dir);
			return rotEsq(r);
		}
		if(v<-1 && strcmp(elemento->palavra, r->esq->palavra) > 0){
			r->esq=rotEsq(r->esq);
			return rotDir(r);
		}
	}
	return r;
}

Dicio *busca (Dicio *r, const char *palavra) {
    if (r == NULL || strcmp(r->palavra, palavra) == 0){
      return r;
    }
    if (strcmp(r->palavra, palavra) > 0)
       return busca (r->esq, palavra);
    else
       return busca (r->dir, palavra);
}

/* Retorna true se a palavra estah no dicionario. Do contrario, retorna false */
bool conferePalavra(const char *palavra) {
  char *p = (char *)palavra;
  for ( ; *p; ++p) *p = tolower(*p);
    /* construa essa funcao */
    if(busca(dict, palavra) == NULL)
      return false;
    else{
      return true;
    }
} /* fim-conferePalavra */

/* Carrega dicionario na memoria. Retorna true se sucesso; senao retorna false. */
bool carregaDicionario(const char *dicionario) {
    /* construa essa funcao */
  char novaPalavra[TAM_MAX];
  FILE *fp = fopen(dicionario, "r");
  if(fp != NULL){
    while(!feof(fp)){
      fscanf(fp, "%s\n", novaPalavra);
      cont++;
      dict = inserir(dict, novo(novaPalavra));
    }
    fclose(fp);
    return true;
  }else{
    fclose(fp);
    return false;
  }
} /* fim-carregaDicionario */


/* Retorna qtde palavras do dicionario, se carregado; senao carregado retorna zero */
unsigned int contaPalavrasDic(void) {
    /* construa essa funcao */
    return cont;
} /* fim-contaPalavrasDic */

void limpar(Dicio *r){

	if(r != NULL){
		limpar(r->esq);
		limpar(r->dir);
		free(r);
	}
}

/* Descarrega dicionario da memoria. Retorna true se ok e false se algo deu errado */
bool descarregaDicionario(void) {
    /* construa essa funcao */
    limpar(dict);
    return true;
} /* fim-descarregaDicionario */

/* Retorna o numero de segundos entre a e b */
double calcula_tempo(const struct rusage *b, const struct rusage *a) {
    if (b == NULL || a == NULL)
        return 0;
    else
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
} /* fim-calcula_tempo */


int main(int argc, char *argv[]) {
    struct rusage tempo_inicial, tempo_final; /* structs para dados de tempo do processo */
    double tempo_carga = 0.0, tempo_check = 0.0, tempo_calc_tamanho_dic = 0.0, tempo_limpeza_memoria = 0.0;
    /* determina qual dicionario usar; o default eh usar o arquivo dicioPadrao */
    char *dicionario = (argc == 3) ? argv[1] : NOME_DICIONARIO;
    int  indice, totPalErradas, totPalavras, c;
    char palavra[TAM_MAX+1];
    bool palavraErrada, descarga, carga;
    unsigned int qtdePalavrasDic;
    char *arqTexto;
    FILE *fd;

    /* Confere se o numero de argumentos de chamada estah correto */
    if (argc != 2 && argc != 3) {
        printf("Uso: %s [nomeArquivoDicionario] nomeArquivoTexto\n", argv[0]);
        return NUM_ARGS_INCORRETO;
    } /* fim-if */

    /* carrega o dicionario na memoria, c/ anotacao de tempos inicial e final */
    getrusage(RUSAGE_SELF, &tempo_inicial);
       carga = carregaDicionario(dicionario);
    getrusage(RUSAGE_SELF, &tempo_final);

    /* aborta se o dicionario nao estah carregado */
    if (!carga) {
        printf("Dicionario nao carregado!\n");
        return ARQDICIO_NAOCARREGADO;
    } /* fim-if */

    /* calcula_tempo para carregar o dicionario */
    tempo_carga = calcula_tempo(&tempo_inicial, &tempo_final);

    /* tenta abrir um arquivo-texto para corrigir */
    arqTexto = (argc == 3) ? argv[2] : argv[1];
    fd = fopen(arqTexto, "r");
    if (fd == NULL) {
        printf("Nao foi possivel abrir o arquivo de texto %s.\n", arqTexto);
        descarregaDicionario();
        return ARQTEXTO_ERROABERTURA;
    } /* fim-if */

    /* Reportando palavras erradas de acordo com o dicionario */
    printf("\nPALAVRAS NAO ENCONTRADAS NO DICIONARIO \n\n");

    /* preparando para checar cada uma das palavras do arquivo-texto */
    indice = 0, totPalErradas = 0, totPalavras = 0;

    /* checa cada palavra do arquivo-texto  */
    for (c = fgetc(fd); c != EOF; c = fgetc(fd)) {
        /* permite apenas palavras c/ caracteres alfabeticos e apostrofes */
        if (isalpha(c) || (c == '\'' && indice > 0)) {
            /* recupera um caracter do arquivo e coloca no vetor palavra */
            palavra[indice] = c;
            indice++;
            /* ignora palavras longas demais (acima de 45) */
            if (indice > TAM_MAX) {
                /* nesse caso, consome o restante da palavra e "reseta" o indice */
                while ((c = fgetc(fd)) != EOF && isalpha(c));
                indice = 0;
            } /* fim-if */
        } /* fim-if */
        /* ignora palavras que contenham numeros */
        else if (isdigit(c)) {
            /* nesse caso, consome o restante da palavra e "reseta" o indice */
            while ((c = fgetc(fd)) != EOF && isalnum(c));
            indice = 0;
        } /* fim-if */
        /* encontra uma palavra completa */
        else if (indice > 0) { /* termina a palavra corrente */
            palavra[indice] = '\0';
            totPalavras++;
            /* confere o tempo de busca da palavra */
            getrusage(RUSAGE_SELF, &tempo_inicial);
              palavraErrada = !conferePalavra(palavra);
            getrusage(RUSAGE_SELF, &tempo_final);
            /* atualiza tempo de checagem */
            tempo_check += calcula_tempo(&tempo_inicial, &tempo_final);
            /* imprime palavra se nao encontrada no dicionario */
            if (palavraErrada) {
                printf("%s\n", palavra);
                totPalErradas++;
            } /* fim-if */
            /* faz "reset" no indice para recuperar nova palavra no arquivo-texto*/
            indice = 0;
        } /* fim-if */
    } /* fim-for */

    /* verifica se houve um erro na leitura do arquivo-texto */
    if (ferror(fd)) {
        fclose(fd);
        printf("Erro de leitura %s.\n", arqTexto);
        descarregaDicionario();
        return ARQTEXTO_ERROLEITURA;
    } /* fim-if */

    /* fecha arquivo */
    fclose(fd);

    /* determina a quantidade de palavras presentes no dicionario, c anotacao de tempos */
    getrusage(RUSAGE_SELF, &tempo_inicial);
      qtdePalavrasDic = contaPalavrasDic();
    getrusage(RUSAGE_SELF, &tempo_final);

    /* calcula tempo p determinar o tamanho do dicionario */
    tempo_calc_tamanho_dic = calcula_tempo(&tempo_inicial, &tempo_final);

    /* limpa a memoria - descarrega o dicionario, c anotacao de tempos */
    getrusage(RUSAGE_SELF, &tempo_inicial);
      descarga = descarregaDicionario();
    getrusage(RUSAGE_SELF, &tempo_final);

    /* aborta se o dicionario nao estiver carregado */
    if (!descarga) {
        printf("Nao foi necessario fazer limpeza da memoria\n");
        return ERRO_DICIO_NAOCARREGADO;
    } /* fim-if */

    /* calcula tempo para descarregar o dicionario */
    tempo_limpeza_memoria = calcula_tempo(&tempo_inicial, &tempo_final);

    /* RESULTADOS ENCONTRADOS */
    printf("\nTOTAL DE PALAVRAS ERRADAS NO TEXTO    : %d\n",   totPalErradas);
    printf("TOTAL DE PALAVRAS DO DICIONARIO         : %d\n",   qtdePalavrasDic);
    printf("TOTAL DE PALAVRAS DO TEXTO              : %d\n",   totPalavras);
    printf("TEMPO GASTO COM CARGA DO DICIONARIO     : %.2f\n", tempo_carga);
    printf("TEMPO GASTO COM CHECK DO ARQUIVO        : %.2f\n", tempo_check);
    printf("TEMPO GASTO P CALCULO TAMANHO DICIONARIO: %.2f\n", tempo_calc_tamanho_dic);
    printf("TEMPO GASTO COM LIMPEZA DA MEMORIA      : %.2f\n", tempo_limpeza_memoria);
    printf("------------------------------------------------------\n");
    printf("T E M P O   T O T A L                   : %.2f\n\n",
     tempo_carga + tempo_check + tempo_calc_tamanho_dic + tempo_limpeza_memoria);
    printf("------------------------------------------------------\n");
   return SUCESSO;
} /* fim-main */
