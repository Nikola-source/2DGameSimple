#include <main_state.h>
#include <glad/glad.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <rafgl.h>

static int w, h;
static rafgl_raster_t raster;
static rafgl_raster_t raster2;
static rafgl_raster_t raster3;
static rafgl_texture_t tex;

#define RASTER_WIDTH 1280
#define RASTER_HEIGHT 960
static int raster_width = RASTER_WIDTH, raster_height = RASTER_HEIGHT;

#define NUMBER_OF_TILES 2
rafgl_raster_t tiles[NUMBER_OF_TILES];

#define WORLD_H 35
#define WORLD_W 50
int tile_world[WORLD_W][WORLD_H];

#define TILE_SIZE 64

int camx = 0;
int camy = 0;
int hero_world_x = 0;
int hero_world_y = 0;
int hero_screen_x = 0;
int hero_screen_y = 0;

#define MAX_MONSTERS 15
typedef struct {
    int world_x, world_y;
    int speed;
    int active;
    float animation_timer;
    int animation_frame;
} Monster;

Monster monsters[MAX_MONSTERS];
int max_monsters = 0;
int current_monsters = 0;
static rafgl_spritesheet_t monster_sprite;

#define OBJECT_SIZE 128

int object_world_x = 0;
int object_world_y = 0;
int object_active = 0;
int object_respawn = 0;
float object_float_offset = 0.0f;
float object_float_speed = 2.0f;
float object_float_amplitude = 3.0f;
int effect_radius = 10;
int effect_speed = 20;

static rafgl_raster_t object_image;

void spawn_object(void) {

    object_world_x = (rand() % (WORLD_W - 4) + 2) * TILE_SIZE;
    object_world_y = (rand() % (WORLD_H - 4) + 2) * TILE_SIZE;

    object_active = 1;
    object_float_offset = 0.0f;
    object_float_speed = 2.0f;
   // printf("Objekat spawnovan na (%d, %d)\n", object_world_x, object_world_y);
}

typedef struct _particle_t
{
    float x, y, dx, dy;
    int color;
    int life;


} particle_t;

#define MAX_PARTICLES 200

particle_t particles[MAX_PARTICLES];

void draw_particles(rafgl_raster_t *raster)
{
    int i;
    particle_t p;
    int ra;
    for(i = 0; i < MAX_PARTICLES; i++)
    {
        p = particles[i];
        if(p.life <= 0) continue;
        if(p.color == 0){
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy, p.x, p.y, rafgl_RGB(0, 0,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+1, p.x-1, p.y, rafgl_RGB(0, 0,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx+1, p.y - p.dy, p.x, p.y-1, rafgl_RGB(0, 0,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+2, p.x-2, p.y, rafgl_RGB(0, 0,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx+2, p.y - p.dy, p.x, p.y-2, rafgl_RGB(0, 0,  255));


        }else if(p.color == 1){
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy, p.x, p.y, rafgl_RGB(255, 0, 0));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+1, p.x-1, p.y, rafgl_RGB(255, 0,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx+1, p.y - p.dy, p.x, p.y-1, rafgl_RGB(255, 0,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+2, p.x-2, p.y, rafgl_RGB(255, 0,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx+2, p.y - p.dy, p.x, p.y-2, rafgl_RGB(255, 0,  0));

        }else if(p.color == 2){
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy, p.x, p.y, rafgl_RGB(0, 255, 0));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+1, p.x-1, p.y, rafgl_RGB(0, 255,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx+1, p.y - p.dy, p.x, p.y-1, rafgl_RGB(0, 255,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+2, p.x-2, p.y, rafgl_RGB(0, 255,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx+2, p.y - p.dy, p.x, p.y-2, rafgl_RGB(0, 255,  0));

        }else if(p.color == 3){
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy, p.x, p.y, rafgl_RGB(0, 255, 255));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+1, p.x-1, p.y, rafgl_RGB(0, 255,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx+1, p.y - p.dy, p.x, p.y-1, rafgl_RGB(0, 255,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+2, p.x-2, p.y, rafgl_RGB(0, 255,  255));
            rafgl_raster_draw_line(raster, p.x - p.dx+2, p.y - p.dy, p.x, p.y-2, rafgl_RGB(0, 255,  255));

        }else if(p.color == 4){
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy, p.x, p.y, rafgl_RGB(255, 255, 0));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+1, p.x-1, p.y, rafgl_RGB(255, 255,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx+1, p.y - p.dy, p.x, p.y-1, rafgl_RGB(255, 255,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx, p.y - p.dy+2, p.x-2, p.y, rafgl_RGB(255, 255,  0));
            rafgl_raster_draw_line(raster, p.x - p.dx+2, p.y - p.dy, p.x, p.y-2, rafgl_RGB(255, 255,  0));

        }
    }
}

static float elasticity = 0.6;

void update_particles(float delta_time)
{
    int i;
    for(i = 0; i < MAX_PARTICLES; i++)
    {
        if(particles[i].life <= 0) continue;

        particles[i].life--;

        particles[i].x += particles[i].dx;
        particles[i].y += particles[i].dy;
        particles[i].dx *= 0.995f;
        particles[i].dy *= 0.995f;
        particles[i].dy += 0.05;

        if(particles[i].x < 0)
        {
            particles[i].life = 0;
        }

        if(particles[i].y < 0)
        {
            particles[i].life = 0;
        }

        if(particles[i].x >= raster_width)
        {
            particles[i].life = 0;
        }

        if(particles[i].y >= raster_height)
        {
            particles[i].life = 0;
        }
    }
}

void init_tile_map(void){
    int x,y;
    for(y = 0;y< WORLD_H;y++){
        for(x = 0;x<WORLD_W;x++){
            if(y==0 || y==WORLD_H-1 || x==0 || x == WORLD_W-1){
                tile_world[x][y] = 1;
            }else{
                tile_world[x][y] = 0;
            }
        }
    }
}

void render_tile_map2(rafgl_raster_t *raster){

    int start_x = camx / TILE_SIZE;
    int start_y = camy / TILE_SIZE;
    int end_x = start_x + (raster_width / TILE_SIZE) + 2;
    int end_y = start_y + (raster_height / TILE_SIZE) + 2;

    if(start_x < 0) start_x = 0;
    if(start_y < 0) start_y = 0;
    if(end_x > WORLD_W) end_x = WORLD_W;
    if(end_y > WORLD_H) end_y = WORLD_H;

    rafgl_raster_t *draw_tile;
    rafgl_raster_t negative_tile;
    int x,y;
    rafgl_pixel_rgb_t sampled;
    rafgl_pixel_rgb_t result;
    for(y = start_y; y < end_y; y++)
    {
        for(x = start_x; x < end_x; x++)
        {
            draw_tile = tiles + (tile_world[x][y] % NUMBER_OF_TILES);
            rafgl_raster_init(&negative_tile, draw_tile->width, draw_tile->height);
            for(int i = 0;i<draw_tile->width;i++){
                for(int j = 0;j<draw_tile->height;j++){
                    sampled = pixel_at_pm(draw_tile,j,i);
                    result.r = 255-sampled.r;
                    result.g = 255-sampled.g;
                    result.b = 255-sampled.b;
                    result.a = sampled.a;
                    pixel_at_m(negative_tile,j,i) = result;
                }
            }
            rafgl_raster_draw_raster(raster, &negative_tile, x * TILE_SIZE - camx, y * TILE_SIZE - camy - draw_tile->height + TILE_SIZE);
        }
    }
}

void render_tile_map(rafgl_raster_t *raster){

    int start_x = camx / TILE_SIZE;
    int start_y = camy / TILE_SIZE;
    int end_x = start_x + (raster_width / TILE_SIZE) + 2;
    int end_y = start_y + (raster_height / TILE_SIZE) + 2;

    if(start_x < 0) start_x = 0;
    if(start_y < 0) start_y = 0;
    if(end_x > WORLD_W) end_x = WORLD_W;
    if(end_y > WORLD_H) end_y = WORLD_H;

    rafgl_raster_t *draw_tile;
    int x,y;
    for(y = start_y; y < end_y; y++)
    {
        for(x = start_x; x < end_x; x++)
        {
            draw_tile = tiles + (tile_world[x][y] % NUMBER_OF_TILES);
            rafgl_raster_draw_raster(raster, draw_tile, x * TILE_SIZE - camx, y * TILE_SIZE - camy - draw_tile->height + TILE_SIZE);
        }
    }
}
#define HERO_HEALTH 2
static rafgl_spritesheet_t hero;
int hero_health = HERO_HEALTH;

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
    w = width;
    h = height;
    rafgl_raster_init(&raster, w, h);

    int i;

    char tile_path[256];

    for(i = 0; i < NUMBER_OF_TILES; i++)
    {
        sprintf(tile_path, "res/tiles/svgset%dtest.png", i);
        rafgl_raster_load_from_image(&tiles[i], tile_path);
    }


    init_tile_map();
    rafgl_spritesheet_init(&hero, "res/images/character2.png", 4, 4);
    rafgl_texture_init(&tex);

    hero_world_x = (WORLD_W / 2) * TILE_SIZE;
    hero_world_y = (WORLD_H / 2) * TILE_SIZE;

    camx = hero_world_x - raster_width/2;
    camy = hero_world_y - raster_height/2;

    hero_screen_x = hero_world_x - camx;
    hero_screen_y = hero_world_y - camy;

    for(int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].active = 0;
    }
    max_monsters = 1;
    current_monsters = 0;
    for(int i = 0;i<MAX_PARTICLES;i++){
        particles[i].color = rand()%5;
    }

    rafgl_spritesheet_init(&monster_sprite, "res/images/monster.jpg", 5, 4);

    rafgl_raster_load_from_image(&object_image, "res/images/object.jpg");
    spawn_object();

}

void update_camera(void) {
    int max_cam_x = WORLD_W * TILE_SIZE - raster_width;
    int max_cam_y = WORLD_H * TILE_SIZE - raster_height;

    if (camx < 0) camx = 0;
    else if (camx > max_cam_x) camx = max_cam_x;

    if (camy < 0) camy = 0;
    else if (camy > max_cam_y) camy = max_cam_y;
}

void update_hero(void){
    int map_width_pixels = (WORLD_W-1.2) * TILE_SIZE;
    int map_height_pixels = (WORLD_H-1.7) * TILE_SIZE;

    if(hero_world_x < 1.2*TILE_SIZE) hero_world_x = 1.2*TILE_SIZE;
    if(hero_world_x >= map_width_pixels) hero_world_x = map_width_pixels -1;

    if(hero_world_y < TILE_SIZE) hero_world_y = TILE_SIZE;
    if(hero_world_y >= map_height_pixels) hero_world_y = map_height_pixels - 1;
}

//---------------------------------------------------------------------------
void spawn_monster(void) {
    for(int i = 0; i < MAX_MONSTERS; i++) {
        if(!monsters[i].active) {
            monsters[i].active = 1;

            int side = rand() % 4;

            switch(side) {
                case 0:
                    monsters[i].world_x = (rand() % ((WORLD_W-2) * TILE_SIZE))+65 -monster_sprite.frame_width-1;
                    monsters[i].world_y = 65;
                    break;
                case 1:
                    monsters[i].world_x = (WORLD_W-2)* TILE_SIZE+65-monster_sprite.frame_width-1;
                    monsters[i].world_y = (rand() % ((WORLD_H-2) * TILE_SIZE))+65 -monster_sprite.frame_height-1;
                    break;
                case 2:
                    monsters[i].world_x = (rand() % ((WORLD_W-2) * TILE_SIZE))+65 -monster_sprite.frame_width-1;
                    monsters[i].world_y = (WORLD_H-2) * TILE_SIZE+65 -monster_sprite.frame_height-1;
                    break;
                case 3:
                    monsters[i].world_x = 65;
                    monsters[i].world_y = (rand() % ((WORLD_H-2) * TILE_SIZE))+65 -monster_sprite.frame_height-1;
                    break;
            }

            monsters[i].speed = 3;
            monsters[i].animation_timer = 0;
            monsters[i].animation_frame = 0;

            current_monsters++;

            printf("Spawnovan monster %d na (%d, %d)\n", i, monsters[i].world_x, monsters[i].world_y);
            return;
        }
    }
}

int dead = 0;
int dizzy = 0;
int pause = 0;
void update_monsters(void) {

    current_monsters = 0;

    for(int i = 0; i < MAX_MONSTERS; i++) {
        if(!monsters[i].active) continue;

        current_monsters++;

        monsters[i].animation_timer ++;
        if(monsters[i].animation_timer >=5) {
            monsters[i].animation_frame = (monsters[i].animation_frame + 1) % 5;
            monsters[i].animation_timer = 0;
        }
        pause--;
        if(pause<=0 && hero_world_x+hero.frame_width-64>monsters[i].world_x && hero_world_x<monsters[i].world_x+monster_sprite.frame_width-40 &&
            hero_world_y+hero.frame_height-64>monsters[i].world_y && hero_world_y < monsters[i].world_y+monster_sprite.frame_height){
            if(hero_health>0){
                hero_health--;
                dizzy = 60;
                pause = 120;
            }else{
                dead = 1;
                rafgl_raster_copy(&raster3,&raster);
            }
        }

        int dx = abs(hero_world_x - monsters[i].world_x);
        int dy = abs(hero_world_y - monsters[i].world_y);

        int new_x = monsters[i].world_x;
        int new_y = monsters[i].world_y;

        if(dx >= dy) {
            if(hero_world_x > monsters[i].world_x) {
                if(hero_world_x > monsters[i].world_x + monsters[i].speed) {
                    new_x += monsters[i].speed;
                } else {
                    new_x = hero_world_x;
                }
            }
            else if(hero_world_x < monsters[i].world_x) {
                if(hero_world_x < monsters[i].world_x - monsters[i].speed) {
                    new_x -= monsters[i].speed;
                } else {
                    new_x = hero_world_x;
                }
            }
            new_y = monsters[i].world_y;
        }  else {
            if(hero_world_y > monsters[i].world_y) {
                if(hero_world_y > monsters[i].world_y + monsters[i].speed) {
                    new_y += monsters[i].speed;
                } else {
                    new_y = hero_world_y;
                }
            }
            else if(hero_world_y < monsters[i].world_y) {
                if(hero_world_y < monsters[i].world_y - monsters[i].speed) {
                    new_y -= monsters[i].speed;
                } else {
                    new_y = hero_world_y;
                }
            }
            new_x = monsters[i].world_x;
        }

        int can_move = 1;
        for(int j = 0; j < MAX_MONSTERS; j++) {
            if(i == j || !monsters[j].active) continue;

            int old_x = monsters[i].world_x;
            int old_y = monsters[i].world_y;
            monsters[i].world_x = new_x;
            monsters[i].world_y = new_y;

            if(monsters[i].world_x + monster_sprite.frame_width>monsters[j].world_x &&
            monsters[i].world_x<monsters[j].world_x+monster_sprite.frame_width &&
            monsters[i].world_y + monster_sprite.frame_height>monsters[j].world_y &&
            monsters[i].world_y<monsters[j].world_y+monster_sprite.frame_height){
                can_move = 0;
                monsters[i].world_x = old_x;
                monsters[i].world_y = old_y;
                break;
            }
            monsters[i].world_x = old_x;
            monsters[i].world_y = old_y;
        }
        if(can_move) {
            monsters[i].world_x = new_x;
            monsters[i].world_y = new_y;
        }


        if(monsters[i].world_x<64)monsters[i].world_x = 64;
        if(monsters[i].world_x>WORLD_W*TILE_SIZE-130)monsters[i].world_x= WORLD_W*TILE_SIZE-130;
        if(monsters[i].world_y<64)monsters[i].world_y = 64;
        if(monsters[i].world_y>WORLD_H*TILE_SIZE-130)monsters[i].world_y= WORLD_H*TILE_SIZE-130;
    }

    while(current_monsters < max_monsters) {
        spawn_monster();
    }
}
void render_monsters(rafgl_raster_t *raster) {
    for(int i = 0; i < MAX_MONSTERS; i++) {
        if(!monsters[i].active) continue;


        if(monsters[i].world_x>camx-monster_sprite.frame_width &&
         monsters[i].world_x< camx+raster_width &&
         monsters[i].world_y>camy-monster_sprite.frame_height &&
         monsters[i].world_y<camy+raster_height){

            int direction;
            if(hero_world_x>monsters[i].world_x)direction = 2;
            else if(hero_world_x<monsters[i].world_x)direction = 3;
            else if(hero_world_y>monsters[i].world_y)direction = 0;
            else if(hero_world_y<monsters[i].world_y)direction = 1;
            int monster_screen_x = monsters[i].world_x-camx;
            int monster_screen_y = monsters[i].world_y-camy;

            rafgl_raster_draw_spritesheet2(raster, &monster_sprite,
                monsters[i].animation_frame,
                direction,
                monster_screen_x - monster_sprite.frame_width/2,
                monster_screen_y - monster_sprite.frame_height/2);
        }
    }
}
//-------------------------------------------------------------------------------------
#define TIME 20
int timer_frames = 0;
int timer_seconds = TIME;
int timer_running = 1;
int victory = 0;

void update_timer(void){

    if(!timer_running){
        victory = 1;

        return;
    }

    timer_frames++;

    if(timer_frames>=60){
        timer_seconds--;
        timer_frames = 0;

        if(timer_seconds<=0)
        {
            timer_running = 0;
            float xn, yn;
            int x1, y1;
            int box_blur_size = 5;
            int i,j;
            int rc, gc, bc;
            rafgl_raster_copy(&raster2,&raster);
            rafgl_pixel_rgb_t sampled;
            rafgl_pixel_rgb_t result;
            int sample_limit_left = -5, sample_limit_right=5;
            int sample_count = 20;
            for(int y = 0;y<=raster_height;y++){
                yn = 1.0f*y/raster_height;
                for(int x = 0;x<=raster_width;x++){
                    xn = 1.0f*x/w;
                    rc=gc=bc=0;
                    for(i = 0;i<sample_count;i++){
                        int offsetx = rand() % (sample_limit_right - sample_limit_left+1) + sample_limit_left;
                        int offsety = rand() % (sample_limit_right - sample_limit_left+1) + sample_limit_left;

                        sampled = pixel_at_m(raster, rafgl_clampi(x+offsetx, 0 , raster_width-1), rafgl_clampi(y+offsety, 0 , raster_height-1));
                        rc +=sampled.r;
                        gc +=sampled.g;
                        bc +=sampled.b;
                    }
                    rc/=sample_count;
                    gc/=sample_count;
                    bc/=sample_count;

                    result.rgba = rafgl_RGB(rafgl_saturatei(rc),rafgl_saturatei(gc),rafgl_saturatei(bc));
                    pixel_at_m(raster2,x,y) = result;
                }
            }

        }
        max_monsters = ((TIME-timer_seconds)/5)%MAX_MONSTERS;
        //max_monsters = 5;
    }
}

void draw_timer(rafgl_raster_t *raster) {

    char timer_text[32];
    int minutes = timer_seconds / 60;
    int seconds = timer_seconds % 60;

    sprintf(timer_text, "VREME: %02d:%02d", minutes, seconds);

    int timer_x = 20;
    int timer_y = 40;

     int text_width = strlen(timer_text) * 8;
    int text_height = 12;  // Približna visina

   /* for(int y = timer_y - 5; y < timer_y + text_height + 5; y++) {
        for(int x = timer_x - 5; x < timer_x + text_width + 5; x++) {
            if(x >= 0 && x < raster->width && y >= 0 && y < raster->height) {
                pixel_at_pm(raster, x, y).rgba = rafgl_RGB(0, 0, 0);
            }
        }
    }*/

    rafgl_raster_draw_string(raster, timer_text, timer_x, timer_y, rafgl_RGB(249, 0, 0), 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int boost = 0;
int effect_active = 0;
int effect_done = 0;

void update_object(void) {
    if(object_active==0) {
        object_respawn--;
        if(object_respawn <= 0) {
            spawn_object();
        }
        return;
    }

    object_float_offset += 0.05f * object_float_speed;
    if(object_float_offset > 3.14159f * 2.0f) {
        object_float_offset -= 3.14159f * 2.0f;
    }


    if(object_active){
        if(hero_world_x+hero.frame_width-64>object_world_x && hero_world_x<object_world_x+OBJECT_SIZE &&
            hero_world_y+hero.frame_height-64>object_world_y && hero_world_y < object_world_y+OBJECT_SIZE){
                object_active = 0;
                boost = 180;
                object_respawn = 300;
                effect_active = 1;
            }
    }
}

void render_object(rafgl_raster_t *raster) {
    if(object_active==1){
        float current_object_y = object_world_y + sinf(object_float_offset) * object_float_amplitude;
        int obj_screen_x = object_world_x - camx;
        int obj_screen_y = current_object_y - camy;

        if(object_world_x>camx-OBJECT_SIZE && object_world_x< camx+raster_width && object_world_y>camy-OBJECT_SIZE && object_world_y<camy+raster_height){

            rafgl_raster_draw_raster2(raster, &object_image,obj_screen_x,obj_screen_y);
        }
    }
}

void apply_fog_effect() {
    rafgl_pixel_rgb_t sampled, pixel;
    rafgl_pixel_rgb_t fog_color;
    fog_color.r = 180;
    fog_color.g = 180;
    fog_color.b = 200;
    fog_color.a = 255;
    float max_dist = sqrtf(raster_width*raster_width + raster_height*raster_height) / 2;
    for(int y = 0; y < raster_height; y++) {
        for(int x = 0; x < raster_width; x++) {
            sampled = pixel_at_m(raster, x, y);
            pixel = sampled;

            int center_x = hero_screen_x+hero.frame_width/2;
            int center_y = hero_screen_y+hero.frame_height/2;
            float dist = sqrtf((x - center_x)*(x - center_x) +
                              (y - center_y)*(y - center_y));

            float fog_amount = dist / max_dist;
            if(fog_amount > 1.0f) fog_amount = 1.0f;
            if(fog_amount > 1.0f) fog_amount = 1.0f;
            fog_amount = fog_amount;

            pixel = rafgl_lerppix(sampled, fog_color, fog_amount);

            pixel_at_m(raster, x, y) = pixel;

        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////
int distance_squared(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return dx*dx + dy*dy;
}

int pressed;

int animation_running = 0;
int animation_frame = 0;
int direction = 0;
int death_time = 0;
int death_time2 = 0;

int hero_speed = 7;

int hover_frames = 0;

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
    if(victory){
        if(game_data->keys_pressed[RAFGL_KEY_SPACE]){
            victory = 0;
            hero_health = HERO_HEALTH;
            hero_health = 2;
            dizzy = 0;
            timer_running = 1;
            timer_seconds = TIME;
            hero_world_x = (WORLD_W / 2) * TILE_SIZE;
            hero_world_y = (WORLD_H / 2) * TILE_SIZE;
            direction = 0;
            for(int i = 0;i<MAX_MONSTERS;i++){
                if(monsters[i].active)monsters[i].active = 0;
            }
            max_monsters = 0;
        }
        rafgl_raster_draw_raster(&raster,&raster2,0,0);

        int i, gen = 10, radius = 100;
        float angle, speed;

        for(i = 0; (i < MAX_PARTICLES) && gen; i++)
        {
            if(particles[i].life <= 0)
            {
                particles[i].life = 100 * randf() + 100;
                particles[i].x = rand()%raster_width;

                particles[i].y = raster_height;

                angle = (randf() - M_PI/4.0f) * (M_PI/2.0f);
                speed = ( 0.3f + 1 * randf()) * radius;
                particles[i].dx = cosf(angle) * speed;
                particles[i].dy = sinf(angle) * speed;
                gen--;

            }
        }
        update_particles(delta_time);

        draw_particles(&raster);
    }
    else if(dead){
        if(game_data->keys_pressed[RAFGL_KEY_SPACE]){
            dead = 0;
            hero_health = HERO_HEALTH;
            dizzy = 0;
            timer_seconds = TIME;
            hero_world_x = (WORLD_W / 2) * TILE_SIZE;
            hero_world_y = (WORLD_H / 2) * TILE_SIZE;
            direction = 0;
            death_time = 0;
            death_time2 = 0;
            for(int i = 0;i<MAX_MONSTERS;i++){
                if(monsters[i].active)monsters[i].active = 0;
            }
            max_monsters = 0;
        }
        death_time++;
        rafgl_pixel_rgb_t sampled;
        rafgl_pixel_rgb_t result;
        for(int y = 0;y<=raster_height;y++){
            for(int x = 0;x<=raster_width;x++){
                if(distance_squared(x,y,hero_screen_x,hero_screen_y)<death_time*death_time*100){
                    sampled = pixel_at_m(raster3,x,y);
                    int gray = (sampled.r+sampled.g+sampled.b)/3;
                    result.r = gray;
                    result.g = gray;
                    result.b = gray;
                    pixel_at_m(raster3,x,y) = result;
                }
            }
        }
        rafgl_raster_copy(&raster2,&raster);
        rafgl_raster_draw_spritesheet(&raster2, &hero, animation_frame, direction, hero_screen_x-hero.frame_width/2, hero_screen_y-hero.frame_height/2);
        rafgl_raster_draw_spritesheet2(&raster3, &hero, animation_frame, direction, hero_screen_x-hero.frame_width/2, hero_screen_y-hero.frame_height/2);
        for(int y = hero_screen_y-hero.frame_height/2;y<hero_screen_y+hero.frame_height/2;y++){
            for(int x = hero_screen_x-hero.frame_width/2;x<hero_screen_x+hero.frame_width/2;x++){
                sampled = pixel_at_m(raster3,x,y);
                rafgl_pixel_rgb_t sampled2 = pixel_at_m(raster2,x,y);
                if(sampled2.r<250 || sampled2.g<250 || sampled2.b<250 ){
                    sampled.g = 0;
                    sampled.b = 0;
                    pixel_at_m(raster3,x,y) = sampled;
                }
            }
        }
        rafgl_raster_draw_raster(&raster,&raster3,0,0);
    }
    else{
        update_timer();
        update_object();
        if(boost>=0){
            hero_speed = 15;
            boost--;
        }else{
            effect_active = 0;
            effect_done = 0;
            hero_speed = 4;
        }
        animation_running = 0;
        if(game_data->keys_down[RAFGL_KEY_A])
        {
            hero_world_x -= hero_speed;
            direction = 2;
            animation_running = 1;
        }
        if(game_data->keys_down[RAFGL_KEY_D])
        {
            hero_world_x += hero_speed;
            direction = 3;
            animation_running = 1;
        }
        if(game_data->keys_down[RAFGL_KEY_W])
        {
            hero_world_y -= hero_speed;
            direction = 1;
            animation_running = 1;
        }
        if(game_data->keys_down[RAFGL_KEY_S])
        {
            hero_world_y += hero_speed;
            direction = 0;
            animation_running = 1;
        }
        update_hero();
        camx = hero_world_x - raster_width/2;
        camy = hero_world_y - raster_height/2;

        update_camera();
        hero_screen_x = hero_world_x - camx;
        hero_screen_y = hero_world_y - camy;
            update_monsters();
        if(animation_running)
        {
            if(hover_frames == 0)
            {
                animation_frame = (animation_frame + 1) % 4;
                hover_frames = 5;
            }
            else
            {
                hover_frames--;
            }
        }
        if(effect_active){
            render_tile_map2(&raster);
        }else{
            render_tile_map(&raster);
        }

        rafgl_raster_draw_spritesheet2(&raster, &hero, animation_frame, direction, hero_screen_x-hero.frame_width/2, hero_screen_y-hero.frame_height/2);
        //printf("1");
        render_monsters(&raster);

        render_object(&raster);

        if(boost<=0)apply_fog_effect();
        else{
            int x, y;

            rafgl_pixel_rgb_t sampled;

            int x1, y1;
            float alpha, r, rn;
            int cx=hero_screen_x, cy= hero_screen_y;
            float diagonal = sqrt((raster_width/2 * raster_width/2) + (raster_height/2 * raster_height/2));
            rafgl_raster_t temp;
            rafgl_raster_init(&temp, raster_width, raster_height);
            rafgl_raster_copy(&temp, &raster);

            for(y = 0; y < raster_height; y++)
            {
                for(x = 0; x < raster_width; x++)
                {
                    alpha = atan2(y-cy,x-cx);
                    r = rafgl_distance2D(cx,cy, x,y);
                    rn = r/diagonal;
                    rn = 0.4*sqrtf(rn);
                    r = rn*diagonal;
                    x1 = cx + cosf(alpha)*r;
                    y1 = cy + sinf(alpha)*r;
                    sampled = pixel_at_m(temp, rafgl_clampi(x1, 0, raster_width-1), rafgl_clampi(y1, 0, raster_height-1));
                    pixel_at_m(raster, x, y) = sampled;
                }
            }
        }
        if(dizzy>0){
            dizzy--;
            rafgl_pixel_rgb_t sampled;
            int x, y;
            int x1, y1;

            float wave_frequency = 0.03f;   // Manja = duži talasi
            float wave_amplitude = 8.0f;    // Visina talasa
            float wave_speed = 2.0f;
            for(y = 0; y < h; y++)
            {
                for(x = 0; x < w; x++)
                {
                    //x i y pre "+" ce nam definisati oko cega zelimo da osciliramo
                    //koeficijent kojim mnozimo u okviru kosinusne i sinusne funkcije ce uticati na frekvenciju oscilacija
                    //dok ce ono sa cim mnozimo rezultat kosinusa i sinusa predstavljati koliko ce nam biti velika amplituda
                    x1 = x + cos(y*0.04)*20;
                    y1 = y + sin(x*0.04)*20;

                    //vrsimo semplovanje po novim pozicijama
                    sampled = pixel_at_m(raster, rafgl_clampi(x1, 0, raster_width-1), rafgl_clampi(y1, 0, raster_height-1));
                    pixel_at_m(raster, x, y) = sampled;
                }
            }
        }

        char timer_text[32];
        int minutes = timer_seconds / 60;
        int seconds = timer_seconds % 60;

        sprintf(timer_text, "VREME: %02d:%02d", minutes, seconds);

        int timer_x = 20;
        int timer_y = 40;

        int text_width = strlen(timer_text) * 8;
        int text_height = 12;

        rafgl_raster_draw_string(&raster, timer_text, timer_x, timer_y, rafgl_RGB(0, 0, 0), 2);
    }

}

void main_state_render(GLFWwindow *window, void *args)
{
    rafgl_texture_load_from_raster(&tex, &raster);
    rafgl_texture_show(&tex, 0);
}

void main_state_cleanup(GLFWwindow *window, void *args)
{

}
