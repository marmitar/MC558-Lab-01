# MC558 - Tarefa 1

## Problema

### Quem são meus amigos?

Dr. Sabin Ada, um famoso especialista em Teoria dos Grafos (e programador nas horas vagas) foi convidado a uma festa. Na festa havia *n* pessoas, digamos 1, 2, ..., *n*. Dr. Ada sabia quantos amigos cada pessoa tinha, mas não quem era amigo de quem.

O brilhante cientista então pensou numa solução para reconstruir o “grafo de amizade” das pessoas da festa, ao se lembrar da ideia de sequências gráficas. Sua tarefa é ajudar o Dr. Ada a implementar sua ideia. Para os propósitos deste trabalho, suporemos que se *A* é amigo de *B*, então *B* é amigo de *A*.

PS: Infelizmente, a ideia do Dr. Ada não é completamente satisfatória, pois pode haver mais de um modo de reconstruir o grafo. Para reconstruir exatamente o grafo, seria necessário mais informações do que ele tinha. Mas isto é assunto para outra disciplina...

### Sequências gráficas

Seja `G` um grafo com `V(G) = {v[1], ..., v[n]}` tal que `d(v[i]) ≥ d(v[i+1])` para `i = 1, 2, ..., n−1`. Chamamos a sequência `(d(v[1]), ..., d(v[n]))` de sequência de graus de `G`. Dizemos que uma sequência não-crescente de inteiros não-negativos `d = (d[1], ..., d[n])` é uma **sequência gráfica** se existe algum grafo **simples** `G` cuja sequência de graus é exatamente `d`. Por exemplo, `(4,4,3,3,3,3,2)` e `(4,3,3,3,3,2,2)` são sequências gráficas, mas `(7,6,5,4,3,3,2)` e `(6,6,5,4,3,3,1)` não são.

Seja `d = (d[1], ..., d[n])` uma sequência não-crescente de inteiros não-negativos. Seja `d' = (d[2]−1, d[3]−1, ..., d[d[1]+1]−1, d[d[1]+2], ..., d[n])`. Pode-se mostrar que `d` é gráfica se e somente se `d'` é gráfica (Exercı́cio!). Usando este fato, projete um algoritmo que recebe uma sequência não-crescente de inteiros não-negativos `d = (d[1], ..., d[n])` e que devolve:

- um grafo **simples** descrito por listas de adjacências cuja sequência de graus é `d`, ou

- responde `Não é sequência gráfica!`.

## Entrada e Saída

**Entrada:** cada caso de teste terá duas linhas como entrada. A primeira linha contém um inteiro *n* *(1 ≤ n ≤ 100)* que corresponde ao número de elementos da sequência *d*. A segunda linha contém os *n* inteiros não-negativos que correspondem à sequência *d*. Os elementos estão em ordem não-crescente e são separados por um espaço.

**Saı́da:** caso exista um grafo simples *G* com a sequência de graus exatamente igual à *d*, o programa devera imprimir a lista de adjacências de *G*. Os vértices devem ser rotulados por {1,2,...,*n*} e cada lista deve estar ordenada em ordem crescente: a linha 1 deve conter os rótulos dos vértices adjacentes ao vértice 1, a linha 2 deve conter os rótulos dos vértices adjacentes ao vértice 2, e assim sucessivamente. Em cada linha os rótulos devem estar separados por um espaço. Se houver um vértice *i* com grau zero, o programa deve imprimir somente uma quebra de linha (`\n`) na linha *i*. Caso não exista grafo simples com sequência de graus *d*, o programa deverá imprimir apenas a mensagem `Não é sequência gráfica!` (sem quebra de linha).

### Exemplos

```bash
$ ./t1 << EOF
  4
  3 2 2 1
EOF
2 3 4
1 3
1 2
1
```

```bash
$ ./t1 << EOF
  4
  4 2 2 2
EOF
Não é sequência gráfica!%
```

## Implementação e Submissão

-  A solução deverá ser implementada em C, C++, Pascal ou Python 2 / Python 3.

- O programa deve ser submetido no SuSy, com o nome principal **t1** (por exemplo, `t1.c`).

- O número máximo de submissões é 20.

- A tarefa contém 10 testes abertos e 10 testes fechados. A nota será proporcional ao número de acertos nos testes fechados.

## Prazo

O prazo de entrega é de duas semanas. **Dia 12/04/2021.**
