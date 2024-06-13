//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       _              //
//               _    _       _      _        _     _   _   _    _   _   _        _   _  _   _          //
//           |  | |  |_| |\| |_| |\ |_|   |\ |_|   |_| |_| | |  |   |_| |_| |\/| |_| |  |_| | |   /|    //    
//         |_|  |_|  |\  | | | | |/ | |   |/ | |   |   |\  |_|  |_| |\  | | |  | | | |_ | | |_|   _|_   //
//                                                                                       /              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   Programa básico para controle da placa durante a Jornada da Programação 1
*   Permite o controle das entradas e saídas digitais, entradas analógicas, display LCD e teclado. 
*   Cada biblioteca pode ser consultada na respectiva pasta em componentes
*   Existem algumas imagens e outros documentos na pasta Recursos
*   O código principal pode ser escrito a partir da linha 86
*/

// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC
#include "MP_hcf.h"   // Controles do ADC

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------

#define LIGA_R1        saidas|0b00000001
#define DESLIGA_R1     saidas&0b11111110

#define LIGA_R2        saidas|0b00000010
#define DESLIGA_R2     saidas&0b11111101

#define LIGA_T1        saidas|0b00000100
#define DESLIGA_T1     saidas&0b11111011

#define LIGA_T2        saidas|0b00001000 
#define DESLIGA_T2     saidas|0b11110111

#define LIGA_TRANS1    saidas|0b00010000
#define DESLIGA_TRANS1 saidas&0b11101111

#define LIGA_TRANS2    saidas|0b00100000
#define DESLIGA_TRANS2 saidas&0b11011111

#define LIGA_TRANS3    saidas|0b01000000
#define DESLIGA_TRANS3 saidas&0b10111111

#define LIGA_TRANS4    saidas|0b10000000
#define DESLIGA_TRANS4 saidas&0b01111111

#define DESLIGA_GERAL saidas = (0b00000000)

#define TECLA_1 '1'==le_teclado()
#define TECLA_2 '2'==le_teclado()
#define TECLA_3 '3'==le_teclado()
#define TECLA_4 '4'==le_teclado()
#define TECLA_5 '5'==le_teclado()
#define TECLA_6 '6'==le_teclado()
#define TECLA_7 '7'==le_teclado()
#define TECLA_8 '8'==le_teclado()
#define TECLA_9 '9'==le_teclado()
#define TECLA_0 '0'==le_teclado()

#define TECLA_DIVISAO '/'==le_teclado()
#define TECLA_MULTIPLICACAO 'x'==le_teclado()
#define TECLA_SUBTRACAO '-'==le_teclado()
#define TECLA_SOMA '+'==le_teclado()
#define TECLA_IGUAL '='==le_teclado()
#define TECLA_CANCELA 'C'==le_teclado()

#define FIM_CURSO (entradas>>7)&1

// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------

static const char *TAG = "Placa";
static uint8_t entradas, saidas = 0; //variáveis de controle de entradas e saídas

int controle = 0;
int tempo = 10;
char tecla;
int digito = 0;
char SENHA [4];
char exibir [40];
int seq = 0;

// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------


// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal


    // a seguir, apenas informações de console, aquelas notas verdes no início da execução
    ESP_LOGI(TAG, "Iniciando...");
    ESP_LOGI(TAG, "Versão do IDF: %s", esp_get_idf_version());

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)
    
    // inicializar os IOs e teclado da placa
    ioinit();      
    entradas = io_le_escreve(saidas); // Limpa as saídas e lê o estado das entradas

    // inicializar o display LCD 
    lcd595_init();
    lcd595_write(1,1,"    Jornada 1   ");
    lcd595_write(2,1,"Programa Basico");
    
    // Inicializar o componente de leitura de entrada analógica
    esp_err_t init_result = hcf_adc_iniciar();
    if (init_result != ESP_OK) {
        ESP_LOGE("MAIN", "Erro ao inicializar o componente ADC personalizado");
    }

    //delay inicial
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    lcd595_clear();

   
    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados

    DRV_init(1, 2);
           lcd595_write(1,1,"Digite senha");
           lcd595_write(2,1,"[ ] [ ] [ ] [ ]");
    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {                                                                                                                                 
        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //
        
           tecla = le_teclado();
           entradas = io_le_escreve(saidas);

           uint32_t valor;
           
           hcf_adc_ler(&valor);                                                                                                                                                     
           //sprintf(exibir, "%" PRIu32 "", valor);
           //lcd595_write(1, 0, exibir)

           if (TECLA_DIVISAO)  
           rotacionar_DRV (1,10,saidas);

           if (TECLA_MULTIPLICACAO)
           rotacionar_DRV (0,10,saidas);



            if (tecla >= '0' && tecla <= '9')
            {
                if(controle == 0)
                {
                    digito = digito * 10 + tecla - '0';
                    seq++;

                    if(seq == 1)
                    {
                        lcd595_write(2,1,"[*] [ ] [ ] [ ]");
                    }

                    
                    if(seq == 2)
                    {
                        lcd595_write(2,1,"[*] [*] [ ] [ ]");
                    }

                    
                    if(seq == 3)
                    {
                        lcd595_write(2,1,"[*] [*] [*] [ ]");
                    }

                    
                    if(seq == 4)
                    {
                        lcd595_write(2,1,"[*] [*] [*] [*]");
                    }
                  
                }
            }

            if (seq == 4)
            {
                if( digito == 1710)  
                {
                    seq++;
                }
                    if (seq == 5)
                    {
                        while(valor <= 1900)
                        {
                        rotacionar_DRV(1, 10, saidas);
                        lcd595_clear();
                        lcd595_write(1,1, "COFRE ABRINDO...");  
                        }

                        seq++;  
                    }
                        if (seq == 6)
                        {
                            if (valor > 1900)
                            {
                            lcd595_clear();
                            sprintf(exibir, "%d", tempo);
                            lcd595_write(1,1, exibir);
                            tempo--;
                            }
                        }
                            if (tempo == 0)
                            {
                                tempo = 10;
                                seq++;                                                   
                            }                            
            }

                    if (seq == 7)
                    {
                        if (valor >= 380) 
                        {
                            rotacionar_DRV(0,10, saidas);
                            seq = 0;
                        }
                    }
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        
        
        vTaskDelay(200 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    
            }            
    // caso erro no programa, desliga o módulo ADC
    //hcf_adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
    

}
