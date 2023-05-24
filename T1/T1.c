#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void printMatrix(unsigned ordem, float m[][ordem]);
void printExpMatrix(unsigned ordem, float m[][ordem], float mi[][ordem]);
void printJacobiMethod(unsigned ordem, double s[ordem], double stop, unsigned iteration);
void invertLineExp(unsigned ordem, float m[][ordem], float inv[][ordem]);
void jordanMethod();
void verifyConvergence(unsigned ordem, double sys[][ordem+1]);
void jacobiMethod();

void printMatrix(unsigned ordem, float m[][ordem]){
    for(int i=0;i<ordem;i++){
        for(int j=0;j<ordem;j++){
            printf("%.1f ", m[i][j]);
        }
        printf("\n");
    }
}
void printExpMatrix(unsigned ordem, float m[][ordem], float mi[][ordem]){ //Print the expanded matrix.
    for(int i=0;i<ordem;i++){
        for(int j=0;j<ordem;j++){
            printf("%.1f ", m[i][j]);
        }
        printf("  ");
        for(int j=0;j<ordem;j++){
            printf("%.1f ", mi[i][j]);
        }
        printf("\n");
    }
}

void printJacobiMethod(unsigned ordem, double s[ordem], double stop, unsigned iteration) { //Print the solutions.
    printf("\n--> Solução da iteração %u:\n", iteration);
    for(int i=0;i<ordem;i++) {
        printf("x%u = %.7lf\n", i+1, s[i]);
    }

    printf("--> Com o teste de parada em: %.7lf\n", stop);
}

void invertLineExp(unsigned ordem, float m[][ordem], float inv[][ordem]){
    float aux[ordem], auxInv[ordem];

    for(unsigned i=0;i<ordem;i++){
        if(m[i][i] == 0){ //If the pivot is 0.
            for(unsigned j=i+1;j<ordem;j++){ //The j=i+1 is to begin a row below the row with pivot 0.
                if(m[j][i] != 0){ //In the case that row doesn't have a pivot 0, the row will be changed with it.
                    for(unsigned k=0;k<ordem;k++){
                        aux[k] = m[j][k];
                        auxInv[k] = inv[j][k];
                    }
                    for(unsigned k=0;k<ordem;k++) {
                        m[j][k] = m[i][k];
                        inv[j][k] = inv[i][k];
                    }
                    for(unsigned k=0;k<ordem;k++) {
                        m[i][k] = aux[k];
                        inv[i][k] = auxInv[k];
                    }

                    printf("\n--> L%u = L%u\n", i+1, j+1);
                    printf("--> L%u = L%u\n", j+1, i+1);
                    printExpMatrix(ordem, m, inv);

                    return;
                }
            }
        }
    }
}

void jordanMethod() {
    unsigned ordem, i, j;
    int manipLinha;
    float Mij;

    printf("Qual a ordem da matriz? [1-10]\n-->");
    scanf("%u", &ordem);

    if(ordem<0)
        ordem = 0;
    else if(ordem>10)
        ordem = 10;

    float matriz[ordem][ordem], matrizInv[ordem][ordem];

    printf("\nPreencha a matriz:\n\n");

    for(i=0;i<ordem;i++) {
        for(j=0;j<ordem;j++){ //Fill the matrix and creates the indentity matrix.
            printf("i=%u j=%u\n-->", i + 1, j + 1);
            scanf("%f", &matriz[i][j]);

            if(i==j)
                matrizInv[i][j] = 1;
            else
                matrizInv[i][j] = 0;
        }
    }

    for(i=0;i<ordem;i++) {
        if(matriz[i][i]==0) //If the pivot is 0, change the rows.
            invertLineExp(ordem, matriz, matrizInv);

        for(manipLinha=i+1;manipLinha<ordem;manipLinha++) { //Nullify below the main diagonal.
            if (i != ordem - 1) { //ordem-1 is to not nullify below the last element from the main diagonal.
                Mij = -(matriz[manipLinha][i] / matriz[i][i]);

                printf("\n--> L%d = %.1f*L%d + L%d", manipLinha + 1, Mij, i + 1, manipLinha + 1);

                j = 0;
                while (j < ordem) {
                    matriz[manipLinha][j] = Mij * matriz[i][j] + matriz[manipLinha][j];
                    matrizInv[manipLinha][j] = Mij * matrizInv[i][j] + matrizInv[manipLinha][j];
                    j++;
                }
            }
        }
        for(manipLinha=i-1;manipLinha>=0;manipLinha--){ //Nullify the column above the main diagonal.
            Mij = -(matriz[manipLinha][i] / matriz[i][i]);

            printf("\n--> L%d = %.1f*L%u + L%d", manipLinha+1, Mij, i+1, manipLinha+1);

            j=0;
            while(j<ordem) {
                matriz[manipLinha][j] = Mij * matriz[i][j] + matriz[manipLinha][j];
                matrizInv[manipLinha][j] = Mij * matrizInv[i][j] + matrizInv[manipLinha][j];

                j++;
            }
        }
        printf("\n");
        printExpMatrix(ordem, matriz, matrizInv);
    }

    for(manipLinha=0;manipLinha<ordem;manipLinha++) { //Faz a diagonal principal da matriz inicial ser 1, tranformando em identidade.
        printf("\n--> L%d = 1/%.1f * L%d", manipLinha+1, matriz[manipLinha][manipLinha], manipLinha+1);

        j=0;
        Mij = matriz[manipLinha][manipLinha];

        while(j<ordem) {
            if(matriz[manipLinha][j] != 0)
                matriz[manipLinha][j] =  matriz[manipLinha][j] / Mij;
            if(matrizInv[manipLinha][j] != 0)
                matrizInv[manipLinha][j] = matrizInv[manipLinha][j] / Mij;

            j++;
        }
    }
    printf("\n");
    printExpMatrix(ordem, matriz, matrizInv);

    printf("\n--> Matriz inversa:\n");
    printMatrix(ordem, matrizInv);
}

//Caso na convergência precise trocar as linhas.
void invertLineSystem(unsigned ordem, double sys[][ordem+1], unsigned orig, unsigned change) {
    double aux[ordem+1];

    for(unsigned i=0;i<ordem+1;i++)
        aux[i] = sys[orig][i];

    for(unsigned i=0;i<ordem+1;i++) {
        sys[orig][i] = sys[change][i];
    }
    for(unsigned i=0;i<ordem+1;i++) {
        sys[change][i] = aux[i];
    }
}

//Verifica a convergência do sistema, caso não possua, alerta ao usuário e caso precise trocar as linhas ele troca.
void verifyConvergence(unsigned ordem, double sys[][ordem+1]) {
    unsigned i, j, convConfirm;
    double convTest, convTestAux;

    for(i=0;i<ordem;i++) {
        convTest = 0;
        convConfirm = 0;

        for(j=0;j<ordem;j++) {
            if(j!=i)
                convTest += fabs(sys[i][j]);
        }
        convTest /= fabs(sys[i][i]);

        if(convTest>1) {
            convTestAux = 0;

            for(unsigned k=i+1;k<ordem;k++){
                for(j=0;j<ordem;j++) {
                    if(i!=j)
                        convTestAux += fabs(sys[k][j]);
                }
                convTestAux /= fabs(sys[k][i]);

                if(convTestAux < 1){
                    invertLineSystem(ordem, sys, i, k);
                    convConfirm++;
                    break;
                }
            }
            if(!convConfirm) {
                printf("--> A convergência não é garantida!\n");
                system("read -p \"\nPressione enter para sair.\" saindo");
                exit(1);
            }
        }
    }
}

void jacobiMethod() {
    unsigned ordem, i, j, iterationLvl=1;
    double precision, stopTest = 1000, bigger; //bigger serve para ajudar a calcular o maior valor em módulo da solução para calcular o teste de parada.

    printf("--> Digite a ordem da matriz [1-10]:\n");
    scanf("%u", &ordem);

    if (ordem < 0)
        ordem = 1;
    if (ordem > 10)
        ordem = 10;

    double system[ordem][ordem + 1], initSolution[ordem], solution[ordem];

    printf("\n--> Preencha o sistema linear:\n");
    for (i = 0; i < ordem; i++) {
        for (j = 0; j < ordem; j++) {
            printf("i=%u j=%u\n-->", i + 1, j + 1);
            scanf("%lf", &system[i][j]);
        }
        printf("i=%u j=%u\n-->", i + 1, j + 1);
        scanf("%lf", &system[i][j]);
    }

    verifyConvergence(ordem, system);

    printf("\n--> Qual a precisão?\n-->");
    scanf("%lf", &precision);

    printf("\n--> E a solução inicial?\n");
    for (i = 0; i < ordem; i++) {
        printf("x%u = ", i + 1);
        scanf("%lf", &initSolution[i]);
    }

    while(stopTest > precision){
        bigger = 0;

        printf("\n--> Nível da interação: %d.", iterationLvl);
        for(i=0;i<ordem;i++) {
            solution[i] = system[i][ordem]; //A solução inicia com a constante após o =.

            for(j=0;j<ordem;j++){
                if(i!=j)
                    solution[i] += -system[i][j] * initSolution[j]; //"Passa para o outro lado" os valores.
            }
            solution[i] /= system[i][i]; //Divide pelo 1/a(ii);
        }

        for(i=0;i<ordem;i++){ //Verifica qual o maior número em módulo para calcular o erro.
            if(fabs(solution[i])>=fabs(bigger)) {
                bigger = solution[i];
                j = i;
            }
        }

        stopTest = fabs((bigger - initSolution[j])/bigger);

        printJacobiMethod(ordem, solution, stopTest, iterationLvl);

        for(i=0;i<ordem;i++)
            initSolution[i] = solution[i]; //Coloca solução atual na solução anterior para calcular o erro.

        iterationLvl++;
    }

    printf("\n--> Solução final:\n");
    for(i=0;i<ordem;i++) {
        printf("%.7lf\n", solution[i]);
    }
}

int main() {
    unsigned mainChoose;

    do{
        printf("---Sistemas Lineares---\n"
               "1) Inversao de Matrizes pelo Metodo de Jordan\n"
               "2) Resolucao de Sistemas Lineares Pelo Metodo de Jacobi\n--> ");
        scanf("%u", &mainChoose);

        if(mainChoose!=1 && mainChoose!=2)
            printf("\nAs escolhas sao 1 e 2!\n");
        else
            break;

    } while(1);

    clock_t begin = clock();

    if(mainChoose==1)
        jordanMethod();
    else
        jacobiMethod();

    clock_t end = clock();

    printf("\nTempo de execução: %lf\n", (double)(end-begin) / CLOCKS_PER_SEC);

    return 0;
}
