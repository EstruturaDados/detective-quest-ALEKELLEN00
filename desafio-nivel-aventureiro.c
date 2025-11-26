#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ==========================================================
//                    ESTRUTURAS DE DADOS
// ==========================================================

// 1. ESTRUTURA PARA PISTA (Nó da ÁRVORE DE BUSCA BINÁRIA - BST)
/**
 * @brief Estrutura de um nó da Árvore de Busca Binária (BST), representando uma Pista.
 * A ordenação é feita pela string 'descricao'.
 */
typedef struct Pista
{
    char descricao[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// 2. ESTRUTURA PARA SALA (Nó da ÁRVORE BINÁRIA DE NAVEGAÇÃO)
/**
 * @brief Estrutura de um nó da Árvore Binária, representando um cômodo da mansão.
 */
typedef struct Sala
{
    char nome[50];
    char pista_encontrada[100]; // Pista que pode ser encontrada nesta sala (se não estiver vazia)
    int pista_coletada;         // Flag para evitar coletar a mesma pista duas vezes
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ==========================================================
//                 FUNÇÕES DA BST (PISTAS)
// ==========================================================

/**
 * @brief Cria um novo nó de Pista.
 */
Pista *criarPista(const char *descricao)
{
    Pista *novaPista = (Pista *)malloc(sizeof(Pista));
    if (novaPista == NULL)
    {
        perror("Erro ao alocar memória para Pista");
        exit(EXIT_FAILURE);
    }
    strncpy(novaPista->descricao, descricao, sizeof(novaPista->descricao) - 1);
    novaPista->descricao[sizeof(novaPista->descricao) - 1] = '\0';
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

/**
 * @brief Insere uma nova pista na Árvore de Busca Binária (BST).
 * @param raiz A raiz da BST.
 * @param descricao A string da pista a ser inserida.
 * @return A nova raiz da BST.
 */
Pista *inserirPista(Pista *raiz, const char *descricao)
{
    // 1. Caso base: Atingiu NULL, cria o nó e retorna
    if (raiz == NULL)
    {
        printf("✅ Pista '%s' adicionada ao seu Diário de Investigação!\n", descricao);
        return criarPista(descricao);
    }

    // 2. Compara a nova pista com a pista na raiz (ordem alfabética)
    int comparacao = strcmp(descricao, raiz->descricao);

    if (comparacao < 0)
    { // Nova pista é menor (alfabeticamente), vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    }
    else if (comparacao > 0)
    { // Nova pista é maior, vai para a direita
        raiz->direita = inserirPista(raiz->direita, descricao);
    }
    else
    {
        // Pista duplicada
        printf("⚠️ Pista '%s' já havia sido coletada e foi ignorada.\n", descricao);
    }

    return raiz;
}

/**
 * @brief Percorre a BST em ordem (In-Order) para listar as pistas em ordem alfabética.
 * @param raiz A raiz da BST.
 */
void listarPistasEmOrdem(Pista *raiz)
{
    if (raiz != NULL)
    {
        // Percorre Esquerda (Menores)
        listarPistasEmOrdem(raiz->esquerda);

        // Visita a Raiz (Imprime a pista)
        printf("   -> %s\n", raiz->descricao);

        // Percorre Direita (Maiores)
        listarPistasEmOrdem(raiz->direita);
    }
}

/**
 * @brief Libera a memória alocada para a BST de pistas.
 */
void liberarPistas(Pista *raiz)
{
    if (raiz != NULL)
    {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// ==========================================================
//              FUNÇÕES DA ÁRVORE DE NAVEGAÇÃO
// ==========================================================

/**
 * @brief Cria um novo nó (sala) com o nome e a pista inicial.
 */
Sala *criarSala(const char *nome, const char *pista_inicial)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        perror("Erro ao alocar memória para Sala");
        exit(EXIT_FAILURE);
    }
    // Inicialização
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0';

    strncpy(novaSala->pista_encontrada, pista_inicial, sizeof(novaSala->pista_encontrada) - 1);
    novaSala->pista_encontrada[sizeof(novaSala->pista_encontrada) - 1] = '\0';

    novaSala->pista_coletada = (strlen(pista_inicial) == 0); // Já coletada se não houver pista
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Libera a memória alocada para a árvore binária de salas.
 */
void liberarArvoreSalas(Sala *raiz)
{
    if (raiz != NULL)
    {
        liberarArvoreSalas(raiz->esquerda);
        liberarArvoreSalas(raiz->direita);
        free(raiz);
    }
}

// ==========================================================
//               LÓGICA DO JOGO E INTERAÇÃO
// ==========================================================

/**
 * @brief Função principal para navegação interativa na mansão.
 * @param salaAtual O ponteiro para a sala onde o jogador está atualmente.
 * @param pistasRaiz Ponteiro para a raiz da BST de pistas (usado para inserção).
 */
void explorarSalas(Sala *salaAtual, Pista **pistasRaiz)
{
    char escolha;

    // Caso base de fim de caminho
    if (salaAtual == NULL)
    {
        printf("\n❌ Fim do caminho! Você chegou a uma parede.\n");
        return;
    }

    printf("\n-------------------------------------------------\n");
    printf("🚪 Você está em: %s\n", salaAtual->nome);

    // --- Lógica de Encontrar e Coletar Pista ---
    if (strlen(salaAtual->pista_encontrada) > 0 && salaAtual->pista_coletada == 0)
    {
        printf("\n🌟 PISTA ENCONTRADA! Você encontrou: \"%s\"\n", salaAtual->pista_encontrada);
        // Insere a pista na BST
        *pistasRaiz = inserirPista(*pistasRaiz, salaAtual->pista_encontrada);
        salaAtual->pista_coletada = 1; // Marca como coletada
    }

    // Verifica se é um nó folha
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL)
    {
        printf("\n🎉 Você chegou ao fim deste caminho da mansão !\n");//Nó Folha
        // Opção para listar as pistas ao final de um caminho
        printf("\n📝 Deseja ver seu Diário de Pistas? [l] Listar / [s] Sair: ");
        if (scanf(" %c", &escolha) == 1)
        {
            escolha = tolower(escolha);
            if (escolha == 'l')
            {
                printf("\n=============================================\n");
                printf("📝 DIÁRIO DE PISTAS (Em Ordem Alfabética)\n");
                printf("=============================================\n");
                if (*pistasRaiz == NULL)
                {
                    printf("Nenhuma pista coletada ainda.\n");
                }
                else
                {
                    listarPistasEmOrdem(*pistasRaiz);
                }
                printf("=============================================\n");
            }
        }
        // Limpa o buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return;
    }

    // --- Opções de Navegação ---
    printf("\n Escolha o próximo caminho:\n");
    printf("\n  [e] -> Esquerda (%s)\n", salaAtual->esquerda ? salaAtual->esquerda->nome : "Caminho Bloqueado 🚧");
    printf("  [d] -> Direita (%s)\n", salaAtual->direita ? salaAtual->direita->nome : "Caminho Bloqueado 🚧");
    printf("  [l] -> Listar Pistas Coletadas\n");
    printf("  [s] -> Sair da Exploração\n");
    printf(" \n Sua escolha: ");

    // Leitura da escolha do usuário e limpeza do buffer de entrada
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

    // Processa a escolha
    switch (escolha)
    {
    case 'e':
        if (salaAtual->esquerda != NULL)
        {
            explorarSalas(salaAtual->esquerda, pistasRaiz);
        }
        else
        {
            printf("\n🚫 Caminho para a Esquerda Bloqueado! Escolha outra direção.\n");
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
            printf("\n🚫 Caminho para a Direita Bloqueado! Escolha outra direção.\n");
            explorarSalas(salaAtual, pistasRaiz);
        }
        break;
    case 'l':
        printf("\n=============================================\n");
        printf("📝DIÁRIO DE PISTAS (Em Ordem Alfabética)\n");
        printf("=============================================\n");
        if (*pistasRaiz == NULL)
        {
            printf("Nenhuma pista coletada ainda.\n");
        }
        else
        {
            listarPistasEmOrdem(*pistasRaiz);
        }
        printf("=============================================\n");
        explorarSalas(salaAtual, pistasRaiz); // Retorna à sala atual
        break;
    case 's':
        printf("\n👋 Saindo da exploração da mansão. Obrigado por jogar!\n");
        break;
    default:
        printf("\n⚠️  Opção inválida. Por favor, escolha: 'e', 'd', 'l', ou 's'.\n");
        explorarSalas(salaAtual, pistasRaiz);
        break;
    }
}

// --- 5. Função Principal (main) ---

int main()
{
    // A BST de pistas é iniciada como NULL
    Pista *pistasRaiz = NULL;

    printf("=============================================\n");
    printf(" 🔎 Detective Quest - Nível Aventureiro \n");
    printf("      Mapa (AB) + Pistas (BST)\n");
    printf("=============================================\n");

    // 💡 Montagem da Árvore Binária Estática (O Mapa da Mansão)
    // O segundo argumento de criarSala é a pista encontrada no local.

    // Raiz (Nível 0) - Sem pista
    Sala *hallEntrada = criarSala("Hall de Entrada", "");

    // Nível 1
    Sala *biblioteca = criarSala("Biblioteca", "Lupa quebrada");
    Sala *cozinha = criarSala("Cozinha", "Faca de prata");
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;

    // Nível 2 - Ramo Esquerdo (Biblioteca)
    Sala *estufa = criarSala("Estufa", "Pegadas de barro"); // Nó Folha
    Sala *escritorio = criarSala("Escritório", "Carta rasgada");
    biblioteca->esquerda = estufa;
    biblioteca->direita = escritorio;

    // Nível 2 - Ramo Direito (Cozinha)
    cozinha->esquerda = criarSala("Quarto Principal", "Luva de seda"); // Nó Folha
    // cozinha->direita fica NULL (caminho bloqueado)

    // Nível 3 - Ramo Esquerdo (Escritório)
    escritorio->direita = criarSala("Porão", "Chave enferrujada"); // Nó Folha
    // escritorio->esquerda fica NULL (caminho bloqueado)

    // ----------------------------------------------------

    // Início do Jogo
    printf("\n Iniciando a exploração. Lembre-se, use [l] para listar suas pistas a qualquer momento!\n");
    explorarSalas(hallEntrada, &pistasRaiz);

    // Limpeza de memória
    liberarArvoreSalas(hallEntrada);
    liberarPistas(pistasRaiz);

    printf("\nPrograma finalizado e memória liberada.\n");

    return 0;
}