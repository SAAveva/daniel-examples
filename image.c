#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	Vec2 c1;
	Vec2 c2;
} Mat2;


void vec_print(Vec2 vec) {
	printf("Vec2 { .x = %f, .y = %f }", vec.x, vec.y);
}

void vec_printn(Vec2 vec) {
	printf("Vec2 { .x = %f, .y = %f }\n", vec.x, vec.y);
}

void mat2_print(Mat2 *mat) {
	printf("Mat2 {\n\t");
	vec_print(mat->c1);
	printf(",\n\t");
	vec_print(mat->c2);
	printf("\n}\n");
}

void mat2_printn(Mat2 *mat) {
	mat2_print(mat);
	puts("");
}

void mat2Multiply(Vec2 *result, Mat2 matrix, Vec2 vec) {
	float x = vec.x * matrix.c1.x + vec.y * matrix.c2.x;
	float y = vec.x * matrix.c1.y + vec.y * matrix.c2.y;
	result->x = x;
	result->y = y;
}

uint8_t *gen_pixels_1(unsigned int width, unsigned int height) {
	// generate a red image
	uint8_t *pixels = (uint8_t *)malloc(width * height * 3);
	for (int i = 0;i < height;++i) {
		for (int j = 0;j < width;++j) {
			int index = (i * width + j) * 3;
			pixels[index] = 0xff;
			pixels[index+1] = 0;
			pixels[index+2] = 0;
		}
	}

	return pixels;
}

void _draw_line(uint8_t *image, int width, Vec2 p1, Vec2 p2) {
	Vec2 start, end;
	if (p1.x == p2.x) {
		if (p1.y < p2.y) {
			start	= p1;
			end 	= p2;
		} else {
			start	= p2;
			end		= p1;
		}
	} else {
		if (p1.x < p2.x) {
			start 	= p1;
			end 	= p2;
		}
		else {
			start 	= p2;
			end 	= p1;
		}
	}

	vec_printn(p1);
	vec_printn(p2);
	puts("");
	if (start.x == end.x) {
		int dir_y = start.y <= end.y ? 1:-1;
		for (int y = start.y;y  <= end.y;y++) {
			int index = (y * width + start.x) * 3;
			if (index >= WIDTH * HEIGHT * 3) continue;
			image[index] = 0xff;
			image[index+1] = 0;
			image[index+2] = 0;
		}
	} else {
		float m = (end.y-start.y)/(end.x-start.x);
		float n = end.y - m * end.x;
		for (int x = start.x;x <= end.x;x += 1) {
			float y = m * (float)x + n;
			int index = ((int)roundf(y) * width + x) * 3;
			if (index >= WIDTH * HEIGHT * 3) continue;
			image[index] = 0xff;
			image[index+1] = 0;
			image[index+2] = 0;
		}
	}
}

uint8_t *gen_pixels_2(float width, float height) {
	// generate a square
	uint8_t *image = (uint8_t *)calloc(width * height *  3, 1);

	Vec2 points[] = {
		(Vec2) { .x = .75 * width, .y = .25 * height },
		(Vec2) { .x = .75 * width, .y = .75 * height },
		(Vec2) { .x = .25 * width, .y = .75 * height },
		(Vec2) { .x = .25 * width, .y = .25 * height },
	};

	_draw_line(image, width, points[0], points[1]);
	_draw_line(image, width, points[1], points[2]);
	_draw_line(image, width, points[2], points[3]);
	_draw_line(image, width, points[3], points[0]);

	return image;
}

uint8_t *gen_pixels_3(unsigned int width, unsigned int height) {
	// generate a circle
	uint8_t *image = (uint8_t *)calloc(width * height * 3, 1);
	int r 			= width/3;
	int delta_deg 	= 1;
	int center_x	= width/2;
	int center_y	= height/2;
	
	for (int theta = 0;theta < 360;theta += delta_deg) {
		float theta_rad = theta * M_PI / 180.0;
		int x = r * cos(theta_rad) + center_x;
		int y = r * sin(theta_rad) + center_y;
		int index = (y * width + x) * 3;
		image[index] = 0xff;
		image[index+1] = 0;
		image[index+2] = 0;
	}

	return image;
}

uint8_t *gen_pixels_4(unsigned int width, unsigned int height) {
	// generate a filled circle
	uint8_t *image = (uint8_t *)calloc(width * height * 3, 1);
	int r			= width/3;
	int center_x	= width/2;
	int center_y	= height/2;

	int x,y;
	for (int i = -r;i <= r;++i) {
		for (int j = -r;j <= r;++j) {
			int _r 	= (j * j) + (i * i);
			if (_r < r * r) {
				x = j + center_x;
				y = i + center_y;
				int index = (y * width + x) * 3;
				image[index] = 0xff;
				image[index+1] = 0;
				image[index+2] = 0;
			}
		}
	}
	return image;
}

void image2nc(int width, int height, Vec2 point) {
}

void nc2image(Vec2 point, int width, int height) {
}

uint8_t *updateAnimation(uint8_t *image, unsigned int width, unsigned int height, int time) {
	// generate a simple square rotation animation
	// TODO: Daniel: this doesn't work due to coordinate transformation issue.
	// but i don't have time to fix it. you can try it though!
	// look up normalized device coordinates transformation
	image = (uint8_t *)calloc(width * height * 3, 1);
	
	Vec2 points[] = {
		(Vec2) { .x = .75 * width, .y = .25 * height },
		(Vec2) { .x = .75 * width, .y = .75 * height },
		(Vec2) { .x = .25 * width, .y = .75 * height },
		(Vec2) { .x = .25 * width, .y = .25 * height },
	};

	int speed = 360;
	float angle = (float)(time % speed) * M_PI / 180;

	Mat2 rotation_matrix = (Mat2) { 
		.c1 = (Vec2) { .x = cos(angle), .y = sin(angle)},
		.c2 = (Vec2) { .x = -sin(angle), .y = cos(angle) },
	};

	mat2Multiply(&points[0], rotation_matrix, points[0]);
	mat2Multiply(&points[1], rotation_matrix, points[1]);
	mat2Multiply(&points[2], rotation_matrix, points[2]);
	mat2Multiply(&points[3], rotation_matrix, points[3]);


	_draw_line(image, width, points[0], points[1]);
	_draw_line(image, width, points[1], points[2]);
	_draw_line(image, width, points[2], points[3]);
	_draw_line(image, width, points[3], points[0]);

	return image;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Pixel Buffer Example",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24, 
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

	/* Coose a pixels generating function and see the results */
	uint8_t *pixels = gen_pixels_1(WIDTH, HEIGHT);
	//uint8_t *pixels = gen_pixels_2(WIDTH, HEIGHT);
	//uint8_t *pixels = gen_pixels_3(WIDTH, HEIGHT);
	//uint8_t *pixels = gen_pixels_4(WIDTH, HEIGHT);

    int running = 1;
    SDL_Event event;


	int start_time = SDL_GetTicks();
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT 
			|| event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                running = 0;
            }
        }

		//int elapsed_time = SDL_GetTicks() - start_time;
		//pixels = updateAnimation(pixels, WIDTH, HEIGHT, elapsed_time);

        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * 3);

		free(pixels);
		pixels = NULL;

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    if (pixels != NULL)
		free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
