# 1. INTRODUÇÃO
Este projeto consiste na implementação de um Ray Tracer básico em C++, desenvolvido para a disciplina de Computação Gráfica. O programa é capaz de carregar uma cena a partir de um arquivo de entrada, interpretar câmeras, luzes, pigmentos, materiais e geometrias (esferas e poliedros), e gerar uma imagem em formato PPM, aplicando os princípios fundamentais de rastreamento de raios: interseção, sombreamento, reflexão e refração.
A versão final foi organizada em múltiplos arquivos, seguindo boa prática de modularização, permitindo fácil manutenção, extensibilidade e clareza estrutural.

2. ESTRUTURA GERAL DO PROJETO
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

3. COMPONENTES DA IMPLEMENTAÇÃO
3.1. Representação Geométrica
O arquivo vec3.h implementa:
- operações básicas (soma, subtração, produto, divisão por escalar)
- produto escalar e vetorial
- normalização
- cálculo de reflexão especular

O arquivo ray.h define a estrutura de um raio:
- origem O
- direção D

Usado em praticamente todos os módulos

3.2. TEXTURAS E PIGMENTOS
Pigmentos definem a cor de um objeto.

Três tipos são suportados:
- solid: cor constante
- checker: padrão quadriculado 3D
- texmap: aplicação de textura PPM usando coordenadas projetivas

O arquivo texture.cpp implementa leitura dos formatos:
- P3 (ASCII)
- P6 (binário)

Além de interpolação simples via nearest-neighbor.

3.3. MATERIAIS / MODELO DE ILUMINAÇÃO
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

3.4. ILUMINAÇÃO E sombreamento
O sistema suporta:
- Luz ambiente (primeira luz do arquivo)
- Luzes pontuais com atenuação: