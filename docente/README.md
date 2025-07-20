## ARTHUR DELCASTANHEL DA SILVA 
## MATRíCULA: 202510703607

# 📚 Gerenciador de Turmas em C

Este projeto é um **sistema de gerenciamento de turmas** desenvolvido em linguagem C. Ele permite a criação, visualização, atualização e exclusão de dados de alunos de uma turma, com armazenamento em arquivos de texto.

## 🔧 Funcionalidades

- Criar e selecionar turmas
- Adicionar alunos (nome, nota1, nota2)
- Calcular média e situação (Aprovado/Reprovado)
- Listar todos os alunos da turma
- Atualizar notas de alunos existentes
- Excluir alunos por ID
- Armazenar todos os dados em arquivos `.txt` separados por turma

## 📂 Estrutura do Projeto

O sistema utiliza duas estruturas principais:

- `Aluno`: Contém ID, nome, notas, média e situação.
- `Turma`: Contém o nome da turma e a lista de alunos.

## 💾 Armazenamento em Arquivo

Cada turma é salva em um arquivo `.txt`, cujo nome é gerado automaticamente a partir do nome da turma, substituindo espaços por underlines (`_`).  
Exemplo: a turma `Ciencia da Computacao 2025` será salva como `Ciencia_da_Computacao_2025.txt`.

## ▶️ Como Executar

### Requisitos

- Compilador C (como o `gcc`)

### Compilação

```bash
gcc -o gerenciador_turmas main.c
