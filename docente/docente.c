#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "docente.h"

void calcularMediaESituacao(Aluno *aluno) {
    aluno->media = (aluno->nota1 + aluno->nota2) / 2.0;
    if (aluno->media >= MEDIA_APROVACAO) {
        strcpy(aluno->situacao, "Aprovado");
    } else {
        strcpy(aluno->situacao, "Reprovado");
    }
}

void formatarNomeArquivo(const char *nomeBase, char *nomeFormatado) {
    strcpy(nomeFormatado, nomeBase);
    for (int i = 0; nomeFormatado[i] != '\0'; i++) {
        if (isspace(nomeFormatado[i])) {
            nomeFormatado[i] = '_';
        }
    }
    strcat(nomeFormatado, ".txt");
}

int carregarTurma(Turma *turma, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) return 0;

    fscanf(arquivo, "Turma: %[^\n]\n", turma->nome);
    turma->total_alunos = 0;
    int i = 0;

    while (i < MAX_ALUNOS && fscanf(arquivo, "ID: %d | Nome: %99[^|]| Nota 1: %f | Nota 2: %f\n",
        &turma->alunos[i].id, turma->alunos[i].nome,
        &turma->alunos[i].nota1, &turma->alunos[i].nota2) == 4) {
        
        turma->alunos[i].nome[strcspn(turma->alunos[i].nome, " ")] = '\0';
        calcularMediaESituacao(&turma->alunos[i]);
        i++;
    }

    turma->total_alunos = i;
    fclose(arquivo);
    return 1;
}

void salvarTurma(const Turma *turma, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para gravacao");
        return;
    }

    fprintf(arquivo, "Turma: %s\n", turma->nome);
    fprintf(arquivo, "----------------------------------\n");

    for (int i = 0; i < turma->total_alunos; i++) {
        const Aluno *aluno = &turma->alunos[i];
        fprintf(arquivo, "ID: %d | Nome: %s | Nota 1: %.2f | Nota 2: %.2f\n",
                aluno->id, aluno->nome, aluno->nota1, aluno->nota2);
    }

    fclose(arquivo);
}

int adicionarAluno(Turma *turma, Aluno novoAluno) {
    if (turma->total_alunos >= MAX_ALUNOS) return 0;
    novoAluno.id = (turma->total_alunos > 0) ? turma->alunos[turma->total_alunos - 1].id + 1 : 1;
    calcularMediaESituacao(&novoAluno);
    turma->alunos[turma->total_alunos++] = novoAluno;
    return 1;
}

int encontrarAlunoPorId(const Turma *turma, int id) {
    for (int i = 0; i < turma->total_alunos; i++) {
        if (turma->alunos[i].id == id) return i;
    }
    return -1;
}

int atualizarAluno(Turma *turma, int id, float novaNota1, float novaNota2) {
    int index = encontrarAlunoPorId(turma, id);
    if (index != -1) {
        turma->alunos[index].nota1 = novaNota1;
        turma->alunos[index].nota2 = novaNota2;
        calcularMediaESituacao(&turma->alunos[index]);
        return 1;
    }
    return 0;
}

int excluirAluno(Turma *turma, int id) {
    int index = encontrarAlunoPorId(turma, id);
    if (index != -1) {
        for (int i = index; i < turma->total_alunos - 1; i++) {
            turma->alunos[i] = turma->alunos[i + 1];
        }
        turma->total_alunos--;
        return 1;
    }
    return 0;
}
