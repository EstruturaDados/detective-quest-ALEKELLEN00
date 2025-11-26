#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definição do tamanho da Tabela Hash
#define TAMANHO_HASH 10
#define MAX_NOME 50

// ==========================================================
//                    ESTRUTURAS DE DADOS
// ==========================================================

// --- 1. ESTRUTURA PARA TABELA HASH (Suspeitos & Pistas) ---

/**
 * @brief Estrutura do nó da lista encadeada da Tabela Hash (cada nó é uma associação).
 */
typedef struct Associacao
{
    char pista[100];
    char suspeito[MAX_NOME];
    struct Associacao *proximo;
} Associacao;

// A Tabela Hash é um array de ponteiros para Associacao.
Associacao *tabelaHash[TAMANHO_HASH];

// --- 2. ESTRUTURA PARA PISTA (Nó da ÁRVORE DE BUSCA BINÁRIA - BST) ---
typedef struct Pista
{
    char descricao[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// --- 3. ESTRUTURA PARA SALA (Nó da ÁRVORE BINÁRIA DE NAVEGAÇÃO) ---
typedef struct Sala
{
    char nome[MAX_NOME];
    char pista_encontrada[100];
    char suspeito_associado[MAX_NOME]; // Novo: Suspeito vinculado a esta pista
    int pista_coletada;
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ==========================================================
//                 FUNÇÕES DA TABELA HASH
// ==========================================================

/**
 * @brief Função de espalhamento (Hashing Function).
 * Baseada na soma dos valores ASCII dos 4 primeiros caracteres da pista.
 * @param chave A string (pista) a ser hasheada.
 * @return O índice da tabela hash (0 a TAMANHO_HASH - 1).
 */
int funcaoHash(const char *chave)
{
    unsigned int soma = 0;
    for (int i = 0; i < 4 && chave[i] != '\0'; i++)
    {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * @brief Cria um novo nó de Associação.
 */
Associacao *criarAssociacao(const char *pista, const char *suspeito)
{
    Associacao *nova = (Associacao *)malloc(sizeof(Associacao));
    if (nova == NULL)
    {
        perror("Erro ao alocar memória para Associacao");
        exit(EXIT_FAILURE);
    }
    strncpy(nova->pista, pista, sizeof(nova->pista) - 1);
    nova->pista[sizeof(nova->pista) - 1] = '\0';
    strncpy(nova->suspeito, suspeito, sizeof(nova->suspeito) - 1);
    nova->suspeito[sizeof(nova->suspeito) - 1] = '\0';
    nova->proximo = NULL;
    return nova;
}

/**
 * @brief Insere uma nova associação Pista-Suspeito na Tabela Hash.
 */
void inserirNaHash(const char *pista, const char *suspeito)
{
    int indice = funcaoHash(pista);

    // Verifica se a associação já existe (evita duplicação)
    Associacao *atual = tabelaHash[indice];
    while (atual != NULL)
    {
        if (strcmp(atual->pista, pista) == 0)
        {
            // Se já existe, apenas garante que o suspeito está correto (ou ignora, conforme simplificação)
            // printf("Associação '%s' -> '%s' já existe. Ignorando.\n", pista, suspeito);
            return;
        }
        atual = atual->proximo;
    }

    // Cria e insere no início da lista (encadeamento)
    Associacao *nova = criarAssociacao(pista, suspeito);
    nova->proximo = tabelaHash[indice];
    tabelaHash[indice] = nova;

    // printf("Associação Hash: '%s' -> '%s' (Índice %d)\n", pista, suspeito, indice); // Debug
}

/**
 * @brief Inicializa a Tabela Hash com NULL.
 */
void inicializarHash()
{
    for (int i = 0; i < TAMANHO_HASH; i++)
    {
        tabelaHash[i] = NULL;
    }
}

/**
 * @brief Libera a memória alocada para a Tabela Hash.
 */
void liberarHash()
{
    for (int i = 0; i < TAMANHO_HASH; i++)
    {
        Associacao *atual = tabelaHash[i];
        Associacao *temp;
        while (atual != NULL)
        {
            temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

// ==========================================================
//             FUNÇÕES DE ANÁLISE E DEDUÇÃO
// ==========================================================

/**
 * @brief Analisa a Tabela Hash e identifica o suspeito mais citado.
 */
void analisarEvidencias()
{
    printf("\n=============================================\n");
    printf("🕵️  ANÁLISE DE EVIDÊNCIAS (DEDUÇÃO) \n");
    printf("=============================================\n");

    // Usa um array simples para contagem de votos, pois o número de suspeitos é pequeno e fixo.
    // Estrutura mais robusta usaria outra Hash ou BST para contagem, mas simplificaremos.

    // Supondo 3 Suspeitos Fixos: "Mordomo", "Jardineiro", "Dama"
    char *suspeitos[] = {"Mordomo", "Jardineiro", "Dama"};
    int contagem[3] = {0, 0, 0};
    int num_suspeitos = 3;

    int total_pistas = 0;

    // 1. Percorre a Tabela Hash e Conta as Citações
    for (int i = 0; i < TAMANHO_HASH; i++)
    {
        Associacao *atual = tabelaHash[i];
        while (atual != NULL)
        {
            total_pistas++;
            printf("Evidência: '%s' -> Suspeito: %s\n", atual->pista, atual->suspeito);

            // Incrementa o contador do suspeito correspondente
            for (int j = 0; j < num_suspeitos; j++)
            {
                if (strcmp(atual->suspeito, suspeitos[j]) == 0)
                {
                    contagem[j]++;
                    break;
                }
            }
            atual = atual->proximo;
        }
    }

    if (total_pistas == 0)
    {
        printf("Não há pistas coletadas para realizar a dedução.\n");
        return;
    }

    // 2. Encontra o suspeito mais citado
    int max_citacoes = -1;
    char culpado_final[MAX_NOME] = "INDEFINIDO";
    int empates = 0;

    for (int i = 0; i < num_suspeitos; i++)
    {
        printf("\nTotal de Pistas ligadas a %s: %d", suspeitos[i], contagem[i]);
        if (contagem[i] > max_citacoes)
        {
            max_citacoes = contagem[i];
            strncpy(culpado_final, suspeitos[i], MAX_NOME - 1);
            empates = 0; // Novo máximo, zera empates
        }
        else if (contagem[i] == max_citacoes)
        {
            empates = 1; // Encontrou um empate
        }
    }

    // 3. Exibe o resultado final
    printf("\n\n---------------------------------------------\n");
    if (empates)
    {
        printf("🛑 DEDUÇÃO FINAL: EMPATE!\n");
        printf("Vários suspeitos têm o mesmo número máximo de %d evidências.\n", max_citacoes);
    }
    else
    {
        printf("🎉 DEDUÇÃO FINAL: O suspeito mais citado é: %s\n", culpado_final);
        printf("Com um total de %d evidências encontradas.\n", max_citacoes);
    }
    printf("---------------------------------------------\n");
}

// ==========================================================
//            FUNÇÕES DA BST E NAVEGAÇÃO (Reutilizadas)
// ==========================================================

// --- BST de Pistas ---

Pista *criarPista(const char *descricao)
{
    Pista *novaPista = (Pista *)malloc(sizeof(Pista));
    if (novaPista == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strncpy(novaPista->descricao, descricao, sizeof(novaPista->descricao) - 1);
    novaPista->descricao[sizeof(novaPista->descricao) - 1] = '\0';
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

Pista *inserirPista(Pista *raiz, const char *descricao, const char *suspeito_a_associar)
{
    if (raiz == NULL)
    {
        printf("\n✅ Pista '%s' adicionada ao Diário! (Suspeito: %s)\n", descricao, suspeito_a_associar);
        // NOVO: Insere a associação na Tabela Hash
        inserirNaHash(descricao, suspeito_a_associar);
        return criarPista(descricao);
    }

    int comparacao = strcmp(descricao, raiz->descricao);

    if (comparacao < 0)
    {
        raiz->esquerda = inserirPista(raiz->esquerda, descricao, suspeito_a_associar);
    }
    else if (comparacao > 0)
    {
        raiz->direita = inserirPista(raiz->direita, descricao, suspeito_a_associar);
    }
    else
    {
        printf("⚠️ Pista '%s' duplicada ignorada.\n", descricao);
    }

    return raiz;
}

void liberarPistas(Pista *raiz)
{
    if (raiz != NULL)
    {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// --- Árvore de Salas ---

Sala *criarSala(const char *nome, const char *pista_inicial, const char *suspeito_assoc)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        exit(EXIT_FAILURE);
    }

    strncpy(novaSala->nome, nome, MAX_NOME - 1);
    strncpy(novaSala->pista_encontrada, pista_inicial, sizeof(novaSala->pista_encontrada) - 1);
    strncpy(novaSala->suspeito_associado, suspeito_assoc, MAX_NOME - 1);

    novaSala->pista_coletada = (strlen(pista_inicial) == 0);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

void liberarArvoreSalas(Sala *raiz)
{
    if (raiz != NULL)
    {
        liberarArvoreSalas(raiz->esquerda);
        liberarArvoreSalas(raiz->direita);
        free(raiz);
    }
}

void explorarSalas(Sala *salaAtual, Pista **pistasRaiz)
{
    char escolha;

    if (salaAtual == NULL)
    {
        printf("\n❌ Fim do caminho! Você chegou a uma parede.\n");
        return;
    }

    printf("\n-------------------------------------------------\n");
    printf("🚪 Você está em: %s\n", salaAtual->nome);

    // --- Lógica de Encontrar e Coletar Pista (NOVO: Associa Suspeito) ---
    if (strlen(salaAtual->pista_encontrada) > 0 && salaAtual->pista_coletada == 0)
    {
        printf("\n 🌟 PISTA ENCONTRADA! Você encontrou: \"%s\"\n", salaAtual->pista_encontrada);
        printf("  Esta pista está ligada ao: %s \n", salaAtual->suspeito_associado);

        // Insere a pista na BST E a associação na Tabela Hash
        *pistasRaiz = inserirPista(*pistasRaiz, salaAtual->pista_encontrada, salaAtual->suspeito_associado);
        salaAtual->pista_coletada = 1;
    }

    // Verifica se é um nó folha
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL)
    {
        printf("\n🎉 Você chegou ao fim deste caminho da mansão !\n");//Nó-Folha
        printf("\n🤔 Deseja fazer sua dedução final? [a] Analisar Evidências / [s] Sair: ");
        if (scanf(" %c", &escolha) == 1)
        {
            escolha = tolower(escolha);
            if (escolha == 'a')
            {
                analisarEvidencias(); // Chama a função de dedução!
            }
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return;
    }

    // --- Opções de Navegação ---
    printf("\n Escolha o próximo caminho:\n");
    printf("\n  [e] -> Esquerda (%s)\n", salaAtual->esquerda ? salaAtual->esquerda->nome : "Caminho Bloqueado 🚧");
    printf("  [d] -> Direita (%s)\n", salaAtual->direita ? salaAtual->direita->nome : "Caminho Bloqueado 🚧");
    printf("  [a] -> Analisar Evidências Coletadas\n");
    printf("  [s] -> Sair da Exploração\n");
    printf("\n Sua escolha: ");

    if (scanf(" %c", &escolha) != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("\n⚠️ Entrada inválida. Tente novamente.\n");
        explorarSalas(salaAtual, pistasRaiz);
        return;
    }

    escolha = tolower(escolha);

    switch (escolha)
    {
    case 'e':
        if (salaAtual->esquerda != NULL)
        {
            explorarSalas(salaAtual->esquerda, pistasRaiz);
        }
        else
        {
            printf("\n🚫 Caminho Bloqueado! Tente outra direção.\n");
            explorarSalas(salaAtual, pistasRaiz);
        }
        break;
    case 'd':
        if (salaAtual->direita != NULL)
        {
            explorarSalas(salaAtual->direita, pistasRaiz);
        }
        else
        {
            printf("\n🚫 Caminho Bloqueado! Tente outra direção.\n");
            explorarSalas(salaAtual, pistasRaiz);
        }
        break;
    case 'a':
        analisarEvidencias(); // Opção de análise durante o jogo
        explorarSalas(salaAtual, pistasRaiz);
        break;
    case 's':
        printf("\n👋 Saindo da exploração da mansão.\n");
        break;
    default:
        printf("\n⚠️ Opção inválida. Por favor, escolha 'e', 'd', 'a', ou 's'.\n");
        explorarSalas(salaAtual, pistasRaiz);
        break;
    }
}

// --- 5. Função Principal (main) ---

int main()
{
    Pista *pistasRaiz = NULL;

    printf("=============================================\n");
    printf(" 👑 Detective Quest - Nível Mestre \n");
    printf("  Hash Table (Suspeitos & Dedução)\n");
    printf("=============================================\n");

    // Inicializa a Tabela Hash
    inicializarHash();

    //  Montagem da Árvore Binária (Mapa) com Pistas e Suspeitos
    // Argumentos de criarSala: (nome, pista_encontrada, suspeito_associado)

    // Raiz (Nível 0)
    Sala *hallEntrada = criarSala("Hall de Entrada", "", "");

    // Nível 1
    Sala *biblioteca = criarSala("Biblioteca", "Lupa quebrada", "Mordomo");
    Sala *cozinha = criarSala("Cozinha", "Faca de prata", "Jardineiro");
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;

    // Nível 2 - Ramo Esquerdo (Biblioteca)
    Sala *estufa = criarSala("Estufa", "Pegadas de barro", "Jardineiro"); // Nó Folha
    Sala *escritorio = criarSala("Escritório", "Carta rasgada", "Dama");
    biblioteca->esquerda = estufa;
    biblioteca->direita = escritorio;

    // Nível 2 - Ramo Direito (Cozinha)
    cozinha->esquerda = criarSala("Quarto Principal", "Luva de seda", "Dama"); // Nó Folha
    // cozinha->direita fica NULL

    // Nível 3 - Ramo Esquerdo (Escritório)
    escritorio->esquerda = criarSala("Sala de Jantar", "Poeira de veneno", "Mordomo");
    escritorio->direita = criarSala("Porão", "Chave enferrujada", "Mordomo"); // Nó Folha
    // Sala de Jantar é Nó Folha

    // ----------------------------------------------------

    // Início do Jogo
    printf("\n Iniciando a investigação! Colete as pistas para ligá-las aos Suspeitos.\n");
    printf(" Suspeitos Fixos: Mordomo, Jardineiro, Dama!\n");
    explorarSalas(hallEntrada, &pistasRaiz);

    // Tentativa final de dedução (caso o jogador saia antes de um nó folha)
    if (pistasRaiz != NULL)
    {
        printf("📊 Análise final ao sair do jogo:\n");
        analisarEvidencias();
    }

    // Limpeza de memória
    liberarArvoreSalas(hallEntrada);
    liberarPistas(pistasRaiz);
    liberarHash();

    printf("\nPrograma finalizado e memória liberada.\n");

    return 0;
}