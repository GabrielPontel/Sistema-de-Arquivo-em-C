# SISTEMA DE ARQUIVOS
A princípio, para o desenvolvimento do código que simula um sistema de arquivos a partir de arquivos (binários e textos) deve-se compreender o contexto de como esses sistemas são estruturados na prática. Dessa forma, com base nos conhecimentos adquiridos nas aulas de Sistemas Operacionais, realizadas no Instituto Federal de Educação, Ciência e Tecnologia de São Paulo (IFSP) em 2025, e com o apoio do livro Sistemas Operacionais Modernos, de Andrew S. Tanenbaum, foi possível elaborar o sistema de arquivos inspirado no EXT3.
Nesse sentido, com o conhecimento teórico do sistema EXT3 foi codificado diversas estruturas semelhantes a dele, sendo elas:
- Entrada de diretório: estrutura responsável por armazenar informações prévias de um arquivo dentro do diretório onde está alocado,sendo assim, armazena somente o nome e o número inode (identificador do arquivo) dele;
- Inode: estrutura que armazena todas as informações de um arquivo no EXT3. Desse modo, ela contém número, nome, tipo, quantidade de blocos ocupados, tamanho e um vetor para armazenar o número de cada bloco que o arquivo ocupa;
- Gerenciador de blocos: para o gerenciamento dos blocos do sistema, foi utilizado um mapa de bits, por conta de seu tamanho fixo e baixo custo de armazenamento, utilizando apenas um bit para representar cada bloco (0 para livre e 1 para ocupado). Para a implementação dessa estrutura, foi necessário o uso de conceitos da biblioteca bitwise.;
- Gerenciador de inode livre: Para facilitar a alocação de um inode foi implementado uma lista encadeada, na qual cada estrutura de inode armazena o um inteiro representando o próximo inode livre sendo que o primeiro inode livre é armazenado no campo next do arquivo superblock.dat;
- Estrutura de diretórios: para organizar as entradas dentro de um diretório, foi utilizada uma árvore binária de busca, a qual possibilita uma alocação eficiente e facilita a pesquisa de arquivos. A navegação na árvore ocorre por meio de comparações lexicográficas entre strings: caso o nome procurado seja lexicograficamente maior que o nó atual, a busca segue para a subárvore da direita; caso contrário, para a subárvore da esquerda.

## _Arquivos_:
- structs.h: Centraliza as estruturas básicas do sistema, como structs.
- superblock.h: Define a estrutura do superbloco, responsável por armazenar informações gerais do sistema de arquivos, como tamanho total, quantidade de blocos, quantidade de inodes e metadados globais.
- inode.h: Contém a definição e operações relacionadas ao inode, que armazena os metadados de cada arquivo (tipo, tamanho, blocos ocupados, identificador, etc.).
- blocks.h: Responsável pelo gerenciamento dos blocos de dados, incluindo alocação, liberação e controle via mapa de bits, indicando quais blocos estão livres ou ocupados.
- freespace.h: Gerencia os inodes livres, utilizando uma estrutura (lista encadeada) que facilita a alocação e reutilização de inodes disponíveis no sistema.
- dir_structure.h: Define a estrutura interna dos diretórios, como nós da árvore binária, utilizada para organizar e armazenar as entradas de diretório.
- dir.h: Implementa as operações sobre diretórios, como inserção, busca e remoção de entradas, utilizando a estrutura definida em dir_structure.h.
- file.h: Contém funções relacionadas à manipulação de arquivos, como criação, leitura, escrita e remoção, fazendo uso de inodes e blocos.
- commands.h: Define os comandos disponíveis para o usuário, funcionando como uma camada de interface entre a entrada do usuário e as funções internas do sistema de arquivos.
- util.h: Agrupa funções auxiliares e utilitárias, como validações, manipulação de strings, conversões e rotinas de apoio usadas em diferentes partes do sistema.

## _Linguagem utilizada_:
<div>
  <img align-"center" alt="C" src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white">
</div>

## _Desenvolvedor_:
<div>
<a href="https://github.com/GabrielPontel" target="_blank">
 <img src="https://github.com/GabrielPontel.png" width="150" alt="Foto do perfil GitHub">
</a>
</div>



  
