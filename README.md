# ALUNOS
Bernnardo Seraphim Baptista de Oliveira
Gustavo Amaral Bernardino

# 1. INTRODUÇÃO
Este projeto consiste na implementação de um Ray Tracer básico em C++, desenvolvido para a disciplina de Computação Gráfica. O programa é capaz de carregar uma cena a partir de um arquivo de entrada, interpretar câmeras, luzes, pigmentos, materiais e geometrias (esferas e poliedros), e gerar uma imagem em formato PPM, aplicando os princípios fundamentais de rastreamento de raios: interseção, sombreamento, reflexão e refração.
A versão final foi organizada em múltiplos arquivos, seguindo boa prática de modularização, permitindo fácil manutenção, extensibilidade e clareza estrutural.

# 2. ESTRUTURA GERAL DO PROJETO
A implementação foi dividida em módulos, cada um responsável por uma parte bem definida do sistema:
- vec3.h: Estrutura e operações com vetores 3D
- ray.h: Estrutura de raios (origem + direção)
- constants.h: Constantes globais (EPS, MAX_DEPTH)
- texture.h, texture.cpp: Carregamento e amostragem de texturas PPM
- pigment.h, pigment.cpp: Pigmentos: cores sólidas, checker e texmaps
- material.h: Parâmetros de acabamento (Phong, reflexão, refração)
- light.h: Luzes pontuais e luz ambiente
- camera.h, camera.cpp: Câmera e construção da base ortonormal
- objects.h: Objetos geométricos (Esfera e Poliedro Convexo)
- scene.h: Estrutura geral da cena
- scene_loader.h, scene_loader.cpp: Leitura do arquivo de cena
- raytracer.h, raytracer.cpp: Núcleo do ray tracing (interseções, sombra, shading, reflexão/refração)
- image_io.h, image_io.cpp: Escrita de imagens PPM
- main.cpp: Função principal, laço de renderização

Essa organização reflete o pipeline clássico do Ray Tracing, facilitando futuras extensões, como novos tipos de materiais, objetos ou iluminadores.

# 3. COMPONENTES DA IMPLEMENTAÇÃO

## 3.1. Representação Geométrica
O arquivo vec3.h implementa:
- operações básicas (soma, subtração, produto, divisão por escalar)
- produto escalar e vetorial
- normalização
- cálculo de reflexão especular

O arquivo ray.h define a estrutura de um raio:
- origem O
- direção D

Usado em praticamente todos os módulos

## 3.2. Texturas e Pigmentos
Pigmentos definem a cor de um objeto.

Três tipos são suportados:
- solid: cor constante
- checker: padrão quadriculado 3D
- texmap: aplicação de textura PPM usando coordenadas projetivas

O arquivo texture.cpp implementa leitura dos formatos:
- P3 (ASCII)
- P6 (binário)

Além de interpolação simples via nearest-neighbor.

## 3.3. Materiais / Modelo de Iluminação
- ka: Componente ambiente
- kd: Difusa Lambertiana
- ks: Componente especular
- α: Expoente de Phong
- kr: Coeficiente de reflexão
- kt: Coeficiente de transmissão
- ior: Índice de refração

O Raim Tracer implementa:
- Reflexão especular por reflect()
- Refração via Lei de Snell + vetor transmitido
- Checagem para reflexão interna total

## 3.4. Iluminação E Sombreamento
O sistema suporta:
- Luz ambiente (primeira luz do arquivo)
- Luzes pontuais com atenuação
Para sombras, um pequeno raio secundário é lançado em direção à luz e verifica-se se há obstrução.

## 3.5. Objetos da Cena
Esfera: Interseção resolvida através da equação quadrática.
Poliedro: Implementação usando o método slab, verificando entrada/saída em meia-espaços.
Ambos retornam:
- ponto de interseção
- normal da superfície
- índices de pigmento e finish correspondente

## 3.6. Câmera
A câmera é especificada por:
- posição do olho
- ponto de interesse
- vetor "up"
- campo de visão vertical
O módulo camera.cpp cria uma base ortonormal (u, v, w) para o cálculo dos raios primários.

## 3.7. Loader da Cena
O arquivo de entrada possui o seguinte formato:
1. Camera
2. Luzes
3. Pigmentos
4. Materiais (finish)
5. Objetos (sphere, polyhedron)
O módulo scene_loader.cpp interpreta cada linha e popula a estrutura Scene.

## 3.8. Núcleo do Ray Tracing
O arquivo raytracer.cpp implementa:
1. Busca de interseção mais próxima
- Itera todos os objetos e seleciona o menor t > EPS.
2. Sombreamento (shade)
Combina:
- ambiente
- difusa
- especular
- sombra
- reflexão recursiva
- refração recursiva
O algoritmo funciona da seguinte forma:
```
traceRay(ray, depth):
    encontrar interseção mais próxima
    se não houver: retornar preto
    caso contrário: shade(...)
```
Recursão limitada a MAX_DEPTH = 5.

## 3.9. Geração da Imagem
O framebuffer é salvo em PPM ASCII (P3) para simplicidade.
Formato gerado:
```
P3
WIDTH HEIGHT
255
r g b
r g b
...

```

# 4. PROCESSO DE RENDERIZAÇÃO
O main.cpp realiza:
1. Carregamento da cena
2. Configuração da câmera e resolução
3. Loop duplo sobre os pixels
4. Geração do raio primário para cada pixel
5. Chamada de traceRay()
6. Armazenamento do resultado no framebuffer
7. Escrita do arquivo PPM final
O programa imprime o progresso linha a linha.

# 5. INSTRUÇÕES DE COMPILAÇÃO E EXECUÇÃO (WINDOWS)
## 5.1. Compilação
A partir da pasta base do projeto:
```
g++ -std=c++17 -O2 src/*.cpp -o raytracer
```

## 5.2. Execução
A partir da pasta base do projeto
```
./raytracer entrada.in saida.ppm [largura] [altura]
```
