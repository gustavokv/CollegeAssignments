#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void rootInterval(){
    unsigned short int grau, j, rootConfirm=0;
    float initInterval[2], fA=0, fB=0;

    system("clear");
    printf("Digite o grau do polinômio.\n--> ");
    scanf("%hu", &grau);

    float coeficiente[grau];

    j=grau;
    printf("\nDigite os coeficientes.\n");
    for(int i=0;i<=grau;i++){
        printf("\nx^%d = ", j--);
        scanf("%f", &coeficiente[i]);
    }

    printf("\nDigite o intervalo inicial.\n[a] = ");
    scanf("%f", &initInterval[0]);
    printf("\n[b] = ");
    scanf("%f", &initInterval[1]);

    system("clear");

    //Testa de 0.1 em 0.1 entre o intervalo dado.
    for(float testInterval = initInterval[0];testInterval<=initInterval[1];testInterval += 0.1) {
        j = grau;
        for(int i=0;i<=grau;i++){ 
            fA += coeficiente[i] * pow(testInterval, j);
            fB += coeficiente[i] * pow(testInterval+0.1, j--);
        }

        //A diferença do intervalo encontrado seja menor ou igual a um para evitar mais de uma impressão.
        if(fA * fB < 0 && testInterval+0.1 <= initInterval[1] && abs(ceil(testInterval) - floor(testInterval)) <= 1){   
            printf("Raiz encontrada entre [%.0f,%.0lf].\n", floor(testInterval), ceil(testInterval));
            rootConfirm++;
        } 

        fA = 0;
        fB = 0;
    }

    if(!rootConfirm)
        printf("Não foi encontrado raízes neste intervalo!\n");
}

void bissection() {
    unsigned short int grau, j, n=0, neg=1;
    float error, interval[2], fA=0, fB=0, Xn = 0, XnAnt, precision = 10000;

    system("clear");

    printf("Digite o grau do polinômio.\n--> ");
    scanf("%hu", &grau);

    float coeficiente[grau];

    j = grau;
    printf("\nDigite os coeficientes.\n");
    for(int i=0;i<=grau;i++) {
        printf("\nx^%d = ", j--);
        scanf("%f", &coeficiente[i]);
    }

    printf("\nDigite o intervalo que contém a raiz.\n[A] = ");
    scanf("%f", &interval[0]);
    printf("[B] = ");
    scanf("%f", &interval[1]);

    printf("\nDigite o erro.\n--> ");
    scanf("%f", &error);

    j = grau;
    for(int i=0;i<=grau;i++){ 
        fA += coeficiente[i] * pow(interval[0], j);
        fB += coeficiente[i] * pow(interval[1], j--);
    }

    if(fA * fB >= 0){
        printf("O intervalo não possui raízes reais!\n");
        return;
    }

    //Define qual é o valor do intervalo positivo e o negativo.
    if(fA<0)
        neg = 0;

    system("clear");    

    while(precision > error) {
        printf("Iteração número %hu.\n", n);

        fA = 0;
        XnAnt = Xn;
        Xn = (interval[0] + interval[1]) / 2;

        printf("a = %f.\nb = %f.\n", interval[0], interval[1]);

        printf("Xn: %f.\n", Xn);

        j = grau;
        for(int i=0;i<=grau;i++)
            fA += coeficiente[i] * pow(Xn, j--);

        if(fA>0){
            if(neg == 0)
                interval[1] = Xn;
            else
                interval[0] = Xn;
        }
        else{
            if(neg == 0)
                interval[0] = Xn;
            else
                interval[1] = Xn;
        }


        printf("f(Xn): %f.\n", fA);

        if(n>0){
            precision = fabs(Xn - XnAnt);
            printf("Precisão: %lf.\n\n", precision);
        }
        else
            printf("Precisão: -.\n\n");

        n++;
    }

    printf("Solução é aproximadamente: %f.\nErro Final: %lf.\n\n", Xn, precision);
}

int main() {
    short int mainChoose;

    printf("---ZERO DE FUNÇÕES---\n\
1- Calcular o Intervalo\n\
2- Método da Bissecção\n--> ");
    scanf("%hu", &mainChoose);

    if(mainChoose == 1){
        rootInterval();
    }
    else{
        clock_t begin = clock();
        bissection();
        clock_t end = clock();

        printf("\nTempo de execução: %lf\n", (double)(end-begin) / CLOCKS_PER_SEC);
    }

    return 0;
}
