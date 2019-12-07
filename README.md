# Tom-and-Jerry
Projeto para disciplina Algoritmos Evolutivos - 2019.2

### Features
As entidades do "jogo" (o gato e o rato), podem se alimentar, e se reproduzir.     
Em cada passo do jogo, os gatos verificam em um raio ao seu redor (smell_range) se existe um rato, para que ele se alimente, ou se existe um parceiro com quem possa reproduzir. Caso exista um parceiro que possua energia suficiente, assim como o proprio rato, eles tentam se aproximar e reproduzir.     
Já os ratos, em cada passo, verificam em um raio ao seu redor procurando por 3 coisas: queijos, gatos ou outros ratos. Os queijos são os alimentos dos ratos, para que possam aumentar a sua energia. Os gatos são os predadores dos ratos, então, quando um rato vê um gato, ele tenta correr para longe de seu predador. Eles também procuram por ratos para tentar se reproduzir, respeitando as mesmas condições dos gatos para a realização da reprodução.     
Os queijos possuem uma chance de serem gerados a cada iteração do programa, o que faz com que os ratos não acabem sempre morrendo sem comida.

### Algoritmo Genético
O objetivo do nosso AG é de gerar um ambiente equilibrado entre gatos, ratos e queijos, que não tenha dominio de uma espécie.     
Ao se reproduzirem, os animais acabam sofrendo mutações durante cada "geração" do programa.      
Porém, o AG de fato está nos parâmetros de inicialização de cada geração. Para simular uma pseudoaleatoriedade que é fixa para cada geração, antes de inicializar as populações de uma geração, mudamos a seed do gerador aleatório para a mesma (que é um define no programa, a RAND_SEED). A partir disso, sabemos que a única coisa que diferencia uma geração de outra são os parâmetros iniciais, o que geramos com o AG.


* **Mas o que são esses parâmetros?**     
Os parametros que são gerados pelo AG são os valores de inicialização da população de gatos e de ratos. Esses parâmetros são: velocidade (cat_speed & mouse_speed), o range de visualização de cada animal (cat_smell_range & mouse_smell_range), além da limiar de energia necessária para a reprodução entre duas entidades da mesma espécie (cat_reprodution_limiar & mouse_reproduction_limiar).
Esses parâmetros sempre são iniciados com os seguintes valores:    
    * **Gatos**
        * **cat_speed:** 1.0f
        * **cat_smell_range:** 5.0f
        * **cat_reprodution_limiar:** 0.6f
    * **Ratos**
        * **mouse_speed:** 1.0f
        * **mouse_smell_range:** 5.0f
        * **mouse_reprodution_limiar:** 0.6f

    Outro fator importante de mencionar são as variáveis min_speed e min_smell_range, que controlam quais os menores valores durante a mutação, para que não haja "roubo" do AG.

* **Mas como é medida a fitness de uma geração?**     
Atualmente, a melhor maneira que encontramos de calcular a fitness de uma geração foi utilizar o tempo de duração dela. Quanto mais tempo uma geração dura, mais equilibrada ela deve estar. Então sempre que uma geração acaba (ou seja, sempre que uma das populações seja menor ou igual a 1) é calculado o fitness dela. Caso esse valor seja melhor que a melhor geração, ela passa a ser a melhor geração, e nela são feitas mutações, e iniciado novamente o processo.


## Código
O codigo que controla o algoritmo genético está todo localizado no namespace "GameManager" (no arquivo GameManager.cpp). As funções mais essenciais que estão presentes nele são: makeStep, makeNewGen, genFood, mutateGen, genEnded e calculateWeight. Essas são as funcões que controlam o AG.  

* **makeNewGen()**     
A função makeNewGen é chamada sempre que uma geração acaba. Nela são comparadas a melhor geração e a geração atual, para verificar se houve melhora. Nela são iniciados os parametros para a nova geração.

* **makeStep()**     
Essa função é chamada STEPS_PER_RENDER vezes por iteração do programa (esse é um define feito no programa principal, tanto na versão GL quando na versão noGL). Ou seja, ela é o coração de uma geração. Nela são realizados os movimentos, nela são calculadas a perda de energia e morte por fome. Ela é quem faz o "jogo" funcionar.

* **genFood()**     
Essa função é chamada uma vez por iteração e tem uma chance de gerar queijos para que os ratos não morram de fome. A variável food_spawn_difficulty define qual a dificuldade de um queijo ser gerado por iteração (quanto maior, mais difícil).

* **genEnded()**     
É uma função que define quando uma geração acaba. Atualmente ela acaba sempre que exista apenas um gato ou apenas um rato. Ela deverá retornar true quando a geração tiver acabado e false caso contrário.

* **mutateGen()**     
É chamada sempre antes de uma geração se iniciar e realiza uma mutação nos parâmetros de inicialização da população, para tentar achar parâmetros que gerem uma geração melhor.

* **calculateWeight()**     
É uma função que calcula o peso da mutação, aumentando sempre que não houver melhora em uma geração.

* **outras...**     
Algumas outras funções são importantes também para o programa, como por exemplo a função energyConsume() da classe Entity, que atualmente pune as entidades que possuem pouca velocidade, para evitar que o AG roube, colocando velocidade baixa para aumentar a quantidade de iterações.

### Execução
Após explicar brevemente o funcionamento do programa, partimos para a execução do mesmo. Existem duas formas de executar o programa: com o openGL e sem o mesmo.

* **Compilação noGL**     
Essa execução é um pouco mais rápida, pois não tem o limitante do framerate causado pela renderiazação.
Para gerar o arquivo executável basta rodar o seguinte comando:

```console
$ make noGL
```

* **Compilação GL**     
Para compilar essa versão é necessário que o usuário possua a biblioteca GLUT instalada em seu computador.

    * **Linux**     
    Para instalar a lib no linux, basta rodar os seguintes comandos:
    ```console
    $ sudo apt-get update
    $ sudo apt-get install libgl1-mesa-dev
    $ sudo apt-get install freeglut3-dev
    $ sudo apt-get install mesa-utils
    $ sudo apt-get install libxmu-dev libxi-dev
    ```

    Então, basta rodar o seguinte comando:
```console
$ make GL
```

* **Execução**     
Para executar o programa, basta rodar o comando, após ter compilado uma das versões do programa:

```
$ make run
```