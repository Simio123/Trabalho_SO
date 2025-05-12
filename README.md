# Trabalho de Sistemas Operacionais - Sincronização de Processos

**Curso:** Bacharelado em Sistemas de Informação  
**Disciplina:** DCT2101 – Sistemas Operacionais  
**Professor:** João Borges  

**Componentes:**  
EMERSON DA SILVA SANTOS
JOAO PEDRO DOS SANTOS MEDEIROS

## Descrição
Este repositório contém a implementação de três problemas clássicos de sincronização em sistemas operacionais, desenvolvidos em linguagem C como parte de um trabalho acadêmico da disciplina DCT2101. O projeto utiliza semáforos POSIX e é dividido em três módulos principais:

- **[Buffer](./Buffer/README.md)**: Implementação do problema do Buffer Limitado (Produtor-Consumidor), abordando sincronização entre processos que compartilham um buffer.
- **[Filosofos](./Filosofos/README.md)**: Solução para o problema do Jantar dos Filósofos, focado em evitar deadlocks e starvation em sistemas concorrentes.
- **[Leitores](./Leitores/README.md)**: Implementação do problema dos Leitores-Escritores, gerenciando acesso concorrente a um recurso compartilhado.

Cada módulo contém o código-fonte (`*.c`), o código de resposta (`*.resp.c`) e um `README.md` com detalhes específicos de implementação e execução.

## Pré-requisitos
- Um compilador C (ex.: `gcc`)
- Sistema operacional compatível com threads POSIX (ex.: Linux ou macOS)
- Bibliotecas padrão de C (como `pthread` para concorrência)