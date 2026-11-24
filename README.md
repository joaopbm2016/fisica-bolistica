# Física Bolística

Simulação de bolas em C++ usando [raylib](https://www.raylib.com/).

Este guia explica como compilar e executar o projeto em Linux sem instalar o
raylib globalmente e sem usar `sudo`. O raylib será compilado dentro da sua
pasta pessoal.

## Requisitos

O computador precisa ter:

- `g++` (compilador C++)
- `make`
- Dependências de desenvolvimento para gráficos e janelas do Linux, como
  OpenGL e X11

Confira se `g++` e `make` estão disponíveis:

```bash
g++ --version
make --version
```

Se algum comando não for encontrado, ou se faltarem headers/bibliotecas do
Linux, será necessário pedir a instalação das dependências ao administrador.
Baixar o código do raylib manualmente não substitui essas dependências.

## 1. Baixar os arquivos

1. No GitHub deste projeto, clique em **Code > Download ZIP** e extraia o
   arquivo.
2. Na [página oficial de releases do raylib](https://github.com/raysan5/raylib/releases),
   baixe o ZIP do código-fonte de uma versão estável e extraia-o também.

Os comandos abaixo usam como exemplo estas pastas:

- Projeto: `~/Downloads/fisicaBolistica`
- raylib: `~/Downloads/raylib-5.5`

Se os nomes ou locais forem diferentes, ajuste os caminhos nos comandos.

## 2. Compilar o raylib localmente

Abra um terminal e entre na pasta `src` do raylib:

```bash
cd "$HOME/Downloads/raylib-5.5/src"
make PLATFORM=PLATFORM_DESKTOP
```

Esse comando cria a biblioteca estática `libraylib.a` dentro da pasta `src`.
Não execute `make install`: a biblioteca será usada diretamente desse local.

## 3. Compilar o projeto

Entre na pasta do projeto:

```bash
cd "$HOME/Downloads/fisicaBolistica"
```

Compile o executável:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
  -I"$HOME/Downloads/raylib-5.5/src" \
  main.c++ "$HOME/Downloads/raylib-5.5/src/libraylib.a" \
  -lGL -lm -lpthread -ldl -lrt \
  -lX11 -lXrandr -lXi -lXcursor -lXinerama \
  -o wind
```

O arquivo executável `wind` será criado na pasta do projeto.

## 4. Executar

Ainda na pasta do projeto, rode:

```bash
./wind
```

## Problemas comuns

- **`g++` ou `make`: comando não encontrado** — falta o compilador ou a
  ferramenta de build. Peça ao administrador para instalá-los.
- **Header como `X11/Xlib.h`, `GL/gl.h` ou `alsa/asoundlib.h` não encontrado**
  — faltam dependências de desenvolvimento do sistema. Peça ao administrador
  que instale as dependências apropriadas para a distribuição Linux.
- **`cannot find -lGL`, `-lX11` ou outra biblioteca** — falta uma biblioteca
  de desenvolvimento ou o respectivo linker não consegue encontrá-la.
- **O caminho `raylib-5.5` não existe** — substitua-o pelo nome e local da
  pasta que foi extraída do ZIP do raylib.

Consulte também o guia oficial de compilação do
[raylib para GNU/Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).
