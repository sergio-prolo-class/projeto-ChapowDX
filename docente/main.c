#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // importado para o funcionamento da função isspace

// pré-definições para componentes do código
#define MAX_ALUNOS 50
#define MAX_NOME 100
#define MAX_NOME_ARQUIVO 110
#define MEDIA_APROVACAO 7.0
#define FREQUENCIA_MINIMA 75.0  
// novo requisito para aprovação de um aluno: 75% de frequência

// estrutura que contém todos os dados que vão ser cadastradas do aluno
typedef struct {
    int id;
    char nome[MAX_NOME];
    float nota1;
    float nota2;
    float frequencia;  // (adicionada frequência)
    float media;
    char situacao[20];  // "Aprovado" ou "Reprovado"
} Aluno;

// estrutura para representar a turma
typedef struct {
    char nome[MAX_NOME];
    Aluno alunos[MAX_ALUNOS];
    int total_alunos;
    int proximo_id;  // garante que o ID seja sempre único
} Turma;


// essas funções criadas para serem desenvolvidas no main()
void calcularMediaESituacao(Aluno *aluno);
void gerarNomeArquivo(const char *nomeBase, char *nomeArquivoFormatado);
int carregarTurma(Turma *turma, const char *nomeArquivo);
void salvarTurma(const Turma *turma, const char *nomeArquivo);
void adicionarAluno(Turma *turma, Aluno novoAluno);
int encontrarIndiceAlunoPorId(const Turma *turma, int id);
int atualizarAluno(Turma *turma, int id, float novaNota1, float novaNota2, float novaFrequencia);
int excluirAluno(Turma *turma, int id);

// essas são funções com a relação a interface com o usuário
void exibirMenuPrincipal();
void exibirMenuTurma(const char *nomeTurma);
void lerString(char *buffer, int tamanho);
void gerenciarMenuTurma(Turma *turma, const char *nomeArquivo);

// desenvolvimento do código
int main()
{
    int opcao;
    char nomeTurma[MAX_NOME];
    char nomeArquivo[MAX_NOME_ARQUIVO];

    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // ler o caractere da entrada padrão

        switch (opcao) {
            case 1: {
                Turma turmaAtual;

                printf("\nDigite o nome da turma (ex: Prog I): ");
                lerString(nomeTurma, MAX_NOME);

                gerarNomeArquivo(nomeTurma, nomeArquivo);

                if (carregarTurma(&turmaAtual, nomeArquivo)) {
                    printf("\nTurma '%s' carregada com sucesso.\n", turmaAtual.nome);
                } else {
                    printf("\nTurma nao encontrada. Criando nova turma '%s'...\n", nomeTurma);
                    strcpy(turmaAtual.nome, nomeTurma);
                    turmaAtual.total_alunos = 0; // após criar a turma, o código considera como a turma vazia e zerá o ID do 'alunos'
                    turmaAtual.proximo_id = 1; // depois de zerar o id, o código define um ID a partir do primeiro aluno cadastrado
                }

                gerenciarMenuTurma(&turmaAtual, nomeArquivo);
                break;
            }
            case 2:
                printf("Encerrando o programa. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 2);

    return 0;
}

// função responsável para verificar a aprovação de um aluno cadastrado
void calcularMediaESituacao(Aluno *aluno)
{
    aluno->media = (aluno->nota1 + aluno->nota2) / 2.0;
    // a condição de aprovação agora depende da média e da FREQUÊNCIA (como pedido em sala de aula)
    if (aluno->media >= MEDIA_APROVACAO && aluno->frequencia >= FREQUENCIA_MINIMA) {
        strcpy(aluno->situacao, "Aprovado");
    } else {
        strcpy(aluno->situacao, "Reprovado");
    }
}

// função responsável pela criação do arquivo .txt
void gerarNomeArquivo(const char *nomeBase, char *nomeArquivoFormatado)
{
    strcpy(nomeArquivoFormatado, nomeBase); // copia o nome original da turma para replicar no nome do arquivo
    for (int i = 0; nomeArquivoFormatado[i] != '\0'; i++) {
        if (isspace(nomeArquivoFormatado[i])) { // isspace vai verificar se o caractere é um espaço em branco. ex:(PROG 1)
            nomeArquivoFormatado[i] = '_'; // o código formatado, transformará o espaço em branco em '_'. ex:(PROG 1) será (PROG_1)
        }
    }
    strcat(nomeArquivoFormatado, ".txt"); // o strcat é responsável por adicionar o '.txt' no final do arquivo
}

// salva todos os dados da turma dentro do arquivo, agora salvando também a frequência
int carregarTurma(Turma *turma, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r"); // chama fopen para abrir o arquivo no modo de leitura com 'r' = read
    if (arquivo == NULL) { // caso fopen não consiga abrir o arquivo, ele retorna NULO sinalizando que o arquivo não existe ou não pode ser aberto
        return 0;
    }

    fscanf(arquivo, "Turma: %[^\n]\n", turma->nome); // usei %[^\n]\n para ler todo o nome da turma até que encontrar uma nova linha

    char buffer[256]; // criei um buffer com 256 bytes pra salvar cada linha lida do arquivo.
    fgets(buffer, sizeof(buffer), arquivo);  // Lê a primeira linha do arquivo e armazena no buffer

    turma->total_alunos = 0;
    int max_id = 0;

    while (turma->total_alunos < MAX_ALUNOS && fgets(buffer, sizeof(buffer), arquivo) != NULL) { // função fgets permite ler uma nova linha do arquivo com sucesso(!= NULL) e quando fgets chega ao final do arquivo, fgets retorna NULL e o laço para.
        Aluno *aluno = &turma->alunos[turma->total_alunos];

        // O sscanf agora também busca o valor da frequência no arquivo, lendo todos os dados da string do buffer criado
        // usei %*f na média pra poder ler e descartar a média salva, porque ela será recalculada para garantir consistência.
        if (sscanf(buffer, "ID: %d | Nome: %[^|]| N1: %f | N2: %f | Media: %*f | Frequencia: %f%%", // %% em frequência, para fazer o código reconhece % como caractere
                   &aluno->id, aluno->nome, &aluno->nota1, &aluno->nota2, &aluno->frequencia) == 5) { // retorna 5 indicando o número de itens que sscanf conseguiu ler sem falhas
            
// código para limpar o nome do aluno
// pega a quantidade de caracteres do nome com size_t
size_t comprimento = strlen(aluno->nome);

// antes vai verificar se a string não está vazia para evitar erros.
if (comprimento > 0) {
    // vai pegar o último caractere da string.
    // a posição é 'comprimento - 1' porque os índices começam em 0.
    char ultimo_caractere = aluno->nome[comprimento - 1];

    // vai verificar se o último caractere é um espaço em branco.
    if (isspace(ultimo_caractere)) {
        //se for, vai substituir o espaço pelo caractere nulo '\0' pra que termine a string uma posição antes, removendo o espaço.
        aluno->nome[comprimento - 1] = '\0';
    }
}

            calcularMediaESituacao(aluno);  // recalcula média e situação para garantir que estejam atualizadas
            if (aluno->id > max_id) {
                max_id = aluno->id;
            }
            turma->total_alunos++;
        }
    }

    turma->proximo_id = max_id + 1;
    fclose(arquivo);
    return 1;
}

//função para salvar os dados da turma em arquivo, agora incluindo média e frequência.
void salvarTurma(const Turma *turma, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w"); // vai abrir o arquivo, usando "w" para criar o arquivo se ele não existe 
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para gravacao");
        return;
    }

    fprintf(arquivo, "Turma: %s\n", turma->nome);
    fprintf(arquivo, "--------------------------------------------------------------------------\n");

    for (int i = 0; i < turma->total_alunos; i++) {
        const Aluno *aluno = &turma->alunos[i];
        // agora inclui a média e a frequência na formatação como pedido pelo professor
        fprintf(arquivo, "ID: %d | Nome: %s | N1: %.2f | N2: %.2f | Media: %.2f | Frequencia: %.1f%%\n",
                aluno->id, aluno->nome, aluno->nota1, aluno->nota2, aluno->media, aluno->frequencia);
    }

    fclose(arquivo);
}

void adicionarAluno(Turma *turma, Aluno novoAluno)
{
    if (turma->total_alunos >= MAX_ALUNOS) {
        printf("Erro: A turma esta cheia!\n");
        return;
    }

    novoAluno.id = turma->proximo_id++;
    calcularMediaESituacao(&novoAluno);
    turma->alunos[turma->total_alunos] = novoAluno;
    turma->total_alunos++;
    printf("Aluno adicionado com sucesso com o ID %d!\n", novoAluno.id);
}

int encontrarIndiceAlunoPorId(const Turma *turma, int id)
{
    for (int i = 0; i < turma->total_alunos; i++) { 
        if (turma->alunos[i].id == id) { // 'turma->alunos[i].id' vai acessar o campo e a posição do aluno atual pelo id, (== id) pra comparar o id do atual com o que foi passado na função
            return i;
        }
    }
    return -1; // retorna -1 para indicar que ocorreu um erro (Não foi encontrado o ID ou algum erro relacionado)
}

int atualizarAluno(Turma *turma, int id, float novaNota1, float novaNota2, float novaFrequencia)
{
    int indice = encontrarIndiceAlunoPorId(turma, id);
    if (indice != -1) {
        turma->alunos[indice].nota1 = novaNota1;
        turma->alunos[indice].nota2 = novaNota2;
        turma->alunos[indice].frequencia = novaFrequencia;
        calcularMediaESituacao(&turma->alunos[indice]);
        return 1;
    }
    return 0;
}

int excluirAluno(Turma *turma, int id)
{
    int indice = encontrarIndiceAlunoPorId(turma, id);
    if (indice != -1) {
        for (int i = indice; i < turma->total_alunos - 1; i++) { // começa na posição do aluno removido e vai até o penúltimo aluno da lista
            turma->alunos[i] = turma->alunos[i + 1]; // dentro do for, o aluno vai ser sobrescrito pelo aluno na próxima posição
        }
        turma->total_alunos--;
        return 1; // sucesso
    }
    return 0; // se o indice for igual a -1, a função termina sinalizando o erro com o return 0
}

// a seguir são as funções que vão gerar a interface para o usuário

void exibirMenuPrincipal()
{
    printf("\n===== DOCENTE ACADÊMICO =====\n");
    printf("1. Selecionar ou Criar uma Turma\n");
    printf("2. Sair\n");
    printf("=================================\n");
}

void exibirMenuTurma(const char *nomeTurma)
{
    printf("\n--- Menu da Turma: %s ---\n", nomeTurma);
    printf("1. Adicionar Aluno\n");
    printf("2. Listar Alunos\n");
    printf("3. Atualizar Dados do Aluno\n");  
    printf("4. Excluir Aluno\n");
    printf("5. Salvar e Voltar ao Menu Principal\n");
    printf("----------------------------------\n");
}

void lerString(char *buffer, int tamanho) // char *buffer vai ser o ponteiro para onde o texto lido foi armazenado
{
    fgets(buffer, tamanho, stdin); // ler a string verificando o tamanho e input da string
    buffer[strcspn(buffer, "\n")] = '\0'; // foi usado 'strcspn(buffer, "\n")' para buscar na string o primeiro caractere qie se encontra em uma nova linha, retornando a posiçãp desse caractere
}
// função para a interação do usuário com a interface criada
void gerenciarMenuTurma(Turma *turma, const char *nomeArquivo)
{
    int opcao;
    do {
        exibirMenuTurma(turma->nome);
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: {  // Adicionar Aluno
                Aluno novoAluno;
                printf("Digite o nome do aluno: ");
                lerString(novoAluno.nome, MAX_NOME);
                printf("Digite a primeira nota: ");
                scanf("%f", &novoAluno.nota1);
                printf("Digite a segunda nota: ");
                scanf("%f", &novoAluno.nota2);
                printf("Digite a frequencia do aluno (em %%): ");  // Solicita  a frequência do aluno no cadastro
                scanf("%f", &novoAluno.frequencia);
                getchar();
                adicionarAluno(turma, novoAluno);
                break;
            }
            case 2: {  // Listar Alunos
                printf("\n--- Alunos da Turma: %s ---\n", turma->nome);
                if (turma->total_alunos == 0) {
                    printf("Nenhum aluno cadastrado nesta turma.\n");
                } else {
                    for (int i = 0; i < turma->total_alunos; i++) {
                        Aluno a = turma->alunos[i];
                        // vai exibir também a frequência
                        printf("ID: %d | Nome: %s | N1: %.2f | N2: %.2f | Freq: %.1f%% | Media: %.2f | Situacao: %s\n",
                               a.id, a.nome, a.nota1, a.nota2, a.frequencia, a.media, a.situacao);
                    }
                }
                printf("--------------------------------------------------------------------------------------\n");
                break;
            }
            case 3: {  // Atualizar Aluno
                int id;
                float n1, n2, freq;
                printf("Digite o ID do aluno para atualizar: ");
                scanf("%d", &id);
                printf("Digite a nova primeira nota: ");
                scanf("%f", &n1);
                printf("Digite a nova segunda nota: ");
                scanf("%f", &n2);
                printf("Digite a nova frequencia (em %%): ");  
                scanf("%f", &freq);
                getchar();
                if (atualizarAluno(turma, id, n1, n2, freq)) {
                    printf("Dados do aluno atualizados com sucesso!\n");
                } else {
                    printf("Erro: Aluno com ID %d nao encontrado.\n", id);
                }
                break;
            }
            case 4: {  // Excluir Aluno
                int id;
                printf("Digite o ID do aluno que deseja excluir: ");
                scanf("%d", &id);
                getchar();
                if (excluirAluno(turma, id)) {
                    printf("Aluno excluido com sucesso!\n");
                } else {
                    printf("Erro: Aluno com ID %d nao encontrado.\n", id);
                }
                break;
            }
            case 5:
                salvarTurma(turma, nomeArquivo);
                printf("Alteracoes salvas. Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}
