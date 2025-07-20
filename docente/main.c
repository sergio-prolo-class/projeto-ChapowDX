// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "docente.h"

// Interface textual
void exibirMenuPrincipal() {
    printf("\n===== GERENCIADOR DE TURMAS =====\n");
    printf("1. Selecionar ou Criar uma Turma\n");
    printf("2. Sair\n");
    printf("=================================\n");
}

void exibirSubMenu(const char *nomeTurma) {
    printf("\n--- Menu da Turma: %s ---\n", nomeTurma);
    printf("1. Adicionar Aluno\n");
    printf("2. Listar Alunos\n");
    printf("3. Atualizar Notas do Aluno\n");
    printf("4. Excluir Aluno\n");
    printf("5. Salvar e Voltar ao Menu Principal\n");
    printf("----------------------------------\n");
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Turma turmaAtual;
    char nomeTurma[MAX_NOME];
    char nomeArquivo[NOME_ARQUIVO_TURMA_MAX];
    int opcaoPrincipal, opcaoSubMenu, idAluno;

    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcaoPrincipal);
        limparBufferEntrada();

        switch (opcaoPrincipal) {
            case 1:
                printf("\nDigite o nome da turma: ");
                scanf("%99[^\n]", nomeTurma);
                limparBufferEntrada();

                formatarNomeArquivo(nomeTurma, nomeArquivo);

                if (carregarTurma(&turmaAtual, nomeArquivo)) {
                    printf("Turma carregada com sucesso.\n");
                } else {
                    printf("Criando nova turma '%s'.\n", nomeTurma);
                    strcpy(turmaAtual.nome, nomeTurma);
                    turmaAtual.total_alunos = 0;
                }

                do {
                    exibirSubMenu(turmaAtual.nome);
                    printf("Escolha uma opcao: ");
                    scanf("%d", &opcaoSubMenu);
                    limparBufferEntrada();

                    switch (opcaoSubMenu) {
                        case 1: {
                            Aluno novoAluno;
                            if (turmaAtual.total_alunos >= MAX_ALUNOS) {
                                printf("Turma cheia!\n");
                                break;
                            }
                            printf("Nome do aluno: ");
                            scanf("%99[^\n]", novoAluno.nome);
                            limparBufferEntrada();
                            printf("Nota 1: ");
                            scanf("%f", &novoAluno.nota1);
                            limparBufferEntrada();
                            printf("Nota 2: ");
                            scanf("%f", &novoAluno.nota2);
                            limparBufferEntrada();

                            if (adicionarAluno(&turmaAtual, novoAluno)) {
                                printf("Aluno adicionado com sucesso!\n");
                            }
                            break;
                        }
                        case 2:
                            for (int i = 0; i < turmaAtual.total_alunos; i++) {
                                Aluno a = turmaAtual.alunos[i];
                                printf("ID: %d | Nome: %s | Nota1: %.2f | Nota2: %.2f | Media: %.2f | Situacao: %s\n",
                                       a.id, a.nome, a.nota1, a.nota2, a.media, a.situacao);
                            }
                            break;
                        case 3: {
                            printf("ID do aluno para atualizar: ");
                            scanf("%d", &idAluno);
                            limparBufferEntrada();
                            float n1, n2;
                            printf("Nova nota 1: ");
                            scanf("%f", &n1);
                            limparBufferEntrada();
                            printf("Nova nota 2: ");
                            scanf("%f", &n2);
                            limparBufferEntrada();
                            if (atualizarAluno(&turmaAtual, idAluno, n1, n2)) {
                                printf("Notas atualizadas!\n");
                            } else {
                                printf("Aluno nao encontrado.\n");
                            }
                            break;
                        }
                        case 4:
                            printf("ID do aluno para excluir: ");
                            scanf("%d", &idAluno);
                            limparBufferEntrada();
                            if (excluirAluno(&turmaAtual, idAluno)) {
                                printf("Aluno excluido com sucesso.\n");
                            } else {
                                printf("Aluno nao encontrado.\n");
                            }
                            break;
                        case 5:
                            salvarTurma(&turmaAtual, nomeArquivo);
                            printf("Dados salvos. Voltando ao menu principal...\n");
                            break;
                        default:
                            printf("Opcao invalida.\n");
                            break;
                    }

                } while (opcaoSubMenu != 5);
                break;

            case 2:
                printf("Saindo do programa.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcaoPrincipal != 2);

    return 0;
}
