#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Necessário para a função tolower()

// --- 1. Definição da Estrutura do Nó (Sala) ---

/**
 * @brief Estrutura de um nó da árvore binária, representando um cômodo da mansão.
 */
typedef struct Sala
{
    char nome[50];
    struct Sala *esquerda; // Caminho para a esquerda
    struct Sala *direita;  // Caminho para a direita
} Sala;

// --- 2. Funções de Criação e Inicialização ---

/**
 * @brief Cria um novo nó (sala) com o nome especificado.
 * @param nome O nome do cômodo.
 * @return Um ponteiro para a nova Sala criada.
 */
Sala *criarSala(const char *nome)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        perror("Erro ao alocar memória para Sala");
        exit(EXIT_FAILURE);
    }
    // Copia o nome para a estrutura
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0';
    // Inicializa os ponteiros dos filhos como NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Libera a memória alocada para a árvore binária de salas.
 * @param raiz O nó raiz da subárvore a ser liberada.
 */
void liberarArvore(Sala *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// --- 3. Funções de Navegação e Lógica do Jogo ---

/**
 * @brief Função principal para navegação interativa na mansão.
 * @param salaAtual O ponteiro para a sala onde o jogador está atualmente.
 */
void explorarSalas(Sala *salaAtual)
{
    char escolha;

    // Se a sala atual for NULL (ocorre apenas se o jogador tentar entrar em um caminho NULL)
    if (salaAtual == NULL)
    {
        printf("\n❌ Fim do caminho! Você chegou a uma parede ou saiu da mansão.\n");
        return;
    }

    printf("\n🚪 Você entrou na sala: %s\n", salaAtual->nome);

    // Verifica se é um nó folha
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL)
    {
        printf("🎉 Você chegou ao fim deste caminho da mansão !\n");
        return;
    }

    printf(" Escolha o próximo caminho:\n\n");

    // Exibe as opções de forma dinâmica
    if (salaAtual->esquerda != NULL)
    {
        printf("  [e] -> Esquerda (%s)\n", salaAtual->esquerda->nome);
    }
    else
    {
        printf("  [e] -> Esquerda (Caminho Bloqueado 🚧)\n");
    }

    if (salaAtual->direita != NULL)
    {
        printf("  [d] -> Direita (%s)\n", salaAtual->direita->nome);
    }
    else
    {
        printf("  [d] -> Direita (Caminho Bloqueado 🚧)\n");
    }

    printf("  [s] -> Sair da Exploração\n");
    printf("\n Sua escolha: ");

    // Leitura da escolha do usuário e limpeza do buffer de entrada
    if (scanf(" %c", &escolha) != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("\n⚠️ Entrada inválida. Tente novamente.\n");
        explorarSalas(salaAtual); // Tenta novamente
        return;
    }

    // Converte para minúsculo
    escolha = tolower(escolha);

    // Processa a escolha
    switch (escolha)
    {
    case 'e':
        if (salaAtual->esquerda != NULL)
        {
            explorarSalas(salaAtual->esquerda);
        }
        else
        {
            printf("\n🚫 Caminho para a Esquerda Bloqueado! Escolha outra direção.\n");
            explorarSalas(salaAtual); // Volta à sala atual
        }
        break;
    case 'd':
        if (salaAtual->direita != NULL)
        {
            explorarSalas(salaAtual->direita);
        }
        else
        {
            printf("\n🚫 Caminho para a Direita Bloqueado! Escolha outra direção.\n");
            explorarSalas(salaAtual); // Volta à sala atual
        }
        break;
    case 's':
        printf("\n👋 Saindo da exploração da mansão. Obrigado por jogar!\n");
        break;
    default:
        printf("\n⚠️  Opção inválida. Por favor, escolha:'e', 'd', ou 's'.\n");
        explorarSalas(salaAtual); // Volta à sala atual
        break;
    }
}

// --- 4. Função Principal (main) ---

int main()
{
    printf("=============================================\n");
    printf(" 🔎 Detective Quest - Nível Novato  \n");
    printf("         🗺️  MAPA DA MANSÃO 🧭\n");
    printf("=============================================\n");

    // 💡 Montagem da Árvore Binária Estática (O Mapa da Mansão)
    // Raiz (Nível 0)
    Sala *hallEntrada = criarSala("Hall de Entrada");

    // Nível 1
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;

    // Nível 2 - Ramo Esquerdo (Biblioteca)
    biblioteca->esquerda = criarSala("Estufa"); // Nó Folha
    Sala *escritorio = criarSala("Escritório");
    biblioteca->direita = escritorio;

    // Nível 2 - Ramo Direito (Cozinha)
    cozinha->esquerda = criarSala("Quarto Principal"); // Nó Folha
    // cozinha->direita fica NULL (caminho bloqueado)

    // Nível 3 - Ramo Esquerdo (Escritório)
    escritorio->direita = criarSala("Porão"); // Nó Folha
    // escritorio->esquerda fica NULL (caminho bloqueado)

    // ----------------------------------------------------

    // Início do Jogo
    printf("\n Iniciando a exploração da mansão a partir do - Hall de Entrada.\n");
    explorarSalas(hallEntrada);

    // Limpeza de memória (Liberação da Árvore)
    liberarArvore(hallEntrada);

    printf("\nPrograma finalizado e memória liberada.\n");

    return 0;
}