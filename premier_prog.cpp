#define GL_GLEXT_PROTOTYPES

// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"
#include "particle_system.h"

using namespace std;


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 10;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Drawable* formlist[MAX_FORMS_NUMBER]);

// Renders scene to the screen
const void render(Drawable* formlist[MAX_FORMS_NUMBER], const Point &cam_pos);

// Frees media and shuts down SDL
void close(SDL_Window** window);

Vector scene_angle = Vector(0, 0, 0);
Vector scene_translation = Vector(0, 0, 0);
Vector scene_zoom = Vector(0.07, 0.07, 0.07);

Shader* test;


/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		// Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		// Create window
		*window = SDL_CreateWindow("TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (*window == NULL)
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			// Create context
			*context = SDL_GL_CreateContext(*window);
			if (*context == NULL)
			{
				cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				// Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
				}

				// Initialize OpenGL
				if (!initGL())
				{
					cout << "Unable to initialize OpenGL!" << endl;
					success = false;
				}
				else 
				{
					if (glewInit() != GLEW_OK)
					{
						cout << "Unable to initialize Glew!" << endl;
						success = false;
					}
				}
			}
		}
	}

	return success;
}


bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	// Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport : use all the window to display the rendered scene
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Fix aspect ratio and depth clipping planes
	gluPerspective(40.0, (GLdouble)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0, 250.0);


	// Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Color
	glClearColor(0, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float lightSpecularColor[] = { 1, 1, 1 };
	float lightDiffuseColor[] = { 1, 1, 1 }; 
	float lightAmbient[] = { 0.1, 0.1, 0.1 };
	float lightPos[] = { 0, 50, 0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		cout << "Error initializing OpenGL!  " << gluErrorString(error) << endl;
		success = false;
	}

	return success;
}

void update(Drawable* formlist[MAX_FORMS_NUMBER])
{
	// Update the list of forms
	unsigned short i = 0;
	while (formlist[i] != NULL)
	{
		formlist[i]->update(ANIM_DELAY / 1000.0);
		i++;
	}
}

void handleZoom(bool *zooming, float *zoomValue, float *zoomStep)
{
	if (*zooming)
	{
		*zoomValue -= *zoomStep;

		if (*zoomValue <= 0)
		{
			scene_zoom.x *= 0.95;
			scene_zoom.y *= 0.95;
			scene_zoom.z *= 0.95;
		}
		else
		{
			scene_zoom.x *= 1.05;
			scene_zoom.y *= 1.05;
			scene_zoom.z *= 1.05;
		}
		if (abs(*zoomValue) <= 0.01)
		{
			*zooming = false;
		}
	}

}

const void render(Drawable* formlist[MAX_FORMS_NUMBER], const Point &cam_pos, const Point &cam_target)
{
	// Clear color buffer and Z-Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the camera position and parameters
	gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z, cam_target.x, cam_target.y, cam_target.z, 0.0, 1.0, 0.0);

	glTranslated(scene_translation.x, scene_translation.y, scene_translation.z);
	glRotated(scene_angle.x, 1, 0, 0);
	glRotated(scene_angle.y, 0, 1, 0);
	glRotated(scene_angle.z, 0, 0, 1);
	glScaled(scene_zoom.x, scene_zoom.y, scene_zoom.z);
	glUseProgram(test->getProgramID());
	// X, Y and Z axis
	glPushMatrix(); // Preserve the camera viewing point for further forms
	// Render the coordinates system
	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(1, 0, 0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 1, 0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, 1);
	}
	glEnd();
	glPopMatrix();

	// Render the list of forms
	unsigned short i = 0; 
	while (formlist[i] != NULL)
	{
		formlist[i]->render();
		i++;
	}
}

void close(SDL_Window** window)
{
	//Destroy window
	SDL_DestroyWindow(*window);
	*window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int wmain(int argc, char* args[])
{
	// The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	// OpenGL context
	SDL_GLContext gContext;

	// Start up SDL and create window
	if (!init(&gWindow, &gContext))
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		// Main loop flag
		printf("%s\n\n", glGetString(GL_VERSION));

		bool quit = false;
		bool zooming = false;
		bool dragging = false;


		float zoomStep = 0;
		float zoomValue = 0;
		Uint32 current_time, previous_time;

		// Event handler
		SDL_Event event;

		// Camera position
		Point camera_position(0, 5, 5);
        Point camera_target(0,0,0);

		float initialWindSpeed = 10;
		Vector windDirection = Vector(1, 0, 1);
		WindSystem* windSystem = new WindSystem(windDirection, initialWindSpeed, Point(-20, 22, 0), BLUE);
		Eolienne* eolienne = new Eolienne(Point(0, 7, 0));
		Skybox* skybox = new Skybox(Point(1, 0, 0));
		eolienne->getPales()->updateSpeed(initialWindSpeed, windSystem->getAngleFactor());

		// The forms to render
		Drawable* forms_list[MAX_FORMS_NUMBER];
		unsigned short i;
		for (i = 0; i < MAX_FORMS_NUMBER; i++)
		{
			forms_list[i] = NULL;
		}

		AirHokey* h = new AirHokey(Point(-40, 0, 0));
		forms_list[3] = h;
		forms_list[0] = skybox;
		forms_list[1] = eolienne;
		forms_list[2] = windSystem;

		test = singleton<ShaderManager>().loadData("model/particle");


		// Get first "current time"
		previous_time = SDL_GetTicks();
		// While application is running
		// While application is running
		while (!quit)
		{
			// Handle events on queue
			while (SDL_PollEvent(&event) != 0)
			{
				int x = 0, y = 0;
				SDL_Keycode key_pressed = event.key.keysym.sym;
				float distance = sqrt(pow((camera_position.x), 2) + pow((camera_position.y), 2) + pow((camera_position.z), 2));
				switch (event.type)
				{
					// User requests quit
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
						dragging = true;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
					{

						camera_target.x = scene_translation.x;
						camera_target.y = scene_translation.y;
						camera_target.z = scene_translation.z;
						scene_angle.x = 0;
						scene_angle.y = 0;
						scene_angle.z = 0;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					dragging = false;
					break;
				case SDL_MOUSEWHEEL:
				{
					if (!zooming)
					{
						zooming = true;

						zoomValue = event.wheel.y/200.0;
						zoomStep = zoomValue / 100.0;
					}
				}
				case SDL_KEYDOWN:
					// Handle key pressed with current mouse position
					SDL_GetMouseState(&x, &y);

					switch (key_pressed)
					{
						// Quit the program when 'q' or Escape keys are pressed
					case SDLK_RIGHT:
					case SDLK_d:
						scene_translation.x -= scene_zoom.x;
						break;
					case SDLK_LEFT:
					case SDLK_q:
						scene_translation.x += scene_zoom.x;
						break;
					case SDLK_UP:
					case SDLK_z:
						scene_translation.y -= scene_zoom.x;
						break;
					case SDLK_DOWN:
					case SDLK_s:
						scene_translation.y += scene_zoom.x;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_i:
						windSystem->setSpeed(min(35.0, windSystem->getSpeed() * 1.2));
						eolienne->getPales()->updateSpeed(windSystem->getSpeed(), windSystem->getAngleFactor());
						break;
					case SDLK_k:
						windSystem->setSpeed(max(4.0, windSystem->getSpeed() * 0.8));
						eolienne->getPales()->updateSpeed(windSystem->getSpeed(), windSystem->getAngleFactor());
						break;
					case SDLK_m:
						windSystem->setDirection(
							Vector(
								windSystem->getDirection().x,
								windSystem->getDirection().y,
								windSystem->getDirection().z + 0.1
							)
						);
						eolienne->getPales()->updateSpeed(windSystem->getSpeed(), windSystem->getAngleFactor());
						break;
					case SDLK_l:
						windSystem->setDirection(
							Vector(
								windSystem->getDirection().x,
								windSystem->getDirection().y,
								windSystem->getDirection().z - 0.1
							)
						);
						eolienne->getPales()->updateSpeed(windSystem->getSpeed(), windSystem->getAngleFactor());
						break;
					default:
						break;
					}
					break;

				case SDL_MOUSEMOTION:
				{
					if (dragging)
					{
						if (event.motion.xrel < 0)
							scene_angle.y += event.motion.xrel * distance * cos(event.motion.xrel*M_PI / 180) / 20;
						else
							scene_angle.y += event.motion.xrel * distance * cos(event.motion.xrel*M_PI / 180) / 20;
						if (event.motion.yrel < 0)
							scene_angle.x += event.motion.yrel * distance * cos(event.motion.yrel*M_PI / 180) / 20;
						else
							scene_angle.x += event.motion.yrel * distance * cos(event.motion.yrel*M_PI / 180) / 20;
					}
				}
				break;


				default:
					break;
				}
			}

			// Update the scene
			current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
			if ((current_time - previous_time) > ANIM_DELAY)
			{
				previous_time = current_time;
				update(forms_list);
			}
			// Render the scene
			handleZoom(&zooming, &zoomValue, &zoomStep);

			render(forms_list, camera_position, camera_target);

			// Update window screen
			SDL_GL_SwapWindow(gWindow);
		}
	}

	// Free resources and close SDL
	close(&gWindow);

	return 0;
}
