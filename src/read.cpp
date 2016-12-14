//
//  read.cpp
//  project_1
//
//  Created by Tess Daughton on 10/24/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glm/glm.hpp"

using namespace std;

class Reader
{
    
private:
    vector<GLfloat> coord_all;
    vector<GLfloat> vertices;
    
    vector<GLfloat> verts_spaced;

    
    vector<GLfloat> normals;
    vector<GLuint> faces;
    
    GLfloat* coord_all_arr;
    GLfloat* vert_all_arr;
    
    GLfloat* norm_arr;
    GLuint* face_all_arr;
    
    GLfloat current_color[3];
    char* current_file_name;
    GLfloat max_dist_to_origin;
    
    void read_file()
    {
        coord_all.clear();
        vertices.clear();
        verts_spaced.clear();
        faces.clear();
        FILE* file = fopen(current_file_name, "r");
        
        if (file==NULL) {
            printf("Cannot open file");
            return;
        }
        
        char current[128];
        char vertex_1[128];
        char vertex_2[128];
        char vertex_3[128];
        
        vector<GLfloat> coords;
        vector<vector <GLfloat> > coord_list;
        
        vector<GLfloat> norms;
        vector<vector <GLfloat> > norm_list;
        
        GLfloat v_1;
        GLfloat v_2;
        GLfloat v_3;
        GLfloat distance;
        
        vector<GLfloat> temp_1;
        
        string::size_type sz; // alias of size_t
        
        while(1)
        {
            if(fscanf(file, "%s", current)==EOF) { break; }
            
            if(strcmp(current,"v")==0)
            {
                fscanf(file, "%s %s %s", vertex_1, vertex_2, vertex_3);
                string::size_type sz;     // alias of size_t
                v_1 = stof(vertex_1,&sz);
                v_2 = stof(vertex_2,&sz);
                v_3 = stof(vertex_3,&sz);
                
                distance = sqrt((v_1-0)*(v_1-0) + (v_2-0)*(v_2-0) + (v_3-0)*(v_3-0));
                if(distance>max_dist_to_origin)
                    max_dist_to_origin=distance;
                
                //for drawing w/o shading
                coords.push_back(v_1);
                coords.push_back(v_2);
                coords.push_back(v_3);
                
                coord_list.push_back(coords);
                coords.clear();
                
                //for drawing w shading, buffers
                vertices.push_back(v_1);
                vertices.push_back(v_2);
                vertices.push_back(v_3);
                
                
                //for drawing w shading, buffers
                verts_spaced.push_back(v_1);
                verts_spaced.push_back(v_2);
                verts_spaced.push_back(v_3);
                verts_spaced.push_back(0);
                verts_spaced.push_back(0);
                verts_spaced.push_back(0);
                
            }
            
            //support for reading in norms direct from obj file
            else if(strcmp(current, "vn")==0)
            {
                fscanf(file, "%s %s %s", vertex_1, vertex_2, vertex_3);
                v_1 = stoi(vertex_1,&sz)-1;
                v_2 = stoi(vertex_2,&sz)-1;
                v_3 = stoi(vertex_3,&sz)-1;
                
                norms.push_back(v_1);
                norms.push_back(v_2);
                norms.push_back(v_3);
                
                norm_list.push_back(norms);
                norms.clear();
            }
            
            if(strcmp(current,"f")==0)
            {
                fscanf(file, "%s %s %s", vertex_1, vertex_2, vertex_3);
                v_1 = stoi(vertex_1,&sz)-1;
                v_2 = stoi(vertex_2,&sz)-1;
                v_3 = stoi(vertex_3,&sz)-1;
                
                //for drawing w/o shading
                temp_1=coord_list.at(v_1);
                coord_all.push_back(temp_1.at(0));
                coord_all.push_back(temp_1.at(1));
                coord_all.push_back(temp_1.at(2));
                
                
                temp_1=coord_list.at(v_2);
                coord_all.push_back(temp_1.at(0));
                coord_all.push_back(temp_1.at(1));
                coord_all.push_back(temp_1.at(2));
                
                
                temp_1=coord_list.at(v_3);
                coord_all.push_back(temp_1.at(0));
                coord_all.push_back(temp_1.at(1));
                coord_all.push_back(temp_1.at(2));
                
                
                temp_1.clear();
                
                //for drawing w shading, buffers
                faces.push_back(v_1);
                faces.push_back(v_2);
                faces.push_back(v_3);
                
                
                if(norm_list.size()>0)
                {
                    temp_1 = norm_list.at(v_1);
                    normals.push_back(temp_1.at(0));
                    normals.push_back(temp_1.at(1));
                    normals.push_back(temp_1.at(2));
                    
                    temp_1 = norm_list.at(v_2);
                    normals.push_back(temp_1.at(0));
                    normals.push_back(temp_1.at(1));
                    normals.push_back(temp_1.at(2));
                    
                    temp_1 = norm_list.at(v_3);
                    normals.push_back(temp_1.at(0));
                    normals.push_back(temp_1.at(1));
                    normals.push_back(temp_1.at(2));
                }
                
                temp_1.clear();
            }
        }
        norm_list.clear();
        coords.clear();
        coord_list.clear();
        fclose(file);
    }
    
    
public:
    
    Reader(GLfloat red, GLfloat green, GLfloat blue){
        
        max_dist_to_origin=0.0;
        current_file_name="obj/bunny.obj";
        current_color[0]=red;
        current_color[1]=green;
        current_color[2]=blue;
        read_file();
    }
    
    /** Setters **/
    
    void set_file(char* file_name)
    {
        current_file_name=file_name;
        read_file();
    }
    
    void set_color(GLfloat* color)
    {
        current_color[0]=color[0];
        current_color[1]=color[1];
        current_color[2]=color[2];
    }
    
    
    /** Vector Getters **/

    GLfloat* get_coord_array()
    {
        coord_all_arr = new GLfloat[coord_all.size()];
        std::copy(coord_all.begin(), coord_all.end(), coord_all_arr);
        return coord_all_arr;
    }
    
    GLuint* get_face_array()
    {
        face_all_arr = new GLuint[faces.size()];
        std::copy(faces.begin(), faces.end(), face_all_arr);
        return face_all_arr;
    }
    
    vector<GLfloat> get_vertices()
    {
        return vertices;
    }
    
    vector<GLfloat> get_spaced_vertices()
    {
        return verts_spaced;
    }
    
    vector<GLuint> get_faces()
    {
        return faces;
    }
    
    glm::vec3 get_camera_position()
    {
        return glm::vec3{-3*max_dist_to_origin,0,0};
    }
    
    
    /** Sizes **/
    
    unsigned long vert_size()
    {
        return vertices.size();
    }
    
    unsigned long face_size()
    {
        return faces.size();
    }
    
    unsigned long coord_size()
    {
        return coord_all.size();
    }
    

    
    void free_all()
    {
        delete [] coord_all_arr;
        delete [] vert_all_arr;
        delete [] norm_arr;
        delete [] face_all_arr;
    }
};
