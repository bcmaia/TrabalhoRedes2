# Projeto de Cliente e Servidor IRC

Este é o projeto da disciplina SSC0142 - Redes de Computadores, ministrada pela Prof. Kalinka R. L. J. Castelo Branco. O objetivo do projeto é implementar um cliente e servidor IRC (Internet Relay Chat), seguindo as especificações do RFC 1459, que define o protocolo IRC.

## Integrantes do Grupo

- Bernardo Maia Coelho (12542481)
- Pedro Nascimento (12803492)
- Gustavo Wadas Lopes (12745640)

## Estrutura do Projeto

Cada modulo foi implementado em uma pasta separada. Vale destacar que as funcionalidades do módulo 2 usam o socket do modulo 1.
O projeto está organizado nos seguintes arquivos e diretórios:

- `module_1/socket.hpp` e `module_1/socket.cpp`: Implementação da classe Socket, que fornece funcionalidades para comunicação entre processos utilizando sockets TCP. Essa classe é utilizada pelos módulos 1 e 2 do projeto.
- `module_1/client.cpp` e `module_1/server.cpp`: Códigos-fonte para o cliente e servidor do módulo 1, respectivamente. O cliente estabelece uma conexão com o servidor e envia uma mensagem, enquanto o servidor recebe a mensagem do cliente. O servidor e cliente do
 módulo 1 são simples e servem apenas para demonstrar o funcionamento do socket.
- `module_2/client.cpp` e `module_2/server.cpp`: Códigos-fonte para o cliente e servidor do módulo 2, respectivamente. O cliente estabelece uma conexão com o servidor e permite a troca de mensagens entre vários clientes conectados ao servidor.
- `module_2/utils.cpp` e `module_2/utils.hpp`: Contém uma função útil na comparação de strings.
- `module_3/client.cpp` e `module_3/server.cpp`: Códigos-fonte para o cliente e servidor do módulo 3, respectivamente. O cliente estabelece uma conexão com o servidor e envia comandos e mensagens ao servidor. O servidor armazena os canais, quais clientes estão em quais canais e em quais canais cada cliente está banido ou mutado, assim como admnistra os comandos, pesquisas por nomes e para quais clientes as mensagens em cada canal são enviadas. 
- `module_1/Makefile` e `module_2/Makefile`, `module_3/Makefile`: Os Makefiles de cada módulo. Contém targets `make all`, `make clean`, `make mk-client`, `make mk-server`, `make run-server-3` e `make run-client-3`.

## Ambiente e pacotes utilizados
O código foi compilado e testado em uma máquina de 64bits em um sistema 
operacional linux (`pop-os 22.04 LTS`, uma distribuição baseada em `ubuntu`).
Para compilar e testar esse projeto, os seguintes pacotes são necessários: 

- `g++`: O compilador G++ foi utilizado para compilar os códigos em linguagem C++. 
 Para o instalar, execute:
```
sudo apt install build-essential
```

- `make`: O utilitário Make foi utilizado para automatizar o processo de compilação 
 e execução dos códigos. O arquivo Makefile fornecido no projeto contém as regras
 necessárias para compilar e executar os arquivos. Elas são simples, mas úteis.
 Para o instalar, execute:
 
```
sudo apt install make
```

- `git`: O sistema de controle de versão Git foi utilizado para gerenciar o código-fonte do projeto. Ademais, ele será utilizado para obter o código do projeto. Para o instalar:
```
sudo apt install git
```


## Compilação e Execução

## Setup
Primeiro garanta que os pacotes necessários estão instalados.
Para clonar o projeto em sua máquina, abra um terminal, navegue para um diretório
 que ache adequado e execute:
```
git clone https://github.com/bcmaia/TrabalhoRedes2
```

### Módulo 1
Para compilar o cliente e servidor do módulo 1, execute o seguinte comando no terminal:

```bash
cd module_1
make all
```

Isso irá gerar os executáveis `client` e `server`. Para executar o servidor, utilize o seguinte comando:

```bash
./server
```

Em outra janela do terminal, execute o cliente com o comando:

```bash
./client
```

O cliente irá se conectar ao servidor e enviar uma mensagem, que será exibida pelo servidor.

### Módulo 2

Para compilar o cliente e servidor do módulo 2, execute o seguinte comando no terminal:

```bash
cd module_2
make all
```

Isso irá gerar os executáveis `client` e `server`. Primeiro, execute o servidor com o comando:

```bash
./server
```

Em seguida, execute o cliente em outra janela do terminal:

```bash
./client
```

O cliente irá se conectar ao servidor e permitirá a troca de mensagens entre vários clientes conectados ao servidor.

#### Uso e comandos do módulo 2
O módulo 2 oferece no cliente comandos para gerenciar a troca de mensagens:
- `/connect`: Conecta o cliente ao servidor.
- `/quit`: Desconecta e fecha o cliente.
- `/ping`: executa um ping (requer que o servidor responda 'pong').
  
Ao abrir o cliente, o comando `/connect` deve ser executado primeiro.
Em seguida, qualquer linha de texto digitada pelo usuário no cliente, 
 que não contenha comandos, será enviada para o servidor.

### Módulo 3

Para compilar o cliente e servidor do módulo 3, execute o seguinte comando no terminal:

```bash
cd module_3
make all
```

Isso irá gerar os executáveis `client3` e `server3`. Primeiro, execute o servidor com o comando:

```bash
make run-server-3
```

Em seguida, execute clientes em outras janelas do terminal:

```bash
make run-client-3
```

Os clientes iram se conectar ao servidor que irá administrar os comandos e mensagens enviados a ele.

#### Uso e comandos do módulo 3
No módulo 3, o servidor lida com os seguintes comandos normais:
- `/join CHANNELNAME`: Verifica se existe um canal com o nome CHANNELNAME. Se sim, verifica se o canal não está cheio e executa a entrada do cliente no canal (e saída do canal em que está). Se não, verifica se é possível criar mais um canal e se sim, cria o canal e coloca o cliente que criou ele como admnistrador (tirando ele do canal em que estava antes). Caso o admnistrador saia do canal, o canal é fechado.
- `/nickname NICKNAME`: Altera o nickname do cliente para NICKNAME caso tenha menos de 50 caracteres.
- `/ping`: envia de volta ao cliente um pong.
- `/quit`: remove o cliente do canal que estive e desconecta o cliente do servidor

Além dos seguintes comandos de admnistrador:
- `/kick NICKNAME`: bane usuário do canal, não permitindo entrada futura e removendo ele se estiver no canal.
- `/mute NICKNAME`: deixa o usuário mudo no canal, mesmo que ele não esteja no canal para evitar de sair e voltar ao canal remover o efeito.
- `unmute NICKNAME`: deixa o usuário não mudo no canal.
- `Whois NICKNAME`: retorna o IP do usuário NICKNAME.
  
Ao abrir o cliente, o comando `/connect` deve ser executado primeiro.
Em seguida, qualquer linha de texto digitada pelo usuário no cliente, 
que não contenha comandos será enviada como para o servidor como
mensagem, mas só serão re-enviadas caso o cliente esteja num canal.

## Vídeos
Demonstrações do funcionamento de cada módulo, junto com uma breve explicação do código, estão disponíveis nos seguintes likes:


- [**Módulo 1**](https://youtu.be/QhWooBvRnfU)

- [**Módulo 2**](https://youtu.be/Jn79BAaS2Rc)

- [**Módulo 3**](https://youtu.be/CRXyPqnBBAU)

