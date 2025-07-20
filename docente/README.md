# 📚 Gerenciador de Turmas em C

Este projeto é um **sistema de gerenciamento de turmas** desenvolvido em linguagem C. Ele permite a criação, visualização, atualização e exclusão de dados de alunos de uma turma, com armazenamento em arquivos de texto.

## 🔧 Funcionalidades

- Criar e selecionar turmas
- Cadastrar alunos e seus dados (nome e nota de duas ativadades avaliativas do mesmo)
- Calcular a média e a situação dos alunos (Se o aluno está Aprovado/Reprovado)
- Mostrar para o usuário uma listagem de todos os alunos da turma selecionada
- Editar notas de alunos já cadastrados
- Excluir alunos por ID
- Armazenar todos os dados em arquivos `.txt` separados por turma

## 📂 Estrutura do Projeto

O sistema utiliza duas estruturas principais:

- `Aluno`: Contém ID, nome, notas, média e situação.
- `Turma`: Contém o nome da turma e a lista de alunos.

## 💾 Armazenamento em Arquivo

Cada turma é salva em um arquivo `.txt`, cujo nome é gerado automaticamente a partir do nome da turma, substituindo espaços por underlines (`_`).  
Exemplo: a turma `ProgramaçãoI` será salva como `ProgramaçãoI.txt`.

## ▶️ Como Executar

Para executar e compilar o projeto de um **Compilador C** (como por exemplo o gcc)

### Como Compilar e Executar
1.  **Clone ou baixe o repositório.**
2.  **Abra um terminal** na pasta raiz do projeto
3.  **Compile o arquivo** com o seguinte comando:
    ```bash
    gcc main.c docente.c -o docente
    ```
4.  **Execute o programa:**
    * No Linux ou macOS:
        ```bash
        ./docente
        ```
    * No Windows:
        ```bash
        .\docente.exe
        ```
