#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Para a função toupper
#include "docente.h" 

// REQUISITO: Estruturas e arranjos
#define MAX_ALUNOS 50
#define MAX_NOME 100
#define NOME_ARQUIVO_TURMA_MAX 110
#define MEDIA_APROVACAO 7.0

// Estrutura para representar um aluno
typedef struct {
    int id;
    char nome[MAX_NOME];
    float nota1;
    float nota2;
    float media;
    char situacao[20];  // "Aprovado" ou "Reprovado"
} Aluno;

// Estrutura para representar uma turma
typedef struct {
    char nome[MAX_NOME];
    Aluno alunos[MAX_ALUNOS];
    int total_alunos;
} Turma;

// --- Protótipos das Funções ---
// Funções que NÃO interagem com o usuário
void calcularMediaESituacao(Aluno *aluno);
int carregarTurma(Turma *turma, const char *nomeArquivo);
void salvarTurma(const Turma *turma, const char *nomeArquivo);
int adicionarAluno(Turma *turma, Aluno novoAluno);
int encontrarAlunoPorId(const Turma *turma, int id);
int atualizarAluno(Turma *turma, int id, float novaNota1, float novaNota2);
int excluirAluno(Turma *turma, int id);
void formatarNomeArquivo(const char *nomeBase, char *nomeFormatado);

// Funções de interface (usadas dentro da main)
void exibirMenuPrincipal();
void exibirSubMenu(const char *nomeTurma);
void limparBufferEntrada();

// MAIN do código
int main()
{
    Turma turmaAtual;
    turmaAtual.total_alunos = 0;
    char nomeTurma[MAX_NOME];
    char nomeArquivo[NOME_ARQUIVO_TURMA_MAX];
    int opcaoPrincipal, opcaoSubMenu;
    int idAluno;

    // Loop principal do programa com o uso de laços de repetição
    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcaoPrincipal);
        limparBufferEntrada();

        // Uso de fluxo de decisão para o controle da navegação do menu do professor
        switch (opcaoPrincipal) {
            case 1:  // Selecionar/Criar Turma
                printf("\nDigite o nome da turma (ex: Ciencia_da_Computacao_2025): ");
                scanf("%99[^\n]", nomeTurma);
                limparBufferEntrada();

                formatarNomeArquivo(nomeTurma, nomeArquivo);

                if (carregarTurma(&turmaAtual, nomeArquivo)) {
                    printf("\nTurma '%s' carregada com sucesso.\n", turmaAtual.nome);
                } else {
                    printf("\nTurma nao encontrada. Criando nova turma '%s'...\n", nomeTurma);
                    strcpy(turmaAtual.nome, nomeTurma);
                    turmaAtual.total_alunos = 0;
                }

                // Loop do sub-menu da turma
                do {
                    exibirSubMenu(turmaAtual.nome);
                    printf("Escolha uma opcao: ");
                    scanf("%d", &opcaoSubMenu);
                    limparBufferEntrada();

                    switch (opcaoSubMenu) {
                        case 1: {  // Adicionar Aluno
                            if (turmaAtual.total_alunos >= MAX_ALUNOS) {
                                printf("Erro: A turma esta cheia!\n");
                                break;
                            }
                            Aluno novoAluno;
                            printf("Digite o nome do aluno: ");
                            scanf("%99[^\n]", novoAluno.nome);
                            limparBufferEntrada();
                            printf("Digite a primeira nota: ");
                            scanf("%f", &novoAluno.nota1);
                            limparBufferEntrada();
                            printf("Digite a segunda nota: ");
                            scanf("%f", &novoAluno.nota2);
                            limparBufferEntrada();

                            // Chama a função de lógica que não interage com o usuário
                            if (adicionarAluno(&turmaAtual, novoAluno)) {
                                printf("Aluno adicionado com sucesso!\n");
                            }
                            break;
                        }
                        case 2:  // Listar Alunos
                            printf("\n--- Alunos da Turma: %s ---\n", turmaAtual.nome);
                            if (turmaAtual.total_alunos == 0) {
                                printf("Nenhum aluno cadastrado nesta turma.\n");
                            } else {
                                for (int i = 0; i < turmaAtual.total_alunos; i++) {
                                    Aluno aluno = turmaAtual.alunos[i];
                                    printf("ID: %d | Nome: %s | Nota 1: %.2f | Nota 2: %.2f | Media: %.2f | Situacao: %s\n",
                                           aluno.id, aluno.nome, aluno.nota1, aluno.nota2, aluno.media, aluno.situacao);
                                }
                            }
                            printf("----------------------------------\n");
                            break;
                        case 3: {  // Atualizar Aluno
                            printf("Digite o ID do aluno que deseja atualizar: ");
                            scanf("%d", &idAluno);
                            limparBufferEntrada();

                            float novaNota1, novaNota2;
                            printf("Digite a nova primeira nota: ");
                            scanf("%f", &novaNota1);
                            limparBufferEntrada();
                            printf("Digite a nova segunda nota: ");
                            scanf("%f", &novaNota2);
                            limparBufferEntrada();

                            if (atualizarAluno(&turmaAtual, idAluno, novaNota1, novaNota2)) {
                                printf("Dados do aluno atualizados com sucesso!\n");
                            } else {
                                printf("Erro: Aluno com ID %d nao encontrado.\n", idAluno);
                            }
                            break;
                        }
                        case 4: {  // Excluir Aluno
                            printf("Digite o ID do aluno que deseja excluir: ");
                            scanf("%d", &idAluno);
                            limparBufferEntrada();

                            if (excluirAluno(&turmaAtual, idAluno)) {
                                printf("Aluno excluido com sucesso!\n");
                            } else {
                                printf("Erro: Aluno com ID %d nao encontrado.\n", idAluno);
                            }
                            break;
                        }
                        case 5:  // Voltar e Salvar
                            salvarTurma(&turmaAtual, nomeArquivo);
                            printf("Alteracoes salvas. Voltando ao menu principal...\n");
                            break;
                        default:
                            printf("Opcao invalida. Tente novamente.\n");
                            break;
                    }
                } while (opcaoSubMenu != 5);
                break;

            case 2:  // Sair
                printf("Encerrando o programa. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcaoPrincipal != 2);

    return 0;
}

