#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <alloca.h>

// Atributos do GCC
#define attribute(...) __attribute__((__VA_ARGS__))
// Marcador de branch improvável
#define unlikely(x)    (__builtin_expect(!!(x), 0))


/* Sequência de números. */
typedef struct sequencia {
    // tamanho da seq.
    size_t tam;
    // vetor de valores
    unsigned v[];
} sequencia_t;

/* Nó da lista ligada. */
typedef struct no {
    unsigned valor;
    struct no *prox;
} no_t;

/* Lista ligada. */
typedef struct lista {
    // nó inicial da lista
    no_t *ini;
} lista_t;

/* Grafo em listas de adjacências. */
typedef struct grafo {
    // no. de vértices no grafo
    size_t tam;
    // adjacências de cada vértice
    lista_t adj[];
} grafo_t;


static inline
// Transforma sequência em um grafo.
grafo_t *sequencia_em_grafo(sequencia_t *d)
attribute(malloc, nonnull, hot, nothrow);

static inline
// Imprime grafo na stream.
void imprime_grafo(FILE *restrict stream, const grafo_t *restrict g)
attribute(nonnull, cold, nothrow);

static
// Desalocação do grafo.
void grafo_dealloc(grafo_t *g)
attribute(nonnull, cold, nothrow);


static attribute(nonnull, noreturn, cold, nothrow)
/**
 *  Encerra o programa com uma mensagem na saída de erro.
 * Se `errnum` é zero, usa o valor setado em `errno`.
 */
void encera(int errnum) {
    if (errnum == 0) {
        errnum = errno;
    }
    const char *msg = strerror(errnum);
    fprintf(stderr, "ERRO: %s\n", msg);

    exit(EXIT_FAILURE);
}

static inline attribute(malloc, cold, nothrow)
/**
 * Leitura da sequência de graus pela entrada padrão.
 *
 * Encerra o programa em caso de erro de leitura.
 */
sequencia_t *ler_entrada(void) {
    size_t n;
    // leitura do tamanho
    if unlikely(scanf("%zu", &n) < 1) {
        encera(EINVAL);
    }
    // alocaçã da sequência
    size_t base = offsetof(struct sequencia, v);
    sequencia_t *d = malloc(base + n * sizeof(unsigned));
    if unlikely(d == NULL) encera(0);
    // leitura dos elementos
    for (size_t i = 0; i < n; i++) {
        if unlikely(scanf("%u", &d->v[i]) < 1) {
            free(d);
            encera(EINVAL);
        }
    }

    d->tam = n;
    return d;
}

/**
 * Ponto de entrada da execução do programa.
 */
int main(void) {
    sequencia_t *d = ler_entrada();

    grafo_t *g = sequencia_em_grafo(d);
    free(d);

    if (g == NULL) {
        printf("Não é sequência gráfica!");
    } else {
        imprime_grafo(stdout, g);
        grafo_dealloc(g);
    }
    return EXIT_SUCCESS;
}

static inline attribute(malloc, cold, nothrow)
/**
 *  Inicialização de um grafo de `tam` vértices,
 * sem nenhuma aresta entre eles.
 *
 * Encerra o programa em erro de alocação.
 */
grafo_t *grafo_init(size_t tam) {
    size_t base = offsetof(grafo_t, adj);
    grafo_t *g = malloc(base + tam * sizeof(lista_t));
    if unlikely(g == NULL) encera(0);

    g->tam = tam;
    for (size_t i = 0; i < tam; i++) {
        // lista de adj. vazia
        g->adj[i] = (lista_t) { .ini = NULL };
    }
    return g;
}

/**
 *  Desaloca memória usada pelo grafo.
 */
void grafo_dealloc(grafo_t *g) {
    for (size_t i = 0; i < g->tam; i++) {
        // desaloca elementos da lista
        for (no_t *ptr = g->adj[i].ini; ptr != NULL;) {
            no_t *prox = ptr->prox;
            free(ptr);
            ptr = prox;
        }
    }
    free(g);
}

static attribute(nonnull, hot, nothrow)
/**
 * Insere elemento na lista mantendo ordenação.
 *
 * Encerra o programa em erro de alocação.
 */
void lista_insere(lista_t *lista, unsigned valor) {
    no_t *novo = malloc(sizeof(no_t));
    if unlikely(novo == NULL) encera(0);

    // busca posição que mantém ordem
    no_t **ptr = &lista->ini;
    while (*ptr != NULL && (*ptr)->valor < valor) {
        ptr = &(*ptr)->prox;
    }

    novo->valor = valor;
    novo->prox = *ptr;
    *ptr = novo;
}

static inline attribute(nonnull, hot, nothrow)
/**
 * Cria aresta entre dois vértices no grafo. Os vértices
 * `i` e `j` devem ser menores que `g->tam`.
 *
 * Encerra em erro de alocação.
 */
void grafo_criar_aresta(grafo_t *g, unsigned i, unsigned j) {
    lista_insere(&g->adj[i], j);
    lista_insere(&g->adj[j], i);
}

static inline attribute(hot, nothrow)
/**
 * Ordena permutação de vértices por número de arestas livres.
 *
 * @param v: vetor de permutação
 * @param d: vetor com qtde de arestas livres em cada vértice `v[i]`
 * @param tam: tamanho do vetor
 * @param from: onde iniciar a ordenação
 */
void ordena_vertices(unsigned *restrict v, unsigned *restrict d, size_t tam, size_t from) {
    // insertion sort, já que o vetor deve estar quase ordenado
    for (size_t i = from; i < tam; i++) {
        unsigned di = d[i];
        unsigned vi = v[i];

        size_t j = i;
        for (; (j > from) && (di > d[j-1]); j--) {
            d[j] = d[j-1];
            v[j] = v[j-1];
        }
        d[j] = di;
        v[j] = vi;
    }
}

/**
 *  Tenta encontrar um grafo válido a partir de uma sequência
 * de graus dos vértices. Retorna `NULL` se não for possível.
 *
 * Encerra o programa em erro de alocação.
 */
grafo_t *sequencia_em_grafo(sequencia_t *s) {
    size_t tam = s->tam;
    unsigned *d = s->v;
    grafo_t *g = grafo_init(tam);
    // permutação dos vértices, da ordem de leitura para a nova
    // ordenação não-crescente em relação ao número de arestas livres
    unsigned *v = alloca(tam * sizeof(unsigned));
    for (size_t i = 0; i < tam; i++) {
        v[i] = i;
    }

    for (size_t i = 0; i < g->tam; i++) {
        ordena_vertices(v, d, tam, i);

        // cria aresta nos próximos `d[i]` vértices
        // com maior número de arestas livres
        for (size_t j = i + 1; j <= i + d[i]; j++) {
            if (j >= g->tam || d[j] == 0) {
                grafo_dealloc(g);
                return NULL;
            }
            grafo_criar_aresta(g, v[i], v[j]);
            d[j] -= 1;
        }
    }
    return g;
}

/**
 *  Imprime as listas de adjacência dos elementos do grafo, cada
 * lista em uma linha. Listas viram apenas uma quebra de linha.
 */
void imprime_grafo(FILE *restrict stream, const grafo_t *restrict g) {

    for (size_t i = 0; i < g->tam; i++) {
        const no_t *no = g->adj[i].ini;

        if (no != NULL) {
            fprintf(stream, "%u", no->valor + 1);
            no = no->prox;
        }
        for (; no != NULL; no = no->prox) {
            fprintf(stream, " %u", no->valor + 1);
        }
        fprintf(stream, "\n");
    }
}
