/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 18:01:15 by seonkim           #+#    #+#             */
/*   Updated: 2022/06/02 21:57:02 by seonkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <time.h>

static const double ANGLE_PER_PIXEL = FOV_H / (SCREEN_WIDTH-1.);
static const double FOVH_2 = FOV_H / 2.0;


// random num을 통해 스프라이트 랜덤 렌더링
static int rand_num = 5;

void image_init(t_var *var)
{
	var->image.image = mlx_new_image(var->mlx, SCREEN_WIDTH + var->game.map_width * PIXEL_SIZE + 10, SCREEN_HEIGHT);
	var->image.addr = (int *)mlx_get_data_addr(var->image.image, &var->image.bit_per_pixel,
										&var->image.size_line, &var->image.endian);
}

// x1 부터 x2, y1 부터 y2 까지 선 그리는 함수
void	draw_line(t_image *image, double x1, double y1, double x2, double y2, int color)
{
	double	deltaX;
	double	deltaY;
	double	step;

	deltaX = x2 - x1;
	deltaY = y2 - y1;
	step = (fabs(deltaX) > fabs(deltaY)) ? fabs(deltaX) : fabs(deltaY);
	deltaX /= step;
	deltaY /= step;
	while (fabs(x2 - x1) > 1 || fabs(y2 - y1) > 1)
	{
		image->addr[(int)round(y1) * image->size_line / 4 + (int)round(x1)] = color;
		x1 += deltaX;
		y1 += deltaY;
	}
}

// 0인지 판단하는 함수 (아마 왼각 오른각 구분하려고 사용하는걸로 알음)
int
sgn( double d )
{
    return is_zero(d) ? 0 : ((d > 0) ? 1 : -1);
}

// 기울기 구하는 함수
double
l2dist( double x0, double y0, double x1, double y1 )
{
    double dx = x0 - x1;
    double dy = y0 - y1;
    return sqrt(dx*dx + dy*dy);
}

// 좌표로 맵 내부의 요소 반환하는 함수
int
map_get_cell(t_game *game, int x, int y )
{
    return (x >= 0 && x < game->map_height && y >= 0 && y < game->map_width) ? game->map[x][y] : -1;
}

// ray casting 하는 함수
t_bool
get_wall_intersection(t_var *var, double ray, t_wall *wall, t_sprite *sprite)
{
	double px = var->game.px;
	double py = var->game.py;

    int xstep = sgn( cos(ray) );  /* +1 (right), 0 (no change), -1 (left) */
    int ystep = sgn( sin(ray) );  /* +1 (up),    0 (no change), -1 (down) */

    double xslope = (xstep == 0) ? INFINITY : tan(ray);
    double yslope = (ystep == 0) ? INFINITY : 1./tan(ray);

    double nx = (xstep > 0) ? floor(px)+1 : ((xstep < 0) ? ceil(px)-1 : px);
    double ny = (ystep > 0) ? floor(py)+1 : ((ystep < 0) ? ceil(py)-1 : py);

    // printf("\nray=%.2f deg, xstep=%d, ystep=%d, xslope=%.2f, yslope=%.2f, nx=%.2f, ny=%.2f\n",
        // rad2deg(ray), xstep, ystep, xslope, yslope, nx, ny);

    double f=INFINITY, g=INFINITY;
    t_bool hit = false;
    int hit_side; /* either VERT or HORIZ */

    while( !hit )
    {
        int mapx, mapy;

        if( xstep != 0 ) f = xslope * (nx-px) + py;
        if( ystep != 0 ) g = yslope * (ny-py) + px;

        /* which is nearer to me - VERT(nx,f) or HORIZ(g,ny)? */
        double dist_v = l2dist(px, py, nx, f);
        double dist_h = l2dist(px, py, g, ny);

        if( dist_v < dist_h ) { /* VERT is nearer; go along x-axis */
            mapx = (xstep == 1) ? (int)(nx) : (int)(nx)-1 ;
            mapy = (int) f;
            hit_side = VERT;
            // printf(" V(%d, %.2f) ->", mapx, f);
        }
        else {  /* HORIZ is nearer; go along y-axis */
            mapx = (int) g;
            mapy = (ystep == 1) ? (int)(ny) : (int)(ny)-1 ;
            hit_side = HORIZ;
            // printf(" H(%.2f, %d) ->", g, mapy);
        }
        int cell = map_get_cell(&var->game, mapx, mapy);
        if( cell < 0 ) break;   /* out of map */
		if (cell == CLOSE_DOOR || cell == OPEN_DOOR)
			wall->door = 1;
        if( cell == 1 || cell == CLOSE_DOOR || (cell == SPRITE && sprite->sx == -1)) {   /* hit wall? */
            if( hit_side == VERT ) {
                wall->wdir = (xstep > 0) ? DIR_WEST : DIR_EAST;
			
				wall->wx = nx;
				wall->wy = f;
				if (cell == SPRITE) {
					sprite->sx = nx;
					sprite->sy = f;
					sprite->sdir = wall->wdir;
				}
            }
            else { /* HORIZ */
                wall->wdir = (ystep > 0) ? DIR_SOUTH : DIR_NORTH;
				wall->wx = g;
				wall->wy = ny;
				
				if (cell == SPRITE) {
					sprite->sx = g;
					sprite->sy = ny;
					sprite->sdir = wall->wdir;
				}
			}
			
			if (cell == 1 || cell == CLOSE_DOOR) {
				hit = true;
				// printf(" hit wall!\n");
				break;
			}
        }

        if( hit_side == VERT ) nx += xstep;
        else ny += ystep;
    }
    /* end of while(!hit) */

    return hit;
}

// 보고있는 벽의 좌표를 받아와 벽까지의 길이 구해주는 함수
void	cast_single_ray(t_var *var, int x, t_wall *wall, t_sprite *sprite)
{
    double ray = (var->game.angle + FOVH_2) - (x * ANGLE_PER_PIXEL);

    get_wall_intersection(var, ray, wall, sprite);
        // return INFINITY; /* no intersection - maybe bad map? */

    double wdist = l2dist(var->game.px, var->game.py, wall->wx, wall->wy);
	wall->wdist = wdist * cos(var->game.angle - ray);
	double sdist = l2dist(var->game.px, var->game.py, sprite->sx, sprite->sy);
	sprite->sdist = sdist * cos(var->game.angle - ray);
}

// 세울 벽의 높이를 구해주는 함수
int
get_wall_height( double dist )
{
    double fov_h = 2.0 * dist * tan(FOV_V/2.0);
    return (int)(SCREEN_HEIGHT * (WALL_H / fov_h)); /* in pixels */
}

// 거리별 명암 조절해주는 함수
double get_luminosity(t_var *var, double dist)
{
	static double lum = -1;
	if (lum < 0) lum = (double)(var->game.map_width + var->game.map_height) / 2.0;
	return (dist > lum) ? 0 : (1. - dist / lum);
}

// 200 200 200 int형으로 따로 있는 색을 합쳐주는 함수
int	encode_color(int r, int g, int b)
{
	int color = 0;

	color += r;
	color = (color << 8);
	color += g;
	color = (color << 8);
	color += b;
	return color;
}

// 16진수 color code에서 r, g, b로 분리해주는 함수
void decode_color(int color, int *r, int *g, int *b)
{
	*r = color & 0x00ff0000;
	*r = *r >> 16;
	*g = color & 0x0000ff00;
	*g = *g >> 8;
	*b = color & 0x000000ff;
}

// r, g, b 따로따로 거리를 따라 명암 조절해주는 함수
int fade_color(int color, double lum)
{
	if (lum < 0) lum = 0;
	else if (lum > 1) lum = 1;
	int r, g, b;
	decode_color(color, &r, &g, &b);
	int new_color = encode_color((int)(r * lum), (int)(g * lum), (int)(b * lum));
	return new_color;
}

// 모름이건ㅋ
double get_fov_min_dist()
{
	double T = -1;
	if (T < 0)
		T = WALL_H / (2.0 * tan(FOV_V / 2.0));
	return T;
}

// sprite 그려줌
void	draw_sprite(t_var *var, t_sprite *sprite, int x)
{
	int sh = get_wall_height(sprite->sdist);

    int y0 = (int)((SCREEN_HEIGHT - sh) / 2.0);
    int y1 = y0 + sh - 1;
	double lum = get_luminosity(var, sprite->sdist);

    int ystart = max(0, y0);
    int yend = min(SCREEN_HEIGHT-1, y1);

	for (int y = ystart; y <= yend; y++) {
		int ty = (int)((double)(y - y0) * 64 / sh);
		int color = var->texture[rand_num].texture_image[64 * ty + sprite->stx];
		color = fade_color(color, lum);
		var->image.addr[y * var->image.size_line / 4 + (x + 10 + var->game.map_width * PIXEL_SIZE)] = color;
	}
}

// 벽그려주는 함수
void	draw_wall(t_var *var, t_wall *wall, int x)
{
    int wh = get_wall_height(wall->wdist);

    int y0 = (int)((SCREEN_HEIGHT - wh)/2.0);
    int y1 = y0 + wh - 1;
	double lum = get_luminosity(var, wall->wdist);

    int ystart = max(0, y0);
    int yend = min(SCREEN_HEIGHT-1, y1);

	for (int y = ystart; y <= yend; y++) {
		int ty = (int)((double)(y - y0) * 64 / wh);
		int tex = wall->door ? DIR_DOOR : wall->wdir;
		int color = var->texture[tex].texture_image[64 * ty + wall->wtx];
		color = fade_color(color, lum);
		var->image.addr[y * var->image.size_line / 4 + (x + 10 + var->game.map_width * PIXEL_SIZE)] = color;
	}

	// 천장과 바닥 그려주는 부분
	if (y1 < SCREEN_HEIGHT - 1)
	{
		double ec = get_fov_min_dist();

		for (int y = y1 + 1; y < SCREEN_HEIGHT; y++)
		{
			double h = (double)(SCREEN_HEIGHT - 1 - y) / SCREEN_HEIGHT;
            double D = ec / (1. - 2 * h);
            double lum_f = get_luminosity(var, D) - 0.2;

			var->image.addr[y * var->image.size_line / 4 + (x + 10 + var->game.map_width * PIXEL_SIZE)] = fade_color(var->image.ceilling_color, lum_f);
			var->image.addr[(SCREEN_HEIGHT - 1 -y) * var->image.size_line / 4 + (x + 10 + var->game.map_width * PIXEL_SIZE)] = fade_color(var->image.floor_color, lum_f);
		}
	}
}

/***********************************************************/

// 아마 그리드 그려주는 곳이였는데 현재 사용 안하는듯
void 	draw_lines(t_var *var)
{
	int		i;
	int		j;
	// double k;

	i = 0;
	while (i < var->game.map_width)
	{
		draw_line(&var->image, i * PIXEL_SIZE, 0, i * PIXEL_SIZE, var->game.map_height * PIXEL_SIZE, GRID_COLOR);
		i++;
	}
	draw_line(&var->image, var->game.map_width * PIXEL_SIZE - 1, 0, var->game.map_width * PIXEL_SIZE - 1, var->game.map_height * PIXEL_SIZE, GRID_COLOR);
	j = 0;
	while (j < var->game.map_height)
	{
		draw_line(&var->image, 0, j * PIXEL_SIZE, var->game.map_width * PIXEL_SIZE, j * PIXEL_SIZE, GRID_COLOR);
		j++;
	}
	draw_line(&var->image, 0, var->game.map_height * PIXEL_SIZE - 1, var->game.map_width * PIXEL_SIZE, var->game.map_height * PIXEL_SIZE - 1, GRID_COLOR);
}

// 미니맵 한칸 그려주는 함수
void	draw_rectangle(t_var *var, int x, int y, int color)
{
	int i;
	int j;

	x *= PIXEL_SIZE;
	y *= PIXEL_SIZE;
	i = -1;
	while (++i < PIXEL_SIZE)
	{
		j = -1;
		while (++j < PIXEL_SIZE)
		{
			// if (j == 0 || j == PIXEL_SIZE - 1 || i == 0 || i == PIXEL_SIZE - 1)
			// 	game->img.data[(y + i) * game->img.size_l / 4 + x + j] = 0xb3b3b3;
			// else
			var->image.addr[(y + i) * var->image.size_line / 4 + x + j] = color;
		}
	}
}

// sprite, door등 여러 요소들 미니맵에 표시해주는 함수
void	draw_object(t_var *var)
{
	int i;
	int j;

	i = -1;
	while (++i < var->game.map_height)
	{
		j = -1;
		while (++j < var->game.map_width)
		{
			if (var->game.map[i][j] == SPRITE)
				draw_rectangle(var, j, i, SPRITE_COLOR);
			else if (var->game.map[i][j] == OPEN_DOOR || var->game.map[i][j] == CLOSE_DOOR)
				draw_rectangle(var, j, i, DOOR_COLOR);
		}
	}
}

// 미니맵 그리는 함수
void	draw_rectangles(t_var *var)
{
	int i;
	int j;

	i = -1;
	while (++i < var->game.map_height)
	{
		j = -1;
		while (++j < var->game.map_width)
		{
			if (var->game.map[i][j] == WALL)
				draw_rectangle(var, j, i, WALL_COLOR);
			else if (var->game.map[i][j] == FLOOR)
				draw_rectangle(var, j, i, FLOOR_COLOR);
		}
	}

}

// 위와 동일
void	draw_mini_map(t_var *var)
{
	draw_rectangles(var);
	// draw_lines(var);
	for (int i = 0; i < 10; i++)
		draw_line(&var->image, i + (var->game.map_width * PIXEL_SIZE), 0, i + (var->game.map_width * PIXEL_SIZE), (double)SCREEN_HEIGHT, 0xFFFFFF);
}
/**********************************************************/

// 문 열고 닫는 함수
void handling_door(t_var *var, t_wall *wall)
{
	if (wall->wdist > 1.5 || var->open == 0)
		return ;
		(void)var;
	int mapx = (int)wall->wx;
	int mapy = (int)wall->wy;
	int cell = map_get_cell(&var->game, mapx, mapy);
	if (cell == CLOSE_DOOR)
		var->game.map[mapx][mapy] = OPEN_DOOR;
	else if (cell == OPEN_DOOR)
		var->game.map[mapx][mapy] = CLOSE_DOOR;

	var->open = 0;
}

// 메인 루프
int main_loop(t_var *var)
{
	t_wall		wall[SCREEN_WIDTH];
	t_sprite	sprite[SCREEN_WIDTH];
	clock_t		ms = clock();

	// 이부분이 시간으로 랜덤 난수 생성하는 부분
	if (!(ms % 30)) {
		rand_num = (rand() % 6) + 5;
	}
	
	mlx_destroy_image(var->mlx, var->image.image);
	image_init(var);
	
	// 미니맵 그리는곳 (격자, 칸)
	draw_mini_map(var);
	
	// Ray casting
	for( int x=0; x<SCREEN_WIDTH; x++ ) {
		
		// 동적 할당으로 변경해야 함
		wall[x].door = 0;
		sprite[x].sx = -1;
		sprite[x].sy = -1;
		sprite[x].sdist = -1;
        cast_single_ray(var, x, &wall[x], &sprite[x]);
		handling_door(var, &wall[x]);

		// x쪽을 봐야하는지 y쪽을 봐야하는지 지정해주는 부분
		double text_ratio = (wall[x].wdir == DIR_WEST || wall[x].wdir == DIR_EAST) ? (wall[x].wy - floor(wall[x].wy)) : (wall[x].wx - floor(wall[x].wx));
		// 스프라이트와 벽 텍스쳐의 ray 하나 당 가로길이 지정해 주는 부분
		wall[x].wtx = (int)(text_ratio * var->texture[wall[x].wdir].texture_width);
		double sprite_ratio = (sprite[x].sdir == DIR_WEST || sprite[x].sdir == DIR_EAST) ? (sprite[x].sy - floor(sprite[x].sy)) : (sprite[x].sx - floor(sprite[x].sx));
		sprite[x].stx = (int)(sprite_ratio * 64);

        draw_wall(var, &wall[x], x);

		// sx가 -1인 경우 sprite가 아님
		if (sprite[x].sx != -1)
			draw_sprite(var, &sprite[x], x);

		// 미니맵에 노란 선 그어주는 부분
		draw_line(&var->image, var->game.m_py, var->game.m_px, wall[x].wy * (double)PIXEL_SIZE, wall[x].wx * (double)PIXEL_SIZE, RAY_COLOR);
    }

	// Player position
	double k = -1;
	double l;
	while (++k < var->game.map_height * PIXEL_SIZE)
	{
		l = -1;
		while (++l < var->game.map_width * PIXEL_SIZE)
			if (l >= var->game.m_py - 3 && l <= var->game.m_py + 3 && k >= var->game.m_px - 3 && k <= var->game.m_px + 3)
				var->image.addr[(int)(k * var->image.size_line / 4) + (int)l] = PLAYER_COLOR;
	}

	draw_object(var);

	mlx_put_image_to_window(var->mlx, var->win, var->image.image, 0, 0);
	return (0);
}

// 플레이어 움직임
static int	get_move_offset( double th, int key, double amt, double* pdx, double* pdy )
{
    switch( key ) {
        case KEY_W:
        case KEY_S:
            *pdx = (key==KEY_W ? 1 : -1) * amt * cos(th);
            *pdy = (key==KEY_W ? 1 : -1) * amt * sin(th);
            break;
        case KEY_A:
        case KEY_D:
            *pdx = amt * cos(th + (key==KEY_A ? 1 : -1) * M_PI_2);
            *pdy = amt * sin(th + (key==KEY_A ? 1 : -1) * M_PI_2);
            break;
        default: /* invalid */
            return -1;
    }
    return 0;
}

int	player_move(t_game* game, int key, double amt )
{
    double dx=0, dy=0;
    double nx, ny;

    if( get_move_offset(game->angle, key, amt, &dx, &dy) < 0 ) {
        fprintf(stderr,"player_move: invalid key %d\n", key);
        return -1;
    }
    nx = game->px + dx;
    ny = game->py + dy;

	int cell = map_get_cell(game, (int)nx, (int)ny);
    if(cell != 0 && cell != OPEN_DOOR) {
        printf("** bump !\n");
        return -1;
    }
    game->px = nx;
    game->py = ny;
	game->m_px = game->m_px + (dx * (double)PIXEL_SIZE);
	game->m_py = game->m_py + (dy * (double)PIXEL_SIZE);

    return 0;
}

// 플레이어 회전
void	player_rotate(t_game* pp, double th )
{
    pp->angle += th;
    if( pp->angle < 0 ) pp->angle += _2PI;
    else if( pp->angle > _2PI ) pp->angle -= _2PI;
}

int		deal_key(int key_code, t_var *var)
{
	if(key_code == KEY_ESC ) {   /* quit */
		exit(0);
	}
	if( key_code == KEY_LEFT || key_code == KEY_RIGHT ) {
		player_rotate(&var->game, ROT_UNIT * (key_code ==KEY_LEFT ? 1 : -1));
	}
	else if( key_code == KEY_W || key_code == KEY_A || key_code == KEY_S || key_code == KEY_D ) {
		if( player_move(&var->game, key_code, MOVE_UNIT) == 0 ) {
		}
	}
	else if (key_code == KEY_SPACEBAR)
		var->open = 1;

	return (0);
}

int 	close_window(t_var *var)
{
	(void)var;
		exit(0);
}

void test(char *filename)
{
	t_var	var;

	var.open = 0;
	var.mlx = mlx_init();
	if (parse_cub3d_data(&var, filename))
		return;
	check_valid_map(&var);
	image_init(&var);
	// 미니맵 10 x 10 으로 맞추는 설정 나중에 해야함
	var.win = mlx_new_window(var.mlx, SCREEN_WIDTH + 10 + var.game.map_width * PIXEL_SIZE, SCREEN_HEIGHT, "Cub3D");

	for (int i =0 ; i< var.game.map_height; i++) {
		for (int j = 0; var.game.map[i][j] != '\n' ; j++) {
			if (var.game.map[i][j] == 11)
				printf("%d ", var.game.map[i][j]);
			else
				printf("%d  ", var.game.map[i][j]);
		}
		printf("\n");
	}

	// 나중에 예외처리 할거있어서 안지움
	// int color = 0xdc6400;
	// int r;
	// int g;
	// int b;
	// decode_color(color, &r, &g, &b);
	// printf("%d %d %d %d %d\n", color, r, g, b, encode_color(r, g, b));
	// color = 0xe11e00;
	// decode_color(color, &r, &g, &b);
	// printf("%d %d %d %d %d\n", color, r, g, b, encode_color(r, g, b));
	// printf("[%d, %d] - [%d, %d]\n", var.image.floor_color, 0xdc6400, var.image.ceilling_color, 0xe11e00);
	
	mlx_put_image_to_window(var.mlx, var.win, var.image.image, 0, 0);

	mlx_hook(var.win, X_EVENT_KEY_PRESS, 0, &deal_key, &var);
	mlx_hook(var.win, X_EVENT_KEY_EXIT, 0, &close_window, &var);
	
	mlx_loop_hook(var.mlx, &main_loop, &var);
    mlx_loop(var.mlx);

}

int	main( int ac, char** av)
{
	// ac != 2일 때 종료하게 해야함
	(void)ac;

	test(av[1]);


    return 0;
}