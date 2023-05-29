#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void scan(char *,int, int *);
void split (char *, char *, char, int *);
int pertence (char, char *);
char delta(char, char, char [], char [], char [][10]);
int verifyF(char *, char *, int, int);

void scan(char *str, int t, int *tamW){
    fgets(str,t,stdin);  //coloca \n e \0 no fim
    *tamW = strlen(str) - 1;
    str[*tamW] = '\0';
}

void split (char *Q, char *str, char sep, int *tam){
    int j=0;
    for(int i=0;i<*tam;i++) {
        if (str[i] != sep)
            Q[j++] = str[i];
    }

    *tam = j;
}

int pertence (char c, char *Str){
    int i;

    for(i=0;Str[i] != '\0';i++) {
        if (c == Str[i])
            return 1;
    }

    return -1;  //Retorna uma falha caso não esteja nos estados de F.
}

char delta(char q, char c, char Q[], char A[], char mDelta[][10]){
    int i=0, j=0;

    //Ambos while guardam o i e j da posição equivalente nos arrays de estados e alfabeto, onde está localizado no
    //mesmo i e j da matriz do delta, retornando seu equivalente.
    //EX:   a b
    //  A | B C
    //  B | A D
    //  C | D A
    //  D | C B

    while(Q[i] != '\0'){
        if(Q[i] == q)
            break;

        i++;
    }
    while(A[j] != '\0'){
        if(A[j] == c)
            break;
        j++;
    }

    return mDelta[i][j];
}

int verifyF(char *F, char *Q, int tamF, int tamQ) {
    int confirm = 0;

    for(int i=0;i<tamF;i++){
        for(int j=0;j<tamQ;j++) {
            if(Q[j] == F[i])
                confirm++;
        }
    }

    if(confirm == tamF) //Caso tenha a mesma quantidade de elementos que o conjunto F, quer dizer que todos os
        return 1;       //elementos estão em Q.

    return 0;
}

int verifyq(char q, char *Q, int tamQ) {
    if(q == '-') //Caso o estado digitado seja o estado vazio, ele libera, onde o '-' representa o estado vazio.
        return 1;

    for(int i=0;i<tamQ;i++){
        if(Q[i] == q)
            return 1;   //Caso o estado esteja no array de estados.
    }

    return 0;
}

int Travou(char q) {
    if(q == '\0' || q == '-') //Ou ele não encontrou o caractere digitado, ou não há estado de transição para este
        return 1;             //caractere('-').

    return 0;
}

int main() {
    //W = Cadeia Lida, Q = Estados, A = Alfabeto, qo = Estado Incicial, F = Estados de Aceitação, q = Estado Corrente.
    //mDelta = Matriz de Estados/Alfabeto, c = símbolo corrente.
    char W[20], Q[10], A[10], qo, F[10], q, mDelta[10][10], c;
    char str[30], aux;
    int tamQ, tamF, tamA, tamW, i, j, p;
    int Falha = 0, choose, choose2;

    do{
        printf("\nEntre os simbolos do alfabeto, separados por virgulas:\n");
        scan(str, 30, &tamA);
        split(A, str, ',', &tamA);
        printf("\n");

        printf("Entre com o conjunto de Estados, separados por virgulas:\n");
        scan(str, 30, &tamQ);
        split(Q, str, ',', &tamQ); //Separa cada estado

        do {
            printf("\nEntre com o estado inicial:\n");
            scanf(" %c", &qo);
            getchar(); //Recebe o "\n" para limpar o buffer.

            if (verifyq(qo, Q, tamQ)) //Verifica se qo está em Q.
                break;

            printf("Este estado inicial nao esta contido em Q! Digite novamente.\n");
        } while (1);

        do {
            printf("\nEntre os estados de F (aceitacao), separados por virgulas:\n");
            scan(str, 30, &tamF);
            split(F, str, ',', &tamF);
            if (verifyF(F, Q, tamF, tamQ))
                break;

            printf("\nAlgum simbolo digitado para F nao esta em Q! Digite novamente.");

        } while (1);

        //Leitura da matriz Delta
        for (i = 0; i < tamQ; i++) {
            for (j = 0; j < tamA; j++) {
                do {
                    printf("\nDelta(%c, %c) = ", Q[i], A[j]);
                    aux = getc(stdin);
                    getchar();

                    if (!verifyq(aux, Q, tamQ))
                        printf("Este estado nao esta contido no array de estados! Digite novamente.\n");
                    else {
                        mDelta[i][j] = aux;
                        break;
                    }

                } while (1);
            }
        }

        do {
            system("clear");
            printf("\nEntre com a palavra a ser verificada:\n");
            scan(W, 20, &tamW);

            p = 0;
            q = qo;
            c = W[p++];

            printf("\nSequencia de estados:\n");
            printf("%c", q);

            while (c != '\0') {
                q = delta(q, c, Q, A, mDelta);

                if (Travou(q)) {
                    Falha = 1;
                    break;
                }

                c = W[p++]; //c = ProximoChar
                printf(", %c", q);
            }

            if(Falha && q == '-') //Caso ela tenha travado por conta do caractere vazio nos estados de transição.
                printf("\n\nTravou! Nao ha transicao para este caractere/estado.\n\n");
            else if(pertence(q, F) == -1 || Falha)
                printf("\n\nPalavra NAO reconhecida!\n\n");
            else
                printf("\n\nPalavra reconhecida!\n\n");

            printf("Deseja verificar outra palavra? 1 - Sim / 2 - Nao\n");
            scanf("%d", &choose);

            bzero(W, 20);
            Falha = 0;
            getchar();
        } while (choose == 1);

        printf("Deseja criar outro automato? 1 - Sim / 2 - Nao\n");
        scanf("%d", &choose2);
        system("clear");
        getchar();
    }while(choose2 == 1);

    return 0;
}
