#include <stdio.h>
#include <stdlib.h>

#define VARIABLES 26
#define ASCII 97
#define BASE 100000000
#define SPACE 32

#define I 'I'
#define A 'A'
#define C 'C'
#define J 'J'
#define D 'D'
#define H 'H'

struct machine_code {
    char *tab_name;
    unsigned int **tab_value;
    unsigned int size;
    unsigned int size_all;
};

struct variables {
    int **tab;
    int *tab_size;
    int *tab_size_all;
};

int max(int a, int b) {
    if (a >= b) return a;
    else return b;
}

void write(int tab[], int n) {
    printf("%d", tab[n - 1]);
    for (int i = n - 2; i >= 0; i--)
        printf("%08d", tab[i]);
    printf("\n");
}

void realloc_variable(struct variables *v, unsigned int i) {
    if (v->tab_size[i] > v->tab_size_all[i]) {
        v->tab_size_all[i] *= 2;
        v->tab[i] = realloc(v->tab[i], (unsigned int) v->tab_size_all[i] * sizeof(v->tab));
    }
}

void increment(struct variables *v, unsigned int i) {
    int k = 0;
    while (v->tab[i][k] >= BASE - 1)
        k++;
    if (k < v->tab_size[i])
        v->tab[i][k]++;
    else {
        v->tab_size[i]++;
        realloc_variable(v, i);
        v->tab[i][v->tab_size[i] - 1] = 1;
    }
    for (int j = 0; j < k; j++)
        v->tab[i][j] = 0;
}

void decrement(struct variables *v, unsigned int i) {
    int k = 0;
    while (v->tab[i][k] == 0 && k < v->tab_size[i])
        k++;
    if (k < v->tab_size[i] - 1 || k == 0)
        v->tab[i][k]--;
    else {
        v->tab_size[i]--;
        realloc_variable(v, i);
    }
    for (int j = 0; j < k; j++)
        v->tab[i][j] = BASE - 1;
}

void add(struct variables *v, unsigned int a, unsigned int b) {
    int x = 0;
    for (int i = 0; i < max(v->tab_size[a], v->tab_size[b]); i++) {
        if (i >= v->tab_size[a]) {
            v->tab_size[a]++;
            realloc_variable(v, a);
            v->tab[a][v->tab_size[a] - 1] = 0;
        }
        if (i < v->tab_size[b])
            v->tab[a][i] += v->tab[b][i];
        v->tab[a][i] += x;
        if (v->tab[a][i] >= BASE) {
            v->tab[a][i] -= BASE;
            x = 1;
        } else
            x = 0;
    }
    if (x != 0) {
        v->tab_size[a]++;
        realloc_variable(v, a);
        v->tab[a][v->tab_size[a] - 1] = 1;
    }
}

void clear(struct variables *v, unsigned int i) {
    v->tab_size[i] = 1;
    realloc_variable(v, i);
    v->tab[i][0] = 0;
}

void enter_code(struct machine_code *code, unsigned int i, char n, unsigned int v1, int v2) {
    code->tab_name[i] = n;
    code->tab_value[0][i] = v1;
    code->tab_value[1][i] = v2;
}

void write_code(struct machine_code code) {
    for (int i = 0; i < code.size; i++) {
        int x = (unsigned char) code.tab_name[i];
        if (x == I || x == C || x == A || x == H)
            printf("%c %c %c\n", x, code.tab_value[0][i], code.tab_value[1][i]);
        else if (x == J)
            printf("%c %d %c\n", x, code.tab_value[0][i], code.tab_value[1][i]);
        else if (x == D)
            printf("%c %c %d\n", x, code.tab_value[0][i], code.tab_value[1][i]);
    }
}

void realloc_code(struct machine_code *code) {
    if (code->size >= code->size_all) {
        code->size_all *= 2;
        code->tab_name = realloc(code->tab_name, (unsigned int) code->size_all * sizeof(code->tab_name));
        for (int i = 0; i < 2; i++)
            code->tab_value[i] = realloc(code->tab_value[i],
                                         (unsigned int) code->size_all * sizeof(code->tab_value));
    }
}

void repeat(struct machine_code *code) {
    int x_iterator = getchar();
    int x = getchar();
    int if_last = 0, if_same = 0;
    unsigned int p, s = code->size;
    code->size++;
    realloc_code(code);
    enter_code(code, code->size - 2, D, x_iterator, -1);
    p = code->size - 2;
    while (x != ')') {
        if (x == x_iterator)
            if_same++;
        if (x == '(') {
            repeat(code);
            if_last++;
        } else {
            code->size++;
            realloc_code(code);
            enter_code(code, code->size - 2, I, x, SPACE);
        }
        x = getchar();
    }
    code->size++;
    realloc_code(code);
    enter_code(code, code->size - 2, J, p, SPACE);
    code->tab_value[1][p] = code->size - 1;
    if (if_last == 0 && if_same == 0) {
        code->size--;
        realloc_code(code);
        for (unsigned int i = s - 1; i < code->size - 1; i++)
            enter_code(code, i, A, code->tab_value[0][i + 1], x_iterator);
        enter_code(code, code->size - 2, C, x_iterator, SPACE);
    }
}

struct machine_code *compile(int x) {
    int size = 1;
    char *tab_name = malloc(sizeof(char));
    unsigned int **tab_value = malloc((unsigned int) 2 * sizeof(unsigned int *));
    for (int i = 0; i < 2; i++)
        tab_value[i] = malloc(sizeof(int));
    struct machine_code *code = malloc(sizeof(struct machine_code));
    code->size = size;
    code->size_all = size;
    code->tab_value = tab_value;
    code->tab_name = tab_name;
    while (x != '\n') {
        if (x >= ASCII && x < ASCII + VARIABLES) {
            code->size++;
            realloc_code(code);
            enter_code(code, code->size - 2, I, x, SPACE);
        } else
            repeat(code);
        x = getchar();
    }
    enter_code(code, code->size - 1, H, SPACE, SPACE);
    //write_code(*code);
    return code;
}

void interpret(struct machine_code *code, struct variables *v) {
    unsigned int i = 0;
    while (i < code->size) {
        int x = (unsigned char) code->tab_name[i];
        switch (x) {
            case I:
                increment(v, code->tab_value[0][i] - ASCII);
                break;
            case A:
                add(v, code->tab_value[0][i] - ASCII, code->tab_value[1][i] - ASCII);
                break;
            case C:
                clear(v, code->tab_value[0][i] - ASCII);
                break;
            case J:
                i = code->tab_value[0][i] - 1;
                break;
            case D:
                if (v->tab[code->tab_value[0][i] - ASCII][0] == 0 && v->tab_size[code->tab_value[0][i] - ASCII] == 1)
                    i = code->tab_value[1][i] - 1;
                else
                    decrement(v, code->tab_value[0][i] - ASCII);
                break;
            case H:
                i = code->size;
                break;
            default:
                break;
        }
        i++;
    }
    for (int j = 0; j < 2; j++)
        free(code->tab_value[j]);
    free(code->tab_name);
    free(code->tab_value);
    free(code);
}

void run(struct variables *v) {
    int x = getchar();
    while (x != EOF) {
        if (x == '=') {
            x = getchar();
            write(v->tab[x - ASCII], v->tab_size[x - ASCII]);
        } else if (x != '\n')
            interpret(compile(x), v);
        x = getchar();
    }
}

int main(void) {
    int tab_size[VARIABLES];
    int tab_size_all[VARIABLES];
    for (int i = 0; i < VARIABLES; i++) {
        tab_size[i] = 1;
        tab_size_all[i] = 1;
    }
    int **tab = malloc(VARIABLES * sizeof(int *));
    for (int i = 0; i < VARIABLES; i++) {
        tab[i] = malloc(sizeof(int));
        tab[i][0] = 0;
    }
    struct variables v = {tab, tab_size, tab_size_all};
    run(&v);
    for (int i = 0; i < VARIABLES; i++)
        free(tab[i]);
    free(tab);
    return 0;
}
