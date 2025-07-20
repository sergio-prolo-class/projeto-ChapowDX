#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Para a função toupper

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

// REQUISITO: Funções e Ponteiros
// Esta função recebe um ponteiro para um Aluno e altera seus campos de média e situação.
void calcularMediaESituacao(Aluno *aluno)
{
    aluno->media = (aluno->nota1 + aluno->nota2) / 2.0;
    // REQUISITO: Fluxo de decisão
    if (aluno->media >= MEDIA_APROVACAO) {
        strcpy(aluno->situacao, "Aprovado");
    } else {
        strcpy(aluno->situacao, "Reprovado");
    }
}

// Função para formatar o nome do arquivo da turma (ex: "Turma A" -> "Turma_A.txt")
void formatarNomeArquivo(const char *nomeBase, char *nomeFormatado)
{
    strcpy(nomeFormatado, nomeBase);
    // Substitui espaços por underscores para um nome de arquivo válido
    for (int i = 0; nomeFormatado[i] != '\0'; i++) {
        if (isspace(nomeFormatado[i])) {
            nomeFormatado[i] = '_';
        }
    }
    strcat(nomeFormatado, ".txt");
}

// REQUISITO: Manipulação de arquivos (Leitura)
// Tenta carregar os dados de uma turma de um arquivo para a memória.
int carregarTurma(Turma *turma, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");  // Abre para leitura ("r")
    if (arquivo == NULL) {
        return 0;  // Arquivo não existe ou não pôde ser aberto
    }

    fscanf(arquivo, "Turma: %[^\n]\n", turma->nome);
    turma->total_alunos = 0;
    int i = 0;

    // Lê cada aluno do arquivo
    while (i < MAX_ALUNOS && fscanf(arquivo, "ID: %d | Nome: %99[^|]| Nota 1: %f | Nota 2: %f\n",
                                    &turma->alunos[i].id,
                                    turma->alunos[i].nome,
                                    &turma->alunos[i].nota1,
                                    &turma->alunos[i].nota2) == 4) {
        // Remove o espaço extra no final do nome lido
        turma->alunos[i].nome[strcspn(turma->alunos[i].nome, " ")] = '\0';
        calcularMediaESituacao(&turma->alunos[i]);
        i++;
    }

    turma->total_alunos = i;
    fclose(arquivo);
    return 1;  // Sucesso
}

// REQUISITO: Manipulação de arquivos (Gravação)
// Salva os dados da turma da memória para um arquivo de texto.
void salvarTurma(const Turma *turma, const char *nomeArquivo)
{
    // Abre para gravação ("w"), criando o arquivo se não existir
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para gravacao");
        return;
    }

    fprintf(arquivo, "Turma: %s\n", turma->nome);
    fprintf(arquivo, "----------------------------------\n");

    for (int i = 0; i < turma->total_alunos; i++) {
        const Aluno *aluno = &turma->alunos[i];
        // Salva apenas os dados brutos, a média e situação são calculadas na leitura
        fprintf(arquivo, "ID: %d | Nome: %s | Nota 1: %.2f | Nota 2: %.2f\n",
                aluno->id, aluno->nome, aluno->nota1, aluno->nota2);
    }

    fclose(arquivo);
}

// Adiciona um novo aluno à turma.
int adicionarAluno(Turma *turma, Aluno novoAluno)
{
    if (turma->total_alunos >= MAX_ALUNOS) {
        return 0;  // Turma cheia
    }
    // Gera um ID único simples (pode ser melhorado para sistemas mais complexos)
    novoAluno.id = (turma->total_alunos > 0) ? turma->alunos[turma->total_alunos - 1].id + 1 : 1;
    calcularMediaESituacao(&novoAluno);
    turma->alunos[turma->total_alunos] = novoAluno;
    turma->total_alunos++;
    return 1;  // Sucesso
}

// Encontra o índice de um aluno no array pelo seu ID.
int encontrarAlunoPorId(const Turma *turma, int id)
{
    for (int i = 0; i < turma->total_alunos; i++) {
        if (turma->alunos[i].id == id) {
            return i;  // Retorna o índice
        }
    }
    return -1;  // Não encontrado
}

// Atualiza as notas de um aluno.
int atualizarAluno(Turma *turma, int id, float novaNota1, float novaNota2)
{
    int index = encontrarAlunoPorId(turma, id);
    if (index != -1) {
        turma->alunos[index].nota1 = novaNota1;
        turma->alunos[index].nota2 = novaNota2;
        calcularMediaESituacao(&turma->alunos[index]);  // Recalcula a situação
        return 1;                                       // Sucesso
    }
    return 0;  // Aluno não encontrado
}

// Exclui um aluno da turma.
int excluirAluno(Turma *turma, int id)
{
    int index = encontrarAlunoPorId(turma, id);
    if (index != -1) {
        // Desloca todos os elementos posteriores uma posição para a esquerda
        for (int i = index; i < turma->total_alunos - 1; i++) {
            turma->alunos[i] = turma->alunos[i + 1];
        }
        turma->total_alunos--;
        return 1;  // Sucesso
    }
    return 0;  // Aluno não encontrado
}

// --- Funções de Interface ---
// REQUISITO: Interface iterativa via terminal

void exibirMenuPrincipal()
{
    printf("\n===== GERENCIADOR DE TURMAS =====\n");
    printf("1. Selecionar ou Criar uma Turma\n");
    printf("2. Sair\n");
    printf("=================================\n");
}

void exibirSubMenu(const char *nomeTurma)
{
    printf("\n--- Menu da Turma: %s ---\n", nomeTurma);
    printf("1. Adicionar Aluno\n");
    printf("2. Listar Alunos\n");
    printf("3. Atualizar Notas do Aluno\n");
    printf("4. Excluir Aluno\n");
    printf("5. Salvar e Voltar ao Menu Principal\n");
    printf("----------------------------------\n");
}

// Função auxiliar para limpar o buffer do teclado, evitando erros com scanf.
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}