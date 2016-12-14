//
//  main.cpp
//  project1
//
//  Created by Tess Daughton on 9/11/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//

#include "main.hpp"

/***********************************************************************************/
/****** Drawing and Display ********************************************************/
/***********************************************************************************/

void draw_faces_shaded()
{
    glUseProgram(pid);
    
    if (cw){
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }
    else{
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }
    
    glValidateProgram(pid);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArrayAPPLE(VAO);
    glDrawElements(GL_TRIANGLES, (reader.face_size()*sizeof(GLuint))/4, GL_UNSIGNED_INT, 0  );
    glBindVertexArrayAPPLE(0);
    
    glUseProgram(0);
    
    glFlush();
}

void draw_faces()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, all_norms_arr );
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, coord_all_arr );
    
    glDrawArrays( GL_TRIANGLES, 0, reader.coord_size()/3);

    glfwSwapBuffers(norm_window);
}

/** Recenter camera on user keyboard input 'r' **/
void display()
{
    //Shaders
    glfwMakeContextCurrent(shaded_window);
    draw_faces_shaded();
    create_buffers();
    load_shaders();
    set_ModelViewProjection();
    
    //Fixed Pipeine
    glfwMakeContextCurrent(norm_window);
    draw_faces();
    set_ModelViewProjection();
}

/***********************************************************************************/
/****** Camera Movement and Matrix Calculations ************************************/
/***********************************************************************************/

void calc_perspective(float near, float far, float aspect, float fov)
{
    float frustrumDepth = far-near;
    float inv_depth = 1/frustrumDepth;
    
    projection[1][1]= 1/tan(glm::radians(0.5f*fov));
    projection[0][0]= projection[1][1]/aspect;
    projection[2][2]= far*inv_depth;
    projection[3][2]= (-far * near) * inv_depth;
    projection[2][3]= 1;
    projection[3][3]= 0;
}

void rotate(char axis, float inc)
{
    float rotx[16] = {1,0.0,0.0,0.0,0.0,cos(inc),-sin(inc),0,0,sin(inc),cos(inc),0,0,0,0,1};
    float roty[16] = {cos(inc),0,sin(inc),0,0,1,0,0,-sin(inc),0,cos(inc),0,0,0,0,1};
    float rotz[16] = {cos(inc), -sin(inc), 0.0, 0.0,sin(inc), cos(inc),0.0,0.0,0.0,0.0,1.0,0.0,
        0.0,      0.0,  0.0, 1.0};
    
    switch(axis){
        case 'x':
            
            rotation = glm::make_mat4(rotx);
            break;
        case 'y':
            rotation = glm::make_mat4(roty);
            break;
        case 'z':
            
            rotation = glm::make_mat4(rotz);
            break;
    }
    
    modelview = rotation * modelview;
}

void translate(float x, float y, float z)
{
    translation[3][0] = x;
    translation[3][1] = y;
    translation[3][2] = z;
    
    modelview = translation * modelview;
}

void scale(float zoom)
{
    scaling=glm::mat4();
    scaling[2][2] *= zoom;
    
    modelview = scaling * modelview;
}

void set_ModelViewProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    switch(mode){
        case 'l':
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            break;
        case 'f':
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            break;
        case 'p':
            glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
            break;
    }
    
    if (cw){
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }
    else{
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }
    
    
    calc_perspective(near_clip, far_clip, aspect, CLIP_VALUE);
    mat_to_arr(projection, projection_arr);
    glLoadMatrixf(projection_arr);
    
    
    glMatrixMode(GL_MODELVIEW);
    mat_to_arr(modelview, model_view_arr);
    glLoadMatrixf(model_view_arr);
}

/***********************************************************************************/
/****** Utility Functions **********************************************************/
/***********************************************************************************/

void mat_to_arr(glm::mat4 mat, GLfloat* arr)
{
    int i=0;
    
    for(int j=0; j<4; j++)
        for(int k=0; k<4; k++)
        {
            arr[i]=mat[j][k];
            i++;
        }
}

void assign_to_array(GLfloat* array, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    array[0]=x;
    array[1]=x;
    array[2]=x;
    array[3]=x;
}

/***********************************************************************************/
/****** Shading Functions **********************************************************/
/***********************************************************************************/

void load_shaders()
{
    pid = LoadShaders(current_vert_shader,
                      current_frag_shader);
    
    glUseProgram(pid);
    
    mvp_id = glGetUniformLocation(pid,"ModelViewProjection");
    
    vp_id = glGetAttribLocation(pid,"vertPos");
    norm_id = glGetAttribLocation(pid, "vertNorm");
    
    glMatrixMode(GL_PROJECTION);
    glGetFloatv(GL_PROJECTION_MATRIX, projection_arr);
    glMatrixMode(GL_MODELVIEW);
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view_arr);
    
    MVP = projection*modelview;
    mat_to_arr(MVP, mvp_arr);
    
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, mvp_arr);
}

void create_buffers()
{
    if(buffer_dirty_bit)
    {
        
    buffer_dirty_bit=0;

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VAO);

    VAO=0; EBO=0; VBO=0;
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // setup VAO
    glGenVertexArraysAPPLE(1, &VAO);
    glBindVertexArrayAPPLE(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, reader.vert_size()*sizeof(GLfloat)*2, current_buffer_arr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(vp_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(vp_id);
    
    if(lighting_enabled=='t')
    {
        glVertexAttribPointer(norm_id, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(norm_id);
    }
    
    check_gl_error();
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, reader.face_size()*sizeof(GLuint), faces_arr, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArrayAPPLE(0);
    }
}


/***********************************************************************************/
/****** User Input Handling ********************************************************/
/***********************************************************************************/


/** User Keybord Input Handling **/
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key==GLFW_KEY_LEFT && action==GLFW_PRESS)
        translate(1,0,0);
    else if (key==GLFW_KEY_RIGHT && action==GLFW_PRESS)
        translate(-1,0,0);
    else if (key==GLFW_KEY_UP && action==GLFW_PRESS)
        translate(0,0,-1);
    else if (key==GLFW_KEY_DOWN && action==GLFW_PRESS)
        translate(0,0,1);
    else if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        exit_scene();
    else if (glfwGetKey(norm_window, GLFW_KEY_1) && glfwGetKey(norm_window, GLFW_KEY_A))
        cycle_lighting('a',0);
    else if (glfwGetKey(norm_window, GLFW_KEY_2) && glfwGetKey(norm_window, GLFW_KEY_A))
        cycle_lighting('a',1);
    else if (glfwGetKey(norm_window, GLFW_KEY_3) && glfwGetKey(norm_window, GLFW_KEY_A))
        cycle_lighting('a',2);
    else if (glfwGetKey(norm_window, GLFW_KEY_1) && glfwGetKey(norm_window, GLFW_KEY_S))
        cycle_lighting('s',0);
    else if (glfwGetKey(norm_window, GLFW_KEY_2) && glfwGetKey(norm_window, GLFW_KEY_S))
        cycle_lighting('s',1);
    else if (glfwGetKey(norm_window, GLFW_KEY_3) && glfwGetKey(norm_window, GLFW_KEY_S))
        cycle_lighting('s',2);
    else if (glfwGetKey(norm_window, GLFW_KEY_1) && glfwGetKey(norm_window, GLFW_KEY_D))
        cycle_lighting('d',0);
    else if (glfwGetKey(norm_window, GLFW_KEY_2) && glfwGetKey(norm_window, GLFW_KEY_D))
        cycle_lighting('d',1);
    else if (glfwGetKey(norm_window, GLFW_KEY_3) && glfwGetKey(norm_window, GLFW_KEY_D))
        cycle_lighting('d',2);
    else if (glfwGetKey(norm_window, GLFW_KEY_1) && glfwGetKey(norm_window, GLFW_KEY_G))
        cycle_lighting('g',0);
    else if (glfwGetKey(norm_window, GLFW_KEY_2) && glfwGetKey(norm_window, GLFW_KEY_G))
        cycle_lighting('g',1);
    else if (glfwGetKey(norm_window, GLFW_KEY_3) && glfwGetKey(norm_window, GLFW_KEY_G))
        cycle_lighting('g',2);
    else if (key==GLFW_KEY_W && action==GLFW_PRESS)
        scale(0.9);
    else if (key==GLFW_KEY_S && action==GLFW_PRESS)
        scale(1.1);
    else if (key==GLFW_KEY_A && action==GLFW_PRESS)
        translate(0,-1,0);
    else if (key==GLFW_KEY_D && action==GLFW_PRESS)
        translate(0,1,0);
    else if (key==GLFW_KEY_Q && action==GLFW_PRESS)
        rotate('x',-0.04);
    else if (key==GLFW_KEY_E && action==GLFW_PRESS)
        rotate('x',0.04);
    else if (key==GLFW_KEY_Z && action==GLFW_PRESS)
        rotate('y',-0.04);
    else if (key==GLFW_KEY_C && action==GLFW_PRESS)
        rotate('y',0.04);
    else if (key==GLFW_KEY_V && action==GLFW_PRESS)
        rotate('z',-0.04);
    else if (key==GLFW_KEY_B && action==GLFW_PRESS)
        rotate('z',0.04);
    else if (key==GLFW_KEY_R && action==GLFW_PRESS)
    {
        reset_camera();
        reset_lighting();
    }
    else if (key==GLFW_KEY_X && action==GLFW_PRESS)
    {
        if (cw) cw=false;
        else cw=true;
    }
    else if (key==GLFW_KEY_EQUAL && action==GLFW_PRESS)
    {
        far_clip+=1.0;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_MINUS && action==GLFW_PRESS)
    {
        far_clip-=1.0;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_RIGHT_BRACKET && action==GLFW_PRESS)
    {
        near_clip+=0.01;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_LEFT_BRACKET && action==GLFW_PRESS)
    {
        near_clip-=0.01;
        calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    }
    else if (key==GLFW_KEY_N && action==GLFW_PRESS)
        mode = 'f';
    else if (key==GLFW_KEY_M && action==GLFW_PRESS)
        mode = 'p';
    else if (key==GLFW_KEY_COMMA && action==GLFW_PRESS)
        mode = 'l';
    else if (key==GLFW_KEY_LEFT_ALT && action==GLFW_PRESS)
        reset_scene("obj/bunny.obj");
    else if (key==GLFW_KEY_RIGHT_ALT && action==GLFW_PRESS)
        reset_scene("obj/cactus.obj");
    else if (key==GLFW_KEY_SPACE && action==GLFW_PRESS)
        toggle_shading();
    else if (key==GLFW_KEY_LEFT_CONTROL && action==GLFW_PRESS)
        toggle_lighting();
}

/** Switch between Flat and Smooth Shading **/
void toggle_shading()
{
    if(lighting_enabled=='t')
    {
        if(current_shading=='s')
        {
            current_shading='f';
            current_vert_shader="shaders/flatVert.txt";
            current_frag_shader="shaders/flatFrag.txt";
            all_norms_arr=normal.get_fixed_flat_norms();
            current_buffer_arr=normal.get_shader_smooth_norms();
        }
        else
        {
            current_shading='s';
            current_vert_shader="shaders/phongVert.txt";
            current_frag_shader="shaders/phongFrag.txt";
            all_norms_arr=normal.get_fixed_smooth_norms();
            current_buffer_arr=normal.get_shader_smooth_norms();
        }
    }
}

/** Turn Light on and off **/
void toggle_lighting()
{
    if(lighting_enabled=='t')
    {
        lighting_enabled='f';
        
        glfwMakeContextCurrent(norm_window);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glfwMakeContextCurrent(shaded_window);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        
    }
    else
    {
        if(current_shading=='s')
        {
            current_vert_shader="shaders/phongVert.txt";
            current_frag_shader="shaders/phongFrag.txt";
        }
        else
        {
            current_vert_shader="shaders/flatVert.txt";
            current_frag_shader="shaders/flatFrag.txt";
        }
        glfwMakeContextCurrent(norm_window);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glfwMakeContextCurrent(shaded_window);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        lighting_enabled='t';
    }
    buffer_dirty_bit=1;
    load_shaders();
    create_buffers();
}

void cycle_lighting(char lighting_type, int rgb)
{
    switch (lighting_type){
        case 'a':
            light_Ka[rgb]+=0.1;
            if(light_Ka[rgb]>1) light_Ka[rgb]=0;
            printf("Ambient: %f %f %f \n", light_Ka[0], light_Ka[1], light_Ka[2] );
            glfwMakeContextCurrent(norm_window);
            glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
            glfwMakeContextCurrent(shaded_window);
            glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
            break;
        case 'd':
            light_Kd[rgb]+=0.1;
            if(light_Kd[rgb]>1) light_Kd[rgb]=0;
            printf("Diffuse: %f %f %f \n", light_Kd[0], light_Kd[1], light_Kd[2] );
            glfwMakeContextCurrent(norm_window);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
            glfwMakeContextCurrent(shaded_window);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
            break;
        case 's':
            light_Ks[rgb]+=0.1;
            if(light_Ks[rgb]>1) light_Ks[rgb]=0;
            printf("Specular: %f %f %f \n", light_Ks[0], light_Ks[1], light_Ks[2] );
            glfwMakeContextCurrent(norm_window);
            glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
            glfwMakeContextCurrent(shaded_window);
            glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
            break;
        case 'g':
            lmKa[rgb]+=0.1;
            if(lmKa[rgb]>1) lmKa[rgb]=0;
            printf("Global Ambient: %f %f %f \n", lmKa[0], lmKa[1], lmKa[2] );
            glfwMakeContextCurrent(norm_window);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
            glfwMakeContextCurrent(shaded_window);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
            break;
    }
}

void fbreshape(GLFWwindow *window, int w, int h)
{
    window_width=w;
    window_height=h;
    aspect = (GLfloat)w/(GLfloat) h;
}

/***********************************************************************************/
/****** Cleanup and Resets *********************************************************/
/***********************************************************************************/

/** Close windows and free necessary variables **/
void exit_scene()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VAO);
    
    glDeleteProgram(pid);
    
    normal.free_all();
    reader.free_all();
    
    glfwDestroyWindow(shaded_window);
    glfwDestroyWindow(norm_window);
    glfwTerminate();
    exit(0);
}

/** Recenter camera on user keyboard input 'r' **/
void reset_camera()
{
    camera_position = reader.get_camera_position();
    far_clip = camera_position.x;
    camera_front=DEFAULT_CAMERA_FRONT;
    camera_up=DEFAULT_CAMERA_UP;
    glMatrixMode(GL_PROJECTION);
    modelview = glm::mat4();
    calc_perspective(near_clip,far_clip,aspect,CLIP_VALUE);
    translate(0,0,4);
}

/** Fetch new .obj file and recalculate scene values **/
void reset_scene(char* file)
{
    buffer_dirty_bit=1;
    
    //reader reset
    max_dist_to_origin=0;
    reader.set_file(file);
    coord_all_arr = reader.get_coord_array();
    camera_position=reader.get_camera_position();
    
    //normal reset
    normal.recalculate_normals(reader.get_vertices(), reader.get_faces(), reader.get_spaced_vertices());
    current_buffer_arr =normal.get_shader_smooth_norms();
    faces_arr=reader.get_face_array();
    if(current_shading=='f') all_norms_arr=normal.get_fixed_flat_norms();
    else all_norms_arr=normal.get_fixed_smooth_norms();
}

void reset_lighting()
{
    assign_to_array(light_Ka, 1.0f, 0.5f, 0.5f, 1.0f);
    assign_to_array(light_Ks, 1.0f, 1.0f, 1.0f, 1.0f);
    assign_to_array(light_Kd, 1.0f, 0.1f, 0.1f, 1.0f);
    assign_to_array(lmKa, 0.0f, 0.0f, 0.0f, 0.0f);
    glfwMakeContextCurrent(norm_window);
    init_lighting();
    glfwMakeContextCurrent(shaded_window);
    init_lighting();
}

/***********************************************************************************/
/****** Context and Camera Initializers ********************************************/
/***********************************************************************************/

void init_camera()
{
    coord_all_arr = reader.get_coord_array();
    faces_arr = reader.get_face_array();
    camera_position=reader.get_camera_position();
    camera_front = glm::normalize(FOCUS_POINT-camera_position);
    translate(0,0,4);
    
    normal.calc_normals(reader.get_vertices(), reader.get_faces(), reader.get_spaced_vertices());
    current_buffer_arr = normal.get_shader_smooth_norms();
    all_norms_arr=normal.get_fixed_smooth_norms();
}


void init_lighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    
    // Light model parameters:
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
    
    // Spotlight Attenuation
    
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
    glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
    
    // Lighting parameters:
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
    
    // Material parameters:
    //    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
    
    glfwMakeContextCurrent(shaded_window);
}


void init_GLFW()
{
    /* Initialize GLFW */
    if (!glfwInit()) {
        fprintf(stderr, "Failed to open GLFW window\n");
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    shaded_window = glfwCreateWindow(500,400, "Shaded", NULL, NULL);
    norm_window = glfwCreateWindow(500,400, "Normal", NULL, NULL);
    
    if (!shaded_window || !norm_window)
    {
        printf("Windows not created correctly");
        glfwTerminate();
    }
    
    glfwSetWindowPos(shaded_window,0,0);
    glfwSetWindowPos(norm_window, 500, 0);
    
    glfwMakeContextCurrent(shaded_window);
    glfwGetFramebufferSize(shaded_window, &window_width,
                           &window_height);
    fbreshape(shaded_window, window_width, window_height);
    glfwSetInputMode(shaded_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(shaded_window, keyboard);
    init_lighting();
    
    glfwMakeContextCurrent(norm_window);
    glfwGetFramebufferSize(norm_window, &window_width,
                           &window_height);
    fbreshape(norm_window, window_width, window_height);
    glfwSetInputMode(norm_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(norm_window, keyboard);
    init_lighting();
}

void init_GLEW()
{
    glewExperimental=true;
    
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
}


/***********************************************************************************/
/****** MAIN ***********************************************************************/
/***********************************************************************************/

int main(int argc, char **argv){
    
    init_camera();
    init_GLFW();
    init_GLEW();
    
    load_shaders();
    create_buffers();
    buffer_dirty_bit=1;
    
    set_ModelViewProjection();

    while (!glfwWindowShouldClose(shaded_window) && !glfwWindowShouldClose(norm_window))
    {
        display();
        glfwSwapBuffers(shaded_window);
        glfwSwapBuffers(norm_window);
        glfwPollEvents();
    }
    
    exit_scene();
    return 1;
}
