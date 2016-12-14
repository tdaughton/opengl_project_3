//
//  normals.cpp
//  project_3
//
//  Created by Tess Daughton on 11/19/16.
//  Copyright © 2016 Tess Daughton. All rights reserved.
//
//
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glm/glm.hpp"

using namespace std;

class Normals
{
private:
    
    /** Fixed Pipeline Variables **/

    //store the normals of each vertex, hold the average at the end
    vector<GLfloat> fixed_face_normals;
    GLfloat* fixed_face_norms;
    GLfloat* fixed_vert_norms;


    /** Shader Variables **/
    vector<GLfloat> verts_spaced;
    vector<GLfloat> vertices;
    vector<GLuint> faces;
    GLfloat* shader_vert_norms;
    
    /** Shared **/
    vector<vector <glm::vec3> > avg_vert_norms;
    
    //take 3 position vectors and calculate normal
    glm::vec3 get_face_normal(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3 )
    {
        return glm::normalize(glm::cross(pos2-pos1,pos3-pos1));
    }
    
    void calc_all_face_normals()
    {
        GLuint face_1, face_2, face_3;
        
        GLfloat x1, y1, z1;
        GLfloat x2, y2, z2;
        GLfloat x3, y3, z3;
        
        glm::vec3 vec1, vec2, vec3;
        
        glm::vec3 temp_norm;
        
        avg_vert_norms.resize(vertices.size()/3);
        
        for(int i=0; i<faces.size(); i+=3)
        {
         
            face_1 = faces.at(i);
            face_2= faces.at(i+1);
            face_3 = faces.at(i+2);
            
            x1 = vertices.at(face_1*3);
            y1 = vertices.at(face_1*3+1);
            z1 = vertices.at(face_1*3+2);
            
            x2 = vertices.at(face_2*3);
            y2 = vertices.at(face_2*3+1);
            z2 = vertices.at(face_2*3+2);
            
            x3 = vertices.at(face_3*3);
            y3 = vertices.at(face_3*3+1);
            z3 = vertices.at(face_3*3+2);
        
            vec1 = {x1, y1, z1};
            vec2 = {x2, y2, z2};
            vec3 = {x3, y3, z3};
            
            temp_norm = get_face_normal(vec1, vec2, vec3);
            
            for(int i=0; i<3; i++)
            {
                fixed_face_normals.push_back(temp_norm.x);
                fixed_face_normals.push_back(temp_norm.y);
                fixed_face_normals.push_back(temp_norm.z);
            }
            
            avg_vert_norms.at(face_1).push_back(temp_norm);
            avg_vert_norms.at(face_2).push_back(temp_norm);
            avg_vert_norms.at(face_3).push_back(temp_norm);
        }
    }
    
    void translate_normals()
    {
        glm::vec3 average={0,0,0};
        glm::vec3 temp={0,0,0};
        
        //for shaders
        fixed_face_norms = (GLfloat *) malloc(sizeof(GLfloat) * fixed_face_normals.size());
        shader_vert_norms = new GLfloat[verts_spaced.size()];
        std::copy(verts_spaced.begin(), verts_spaced.end(), shader_vert_norms);

        //for fixed pipeline
        fixed_vert_norms = (GLfloat *) malloc(sizeof(GLfloat) * faces.size()*3);
        
        unsigned long num_norms=0;
        
        //calculate average norm for each vertex and move into array
        for(int i=0; i<avg_vert_norms.size(); i++)
        {
            num_norms = avg_vert_norms.at(i).size();
            for(int j=0; j<num_norms; j++)
            {
                average+= avg_vert_norms.at(i).at(j);
            }
            
            average /=num_norms;
            
//            average=normalize(average);
            
            avg_vert_norms.at(i).clear();
            avg_vert_norms.at(i).push_back(average);
            average={0,0,0};
        }
        
        for(int i =0; i<fixed_face_normals.size(); i++)
        {
            fixed_face_norms[i]=fixed_face_normals.at(i);
        }
        
        int index=0;
        for(int i=0; i<faces.size()*3; i+=3)
        {
            temp = avg_vert_norms.at(faces.at(index)).at(0);
            fixed_vert_norms[i] = temp.x;
            fixed_vert_norms[i+1] = temp.y;
            fixed_vert_norms[i+2] = temp.z;
            temp={0,0,0};
            index++;
        }
        
        index=0;
        for(int i=3; i<verts_spaced.size(); i+=6)
        {
            temp = avg_vert_norms.at(index).at(0);
            shader_vert_norms[i]=temp.x;
            shader_vert_norms[i+1]=temp.y;
            shader_vert_norms[i+2]=temp.z;
            temp={0,0,0};
            index++;
        }
    }
    
    void print_all()
    {
//        for(int i=0; i<faces.size()*3; i+=3)
//        {
//            printf("%f %f %f\n",norm_all_arr[i],norm_all_arr[i+1],norm_all_arr[i+2]);
//        }
//        
//        for(int i=0; i<faces.size(); i++)
//        {
//            printf("Faces: %f %f %f\n", faces.at(i), faces.at(i+1), faces.at(i+1));
//        }
        
//
//        printf("faces.size() %lu \n", faces.size());
//        
//        printf("verts.size() %lu \n", vertices.size());
//        
//        printf("verts norms list.size() %lu \n", vert_norms.size());
//        
//        printf("faces.size() %lu \n", face_normals.size());
//        
//        printf("vert spaced.size() %lu \n", verts_spaced.size());

    }
    
    
public:
    
    void calc_normals(vector<GLfloat> verts_all, vector<GLuint> faces_all, vector<GLfloat> spaced)
    {
        vertices=verts_all;
        faces=faces_all;
        verts_spaced=spaced;
        calc_all_face_normals();
        translate_normals();
//                        print_all();
    }
    
    void recalculate_normals(vector<GLfloat> verts_all, vector<GLuint> faces_all, vector<GLfloat> spaced)
    {
        vertices=verts_all;
        faces=faces_all;
        verts_spaced=spaced;
        calc_all_face_normals();
        translate_normals();
    }
    
    GLfloat* get_fixed_smooth_norms()
    {
        return fixed_vert_norms;
    }
    
    GLfloat* get_shader_smooth_norms()
    {
        return shader_vert_norms;
    }
    
    GLfloat* get_fixed_flat_norms()
    {
        return fixed_face_norms;
    }
    
    void free_all()
    {

        delete [] fixed_face_norms;
        delete [] fixed_vert_norms;
        delete [] shader_vert_norms;
    }
    
};
