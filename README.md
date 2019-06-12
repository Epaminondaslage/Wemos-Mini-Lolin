# D1-Mini-Lolin
Programas de exemplo de uso da placa D1 Mini Lolin

<img src="https://github.com/Epaminondaslage/D1-Mini-Lolin/blob/master/Figuras/wemos di mini lolin.JPG" height="200" width="280">

## Sumário
* [Informações Gerais](#user-content-informacoes_gerais)
* [Figuras](#Figuras)
* [Drives](#Drives)
* [Status](#status)
* [Contato](#contato)

<h2 id="informacoes_gerais">Informações Gerais</h2>
O WeMos D1 mini Lite Lolin é uma placa de desenvolvimento de microcontroladores sem fio 802.11 (Wifi) em miniatura. Tornou-se
muito popular por possuir um  microcontrolador sem fio ESP8285 em uma placa de desenvolvimento completa. Programar o D1 mini lite é tão simples quanto programar qualquer outro microcontrolador baseado em Arduino, já que o módulo inclui uma interface microUSB embutida permitindo que o módulo seja programado diretamente do Arduino IDE (requer que o suporte do ESP8285 seja adicionado através do gerenciador de placa) sem hardware adicional.

<p><strong>Principais características da placa:</strong></p>

<ul>
<li>Product code: HCWEMO0012</li>
<li>Microcontroller: ESP-8266EX</li>
<li>Operating Voltage: 3.3V</li>
<li>Digital I/O Pins: 11</li>
<li>Analog Input Pins: 1 (Max input: 3.2V)</li>
<li>Clock Speed: 80MHz</li>
<li>Flash: 1M bytes</li>
<li>Length: 34.2mm</li>
<li>Width: 25.6mm</li>
<li>Weight: 10g</li>
</ul>
<p><strong>Pin Function ESP-8266 Pin</strong></p>

<p><br />TX TXD&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; TXD<br />RX RXD&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp; &nbsp; RXD<br />A0 Analog input &nbsp; max 3.3V input A0<br />D0 IO&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; GPIO16<br />D1 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; SCL&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; GPIO5<br />D2 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; SDA&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; GPIO4<br />D3 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 10k Pull-up &nbsp; &nbsp; &nbsp; GPIO0<br />D4 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 10k Pull-up&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; BUILTIN_LED GPIO2<br />D5 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; SCK&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; GPIO14<br />D6 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; MISO &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; GPIO12<br />D7 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; MOSI&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; GPIO13<br />D8 IO,&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 10k Pull-down &nbsp; SS GPIO15<br />G Ground, &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; GND<br />5V&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;&nbsp;&nbsp; 5V <br />3V3&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 3.3V &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 3.3V<br />RST &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Reset&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; RST</p>
<p>&nbsp;</p>
## Figuras



## Drives

<p><span style="font-size: 150%; line-height: 116%;"><strong class="text-strong">Drivers</strong></span></p>
<h4>P&aacute;gina oficial do produto: <a href="https://www.wemos.cc/">https://www.wemos.cc/</a></h4>
<p><span lang="pt" tabindex="0">O Wemos D1 mini pro inclui um CH340 USB para UART IC para permitir que seja programado diretamente a partir da porta USB de um computador. </span></p>
<div class="oSioSc">
<div id="tw-target">
<div id="kAz1tf" class="g9WsWb">
<div id="tw-target-text-container" class="tw-ta-container tw-nfl">
<pre id="tw-target-text" class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">Drivers USB 32/64 bits para computadores Microsoft Windows podem ser baixados da p&aacute;gina do Github:</span></pre>
</div>
</div>
</div>
</div>
<p><br /> <a class="postlink" href="https://github.com/HobbyComponents/CH340-Drivers">https://github.com/HobbyComponents/CH340-Drivers</a><br /> <br /> <br /> <strong>Instala&ccedil;&atilde;o no Windows</strong></p>
<p><span lang="pt" tabindex="0">Drivers est&atilde;o dispon&iacute;veis no link abaixo. Basta baix&aacute;-los e descompact&aacute;-los, <br />conectar seu D1 Mini ao seu computador com um cabo USB e quando o novo<br />hardware for detectado, aponte o assistente para a pasta que voc&ecirc; acabou <br />de descompactar e ele deve instalar sem problemas</span><br /> <br /><strong>Instru&ccedil;&otilde;es para instalar no MA</strong></p>
<ul>
<li id="tw-target-text" class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">Baixe o driver CH34x_Install_V1.3.zip </span></li>
<li class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">Depois de baixado, clique duas vezes no arquivo zip para descompact&aacute;-lo.</span></li>
<li class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">Execute o instalador encontrado nessa pasta.</span></li>
<li class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">Agora reinicie o seu Mac.</span><br /> </li>
</ul>
<div class="oSioSc">
<div id="tw-target">
<div id="kAz1tf" class="g9WsWb">
<div id="tw-target-text-container" class="tw-ta-container tw-nfl">
<pre id="tw-target-text" class="tw-data-text tw-ta tw-text-medium" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0"><strong>Adicionando suporte de placa &agrave; IDE Arduino</strong> </span></pre>
</div>
</div>
</div>
</div>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">As vers&otilde;es mais recentes do Arduino IDE permitem adicionar suporte de placa de terceiros diretamente a partir do IDE do Arduino. Para adicionar suporte, simplesmente abra seu Arduino IDE e siga estas etapas: </span></p>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">1) V&aacute; para Arquivo-&gt; Prefer&ecirc;ncias </span></p>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">2) Na janela que se abre, localize a caixa de texto rotulada 'Additional Boards Manager URLs:' e cole a seguinte linha:</span><br /> <br /> <a class="postlink" href="http://arduino.esp8266.com/stable/package_esp8266com_index.json">http://arduino.esp8266.com/stable/packa ... index.json</a></p>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">Em seguida, clique no bot&atilde;o OK para fechar a janela. </span></p>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">3) Em seguida, v&aacute; para Ferramentas-&gt; Board-&gt; Boards Manager </span></p>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">4) Na janela que &eacute; aberta, procure a entrada intitulada 'esp8266 por ESP8266 Community Version x.x.x' e clique nela. </span></p>
<p class="tw-data-text tw-ta tw-text-small" dir="ltr" style="text-align: left;" data-placeholder="Tradu&ccedil;&atilde;o"><span lang="pt" tabindex="0">5) No canto inferior direito desta entrada voc&ecirc; deve ver agora um bot&atilde;o de instala&ccedil;&atilde;o. Clique nele e o gerenciador de placa ir&aacute; adicionar automaticamente todos os arquivos necess&aacute;rios para suportar o ESP8266. Isso pode levar alguns minutos, dependendo da velocidade da sua conex&atilde;o com a Internet.</span></p>

## Status
Este repositório é para alunos do Curso de Eletrotécnica do CEFET-MG 

## Contato
Criado/adaptado por Epaminondas de Souza  Lage - epaminondaslage@gmail.com ou epaminondaslage@cefetmg.br

