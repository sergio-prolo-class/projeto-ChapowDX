#include <stdio.h>     // Para printf, scanf, fopen, fclose, fprintf, fscanf, getchar, etc.
#include <string.h>    // Para strcpy, strcat, strcspn
#include <ctype.h>     // Para isspace


// Funções e Ponteiros
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
