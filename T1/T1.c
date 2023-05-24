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
    printf("\n--> Solution from the interation %u:\n", iteration);
    for(int i=0;i<ordem;i++) {
        printf("x%u = %.7lf\n", i+1, s[i]);
    }

    printf("--> With the stop test in: %.7lf\n", stop);
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

    printf("Whats the matrix order? [1-10]\n-->");
    scanf("%u", &ordem);

    if(ordem<0)
        ordem = 0;
    else if(ordem>10)
        ordem = 10;

    float matriz[ordem][ordem], matrizInv[ordem][ordem];

    printf("\nFill the matrix:\n\n");

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

    for(manipLinha=0;manipLinha<ordem;manipLinha++) { //Make the main diagonal from the initial matrix be 1, transforming in identity.
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

    printf("\n--> Reverse matrix:\n");
    printMatrix(ordem, matrizInv);
}

//If in the convergence is needed to change rows.
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

//Verify the convergence from the system, if it doesn't, alerts the user and if is needed to change the rows it changes.
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
                printf("--> The comvergence is not guaranteed!\n");
                system("read -p \"\nPressione enter para sair.\" saindo");
                exit(1);
            }
        }
    }
}

void jacobiMethod() {
    unsigned ordem, i, j, iterationLvl=1;
    double precision, stopTest = 1000, bigger; //bigger helps to calculate the biggest value in module from the solution to calculate the stop test.

    printf("--> Insert the matrix order [1-10]:\n");
    scanf("%u", &ordem);

    if (ordem < 0)
        ordem = 1;
    if (ordem > 10)
        ordem = 10;

    double system[ordem][ordem + 1], initSolution[ordem], solution[ordem];

    printf("\n--> Fill the linear system:\n");
    for (i = 0; i < ordem; i++) {
        for (j = 0; j < ordem; j++) {
            printf("i=%u j=%u\n-->", i + 1, j + 1);
            scanf("%lf", &system[i][j]);
        }
        printf("i=%u j=%u\n-->", i + 1, j + 1);
        scanf("%lf", &system[i][j]);
    }

    verifyConvergence(ordem, system);

    printf("\n--> Whats the precision?\n-->");
    scanf("%lf", &precision);

    printf("\n--> And the initial solution?\n");
    for (i = 0; i < ordem; i++) {
        printf("x%u = ", i + 1);
        scanf("%lf", &initSolution[i]);
    }

    while(stopTest > precision){
        bigger = 0;

        printf("\n--> Iteration level: %d.", iterationLvl);
        for(i=0;i<ordem;i++) {
            solution[i] = system[i][ordem]; //The solution begins with the constant after =.

            for(j=0;j<ordem;j++){
                if(i!=j)
                    solution[i] += -system[i][j] * initSolution[j]; //"Pass to the other side" the values.
            }
            solution[i] /= system[i][i]; //Devides by 1/a(ii);
        }

        for(i=0;i<ordem;i++){ //Verify whats the biggest number in module to calculates the error.
            if(fabs(solution[i])>=fabs(bigger)) {
                bigger = solution[i];
                j = i;
            }
        }

        stopTest = fabs((bigger - initSolution[j])/bigger);

        printJacobiMethod(ordem, solution, stopTest, iterationLvl);

        for(i=0;i<ordem;i++)
            initSolution[i] = solution[i]; //Put the initial solution in the previous solution to calculate the error.

        iterationLvl++;
    }

    printf("\n--> Final solution:\n");
    for(i=0;i<ordem;i++) {
        printf("%.7lf\n", solution[i]);
    }
}

int main() {
    unsigned mainChoose;

    do{
        printf("---Linear Systems---\n"
               "1) Matrix inversion by Jordan's Method\n"
               "2) Linear System resolutions by Jacobi's Method\n--> ");
        scanf("%u", &mainChoose);

        if(mainChoose!=1 && mainChoose!=2)
            printf("\nThe chooses are 1 and 2!\n");
        else
            break;

    } while(1);

    clock_t begin = clock();

    if(mainChoose==1)
        jordanMethod();
    else
        jacobiMethod();

    clock_t end = clock();

    printf("\nExecution time: %lf\n", (double)(end-begin) / CLOCKS_PER_SEC);

    return 0;
}
