//
//  main.hpp
//  project_3
//
//  Created by Tess Daughton on 11/19/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#ifndef main_hpp
#define main_hpp

#pragma debug
#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "GlError.h"
#include "read.cpp"
#include "shader.hpp"
#include "normals.cpp"

#define CLIP_VALUE 45.0

using namespace std;

/****** Global Variables *********************************************************/

/** Default Camera Values **/
glm::vec3 FOCUS_POINT = glm::vec3(0.0,0.0,0.0f);
glm::vec3 DEFAULT_CAMERA_POSITION = glm::vec3(-4,0,0);
glm::vec3 DEFAULT_CAMERA_FRONT = FOCUS_POINT-DEFAULT_CAMERA_POSITION;
glm::vec3 DEFAULT_CAMERA_UP = glm::vec3(0.0,1.0,0.0f);

/** Actual Camera Values **/
glm::vec3 camera_position = DEFAULT_CAMERA_POSITION;
glm::vec3 camera_front = DEFAULT_CAMERA_FRONT;
glm::vec3 camera_up = DEFAULT_CAMERA_UP;
glm::vec3 camera_right = glm::normalize(glm::cross(camera_up,camera_front));

/** Other Classes **/
Reader reader(0.0,0.0,1.0);
Normals normal;

/** Camera Movement Values **/
glm::mat4 translation = glm::mat4(1.0f);
glm::mat4 scaling = glm::mat4(1.0f);
glm::mat4 rotation = glm::mat4(1.0f);
glm::mat4 modelview = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 MVP=glm::mat4(1.0f);
GLfloat* mvp_arr = new GLfloat[16];
GLfloat* projection_arr = new GLfloat[16];
GLfloat* model_view_arr = new GLfloat[16];

/** Globals for object properties (color, clipping, mode, etc) **/
GLFWwindow* shaded_window, *norm_window;
GLfloat max_dist_to_origin = 0.0;
bool cw = true;
float near_clip = 0.01f;
float far_clip = 10.0f;
float aspect = 0.0f;
float camera_speed = 1.0f;
int window_width, window_height;
int buffer_dirty_bit=1;
char mode='f';
char current_shading='s';
char lighting_enabled='t';

//shader and program ids
GLuint pid=0;
//buffer data handles
GLuint mvp_id=0; GLuint vp_id=0; GLuint color_id=0; GLuint norm_id=0;

/** Fixed Pipeline Variables **/
GLfloat* coord_all_arr;
GLfloat* all_norms_arr;

/** Shading Variables **/
GLuint VBO, EBO, VAO;
GLfloat* current_buffer_arr;
GLuint* faces_arr;
char* current_vert_shader="shaders/phongVert.txt";
char* current_frag_shader="shaders/phongFrag.txt";

/** Lighting Variables **/
GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
GLint spot_exponent = 30;
GLint spot_cutoff = 180;
GLfloat Kc = 1.0;
GLfloat Kl = 0.0;
GLfloat Kq = 0.0;
GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
GLfloat light_Ka[]  = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat light_Kd[]  = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
GLfloat material_Se = 20.0f;


/****** Functions *********************************************************/

/** Drawing Functions **/
void draw_faces_shaded();
void draw_faces();
void display();

/** Camera Movement Functions **/
void calc_perspective(float near, float far, float aspect, float fov);
void translate(float x, float y, float z);
void scale(float zoom);
void rotate(char axis, float inc);
void set_ModelViewProjection();

/** Utility Functions **/
void mat_to_arr(glm::mat4 mat, GLfloat* arr);
void assign_to_array(GLfloat* array, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

/** Shader Functions **/
void load_shaders();
void create_buffers();

/** User Input Handling **/
void cycle_lighting(char lighting_type, int rgb);
void toggle_shading();
void toggle_lighting();
void fbreshape(GLFWwindow *window, int w, int h);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

/** Clean Up and Resets **/
void exit_scene();
void reset_camera();
void reset_lighting();
void reset_scene(char* file);

/** Context and Camera Initializers **/
void init_camera();
void init_lighting();
void init_GLFW();
void init_GLEW();

#endif /* normals_hpp */
