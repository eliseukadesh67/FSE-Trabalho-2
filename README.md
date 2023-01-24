# Projeto 2 - Fundamentos de Sistemas Embarcados

## Requisitos

* make
* gcc

## Como executar

Primeiro digite esse comando para compilar o código

```
  make
```

Após o build rode o programa

```
  make run
```

## Objetivo

Este trabalho tem por objetivo a implementação de um sistema (que simula) o controle de um forno para soldagem de placas de circuito impresso (PCBs). Abaixo vemos alguns exemplos de fornos comerciais para este propósito.

## Como utilizar

Assim que o programa inicia ele estará no modo dashboard, ou seja todas as ações serão controladas pelo dashboard.

![Tela inicial](./images/modo-dashboard.jpg "Tela inicial")

Para mudar para o modo terminal, basta clicar no botão no dashboard

![Botão Dashboard](./images/botao-dashboard.jpg "Botão de modo (Dashboard)")

Ao clicar no botão, e ao acessar o modo terminal pela primeira vez, o programa irá perguntar em que modo de temperatura de referência você deseja utilizar.

![Modo Terminal](./images/modo-terminal.jpg "Modo terminal")

Tendo escolhido o modo DEBUG, um menu do modo terminal irá aparecer para mostrar outras opções

![Menu Terminal](./images/menu-modo-terminal.jpeg "Menu terminal")

Nesse menu você poderá alterar o modo de temperatura de referência, selecionar as constantes de PID (Kp, Ki, Kd), ou sair do menu.

Ao selecionar a opção 2, o programa irá perguntar o valor das constantes que deseja utilizar para o cálculo do PID.

![Constantes PID](./images/menu-constantes-pid.jpg "Constantes PID")

Para finalizar o programa em qualquer ponto do funcionamento, basta apertar as teclas CTRL + C.

![Finalizando o sistema](./images/shutdown-system.jpg "Finalizando o sistema")

## Graficos

As medições dos gráficos foram realizadas num intervalo de aproximadamente 5 minutos cada um.

### 1. Gráfico de temperatura fixa

**Potência do Resistor e Ventoinha no tempo**

![Temperatura Fixa (% Resitor e % Ventoinha)](./graficos/curva-temperatura(%25resistor%26%25ventoinha).png "Temperatura Fixa (% Resitor e % Ventoinha)")

### 2. Gráfico de curva de temperatura

**Potência do Resistor e Ventoinha no tempo**

![Curva de temperatura (% Resitor e % Ventoinha)](./graficos/temperatura-referencia-fixa(%25resistor%26%25ventoinha).png "Curva de temperatura (% Resistor e % Ventoinha)")

## Apresentação

A apresentação deste trabalho pode ser acessada através deste link: [Apresentação - Trabalho 2](https://www.youtube.com/watch?v=80PLBp2DEBA)