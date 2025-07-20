// docente.h
#ifndef DOCENTE_H
#define DOCENTE_H

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

// Funções de lógica (sem interação com o usuário)
void calcularMediaESituacao(Aluno *aluno);
int carregarTurma(Turma *turma, const char *nomeArquivo);
void salvarTurma(const Turma *turma, const char *nomeArquivo);
int adicionarAluno(Turma *turma, Aluno novoAluno);
int encontrarAlunoPorId(const Turma *turma, int id);
int atualizarAluno(Turma *turma, int id, float novaNota1, float novaNota2);
int excluirAluno(Turma *turma, int id);
void formatarNomeArquivo(const char *nomeBase, char *nomeFormatado);

#endif
