#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

/*Variavéis do jogo */
#define LARGURA_TELA 800
#define ALTURA_TELA  600
#define CHAO_Y       500.0f  
#define LARGURA_P    40.0f    
#define ALTURA_P     60.0f    
#define VEL_ANDAR    5.0f     
#define GRAVIDADE    0.8f     
#define FORCA_PULO  -15.0f  


typedef struct {
    float x;
    float y;
    float velocidade_x;
    float velocidade_y;
} Personagem;

bool aplicar_limites(Personagem* p)
{
    bool no_chao = false;


    if (p->x < 0) {
        p->x = 0;
    }
    if (p->x + LARGURA_P > LARGURA_TELA) {
        p->x = LARGURA_TELA - LARGURA_P;
    }


    if (p->y + ALTURA_P >= CHAO_Y) {
        p->y = CHAO_Y - ALTURA_P;
        p->velocidade_y = 0;
        no_chao = true;
    }

    return no_chao;
}

int main()
{

    ALLEGRO_DISPLAY* janela;
    ALLEGRO_KEYBOARD_STATE teclado;
    Personagem jogador;
    bool rodando;
    bool no_chao;

    al_init();


    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (janela == NULL) {
        printf("Erro ao criar a janela.\n");
        return -1;
    }
    al_set_window_title(janela, "Plataforma Lateral Simples");


    jogador.x = 100.0f;
    jogador.y = CHAO_Y - ALTURA_P;
    jogador.velocidade_x = 0;
    jogador.velocidade_y = 0;
    no_chao = true;
    rodando = true;

    al_install_keyboard();
    al_init_primitives_addon();


    while (rodando) {


        al_get_keyboard_state(&teclado);


        if (al_key_down(&teclado, ALLEGRO_KEY_ESCAPE)) {
            rodando = false;
        }


        jogador.velocidade_x = 0;
        if (al_key_down(&teclado, ALLEGRO_KEY_A) || al_key_down(&teclado, ALLEGRO_KEY_LEFT)) {
            jogador.velocidade_x = -VEL_ANDAR;
        }
        if (al_key_down(&teclado, ALLEGRO_KEY_D) || al_key_down(&teclado, ALLEGRO_KEY_RIGHT)) {
            jogador.velocidade_x = VEL_ANDAR;
        }

        if (no_chao && (al_key_down(&teclado, ALLEGRO_KEY_SPACE) ||
            al_key_down(&teclado, ALLEGRO_KEY_W) ||
            al_key_down(&teclado, ALLEGRO_KEY_UP))) {
            jogador.velocidade_y = FORCA_PULO;
            no_chao = false;
        }
        jogador.velocidade_y = jogador.velocidade_y + GRAVIDADE;


        jogador.x = jogador.x + jogador.velocidade_x;
        jogador.y = jogador.y + jogador.velocidade_y;


        no_chao = aplicar_limites(&jogador);


        al_clear_to_color(al_map_rgb(20, 20, 40));


        al_draw_filled_rectangle(0, CHAO_Y, LARGURA_TELA, ALTURA_TELA,
            al_map_rgb(60, 120, 60));


        al_draw_filled_rectangle(jogador.x, jogador.y,
            jogador.x + LARGURA_P, jogador.y + ALTURA_P,
            al_map_rgb(230, 230, 230));


        al_flip_display();


        al_rest(1.0 / 60.0);
    }


    al_destroy_display(janela);

    return 0;
}