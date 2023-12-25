#include <stdio.h>
#include <pthread.h> /* _beginthread, _endthread */
#include <Windows.h>

const int AB = 0;
const int AC = 1;
const int BA = 2;
const int BC = 3;
const int CA = 4;
const int CB = 5;

const int error = -5;
const int A = 1;
const int B = 50;
const int C = 100;
const int wait = 3000;

int canal[6] = {-1, -1, -1, -1, -1, -1};


int fa() {
    return 1;
}

int fb() {
    return 50;
}

int fc() {
    return 100;
}

void send_async(const int *buf, int c) {
    canal[c] = *buf;
}

void receive(int *buf, int c) {
    while (canal[c] == -1);
    *buf = canal[c];
    canal[c] = -1;
}

_Noreturn void *ta() {
    int valor;
    do {
        printf("TA EXECUTANDO FA \n");
        Sleep(wait);
        int y = fa();
        send_async(&y, AB);
        receive(&valor, CA);
        printf("TA RECEBEU: %d \n", valor);
        Sleep(wait);
    } while (valor == C);
    if (valor == error) {
        do {
            printf("TA EXECUTANDO FA \n");
            Sleep(wait);
            int y = fa();
            send_async(&y, AC);
            receive(&valor, CA);
            printf("TA RECEBEU: %d \n", valor);
            Sleep(wait);
        } while (valor == C);
        while (1) {
            printf("TA EXECUTANDO FA \n");
            Sleep(wait);
            printf("TA EXECUTANDO FB \n");
            Sleep(wait);
            printf("TA EXECUTANDO FC \n");
            Sleep(wait);
        }
    } else {
        send_async(&error, AB);
        do {
            printf("TA EXECUTANDO FA \n");
            Sleep(wait);
            printf("TA EXECUTANDO FC \n");
            Sleep(wait);
            int y = fa();
            send_async(&y, AB);
            receive(&valor, BA);
            printf("TA RECEBEU: %d \n", valor);
            Sleep(wait);
        } while (valor == B);
        while (1) {
            printf("TA EXECUTANDO FA \n");
            Sleep(wait);
            printf("TA EXECUTANDO FB \n");
            Sleep(wait);
            printf("TA EXECUTANDO FC \n");
            Sleep(wait);
        }
    }
}

_Noreturn void *tb() {
    int valor;
    receive(&valor, AB);
    while (valor == A) {
        printf("TB EXECUTANDO FB \n");
        Sleep(wait);
        int y = fb();
        send_async(&y, BC);
        receive(&valor, AB);
        printf("TB RECEBEU: %d \n", valor);
        Sleep(wait);
    }
    if (valor == error) {
        do {
            printf("TB EXECUTANDO FB \n");
            Sleep(wait);
            int y = fb();
            send_async(&y, BA);
            receive(&valor, AB);
            printf("TB RECEBEU: %d \n", valor);
            Sleep(wait);
        } while (valor == A);
        while (1) {
            printf("TB EXECUTANDO FA \n");
            Sleep(wait);
            printf("TB EXECUTANDO FB \n");
            Sleep(wait);
            printf("TB EXECUTANDO FC \n");
            Sleep(wait);
        }
    } else {
        send_async(&error, BC);
        do {
            printf("TB EXECUTANDO FA \n");
            Sleep(wait);
            printf("TB EXECUTANDO FB \n");
            Sleep(wait);
            int y = fb();
            send_async(&y, BC);
            receive(&valor, CB);
            printf("TB RECEBEU: %d \n", valor);
            Sleep(wait);
        } while (valor == C);
        while (1) {
            printf("TB EXECUTANDO FA \n");
            Sleep(wait);
            printf("TB EXECUTANDO FB \n");
            Sleep(wait);
            printf("TB EXECUTANDO FC \n");
            Sleep(wait);
        }
    }
}

_Noreturn void *tc() {
    int valor;
    receive(&valor, BC);
    while (valor == B) {
        printf("TC EXECUTANDO FC \n");
        Sleep(wait);
        int y = fc();
        send_async(&y, CA);
        receive(&valor, BC);
        printf("TC RECEBEU: %d \n", valor);
        Sleep(wait);
    }
    if (valor == error) {
        do {
            printf("TC EXECUTANDO FC \n");
            Sleep(wait);
            int y = fc();
            send_async(&y, CB);
            receive(&valor, BC);
            printf("TC RECEBEU: %d \n", valor);
            Sleep(wait);
        } while (valor == B);
        while (1) {
            printf("TC EXECUTANDO FA \n");
            Sleep(wait);
            printf("TC EXECUTANDO FB \n");
            Sleep(wait);
            printf("TC EXECUTANDO FC \n");
            Sleep(wait);
        }
    } else {
        send_async(&error, CA);
        do {
            printf("TC EXECUTANDO FB \n");
            Sleep(wait);
            printf("TC EXECUTANDO FC \n");
            Sleep(wait);
            int y = fc();
            send_async(&y, CA);
            receive(&valor, AC);
            printf("TC RECEBEU: %d \n", valor);
            Sleep(wait);
        } while (valor == A);
        while (1) {
            printf("TC EXECUTANDO FA \n");
            Sleep(wait);
            printf("TC EXECUTANDO FB \n");
            Sleep(wait);
            printf("TC EXECUTANDO FC \n");
            Sleep(wait);
        }
    }
}

int main() {
    pthread_t TA;
    pthread_t TB;
    pthread_t TC;

    pthread_create(&TA, NULL, ta, NULL);
    pthread_create(&TB, NULL, tb, NULL);
    pthread_create(&TC, NULL, tc, NULL);

    pthread_join(TA, NULL);
    pthread_join(TC, NULL);
    pthread_join(TB, NULL);

    return 0;
}