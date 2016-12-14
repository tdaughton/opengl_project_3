// /**********************/
///* Nathaniel Gonzales */
///* CS412 Project 2    */
///**********************/
//
//#include <stdio.h>
//#include <string.h>
//#include <iostream>
//#include <vector>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include "glm/glm.hpp"
////#include <glm/gtc/type_ptr.hpp>
////#include <glm/gtc/matrix_access.hpp>
//
//#include "normals.cpp" 
//#include "read.cpp"
//
////obj file reader object
//Reader reader(0.0,0.0,1.0);
//
////normal calculator
//Normals normal;
//
//char* path = "bunny.obj";
//
//glm::vec3 defaultCameraPosition = { -3,0,0 };
//glm::vec3 defaultCameraFront = { 0.0f, 0.0f, 0.0 };
//glm::vec3 defaultCameraUp = { 0.0f, 1.0f, 0.0f };
//glm::vec3 cameraPosition = defaultCameraPosition;
//glm::vec3 cameraFront = defaultCameraFront;
//glm::vec3 cameraUp = defaultCameraUp;
//
//bool flatShading = false;
//
//
//GLfloat color[3] = { .5,.75,.85 };
//
//
//
//using namespace std;
//vector<unsigned int > vertexIndices, uvIndices, normalIndices;
//vector<vector<GLfloat>> verticies;
//vector<GLfloat> all;
//vector<GLfloat> tempVerts;
//vector<glm::vec3> normalFaces;
//vector<vector<glm::vec3>> normalVerticies;
//GLfloat* verts;
//GLuint* indices2;
//
//int vertSize;
//int indicesSize;
//
//void initBufferObject(void);
//void setShaders();
//void initLighting();
//void smoothToFlat();
//void flatToSmooth();
//GLfloat* normalVertArray;
//GLfloat* normalFaceArray;
//
//float rotationAmount = 0.0174533;
////Rotates Camera by .08
//
//
////Reads in an obj file
//bool readFile(char* path) {
//    GLfloat maxVertex = 0;
//    FILE * file = fopen(path, "r");
//    vector<GLuint> tempIndices;
//    vertSize = 0;
//    indicesSize = 0;
//    if (file == NULL) {
//        printf("Impossible to open the file !\n");
//        return false;
//    }
//    printf(path); printf("\n");
//    char a[128];
//    char b[128];
//    char c[128];
//    char lineHeader[128];
//    std::string::size_type sz;
//    std::vector<GLfloat> vertex;
//    bool flag = true;
//    while (1) {
//        int res = fscanf(file, "%s", lineHeader);
//        if (res == EOF)
//            break;
//        if (strcmp(lineHeader, "v") == 0) {
//            int res = fscanf(file, "%s %s %s\n", a, b, c);
//            vertex.clear();
//            vertex = { std::stof(a, &sz),std::stof(b, &sz),std::stof(c, &sz) };
//            verticies.push_back(vertex);
//            tempVerts.push_back(vertex.at(0));
//            tempVerts.push_back(vertex.at(1));
//            tempVerts.push_back(vertex.at(2));
//            //tempVerts.push_back(color[0]);
//            //tempVerts.push_back(color[1]);
//            //tempVerts.push_back(color[2]);
//            tempVerts.push_back(0);
//            tempVerts.push_back(0);
//            tempVerts.push_back(0);
//            vertSize++;
//            if (vertex.at(0) > maxVertex) maxVertex = vertex.at(0);
//            if (vertex.at(1) > maxVertex) maxVertex = vertex.at(1);
//            if (vertex.at(2) > maxVertex) maxVertex = vertex.at(2);
//        }
//        else if (strcmp(lineHeader, "f") == 0) {
//            if (flag) normalVerticies.resize(verticies.size());
//            flag = false;
//            int res = fscanf(file, "%s %s %s\n", a, b, c);
//            int ix = std::stoi(a, &sz) - 1;
//            int iy = std::stoi(b, &sz) - 1;
//            int iz = std::stoi(c, &sz) - 1;
//            vector < GLfloat > fx = verticies.at(ix);
//            vector < GLfloat > fy = verticies.at(iy);
//            vector < GLfloat > fz = verticies.at(iz);
//            glm::vec3 x = { fx.at(0),fx.at(1),fx.at(2) };
//            glm::vec3 y = { fy.at(0),fy.at(1),fy.at(2) };
//            glm::vec3 z = { fz.at(0),fz.at(1),fz.at(2) };
//            glm::vec3 normal = glm::normalize(glm::cross((y - x), (z - x)));
//            
//            normalVerticies.at(ix).push_back(normal);
//            normalVerticies.at(iy).push_back(normal);
//            normalVerticies.at(iz).push_back(normal);
//            normalFaces.push_back(normal);
//            normalFaces.push_back(normal);
//            normalFaces.push_back(normal);
//            
//            tempIndices.push_back(ix);
//            tempIndices.push_back(iy);
//            tempIndices.push_back(iz);
//            all.push_back(x.x);
//            all.push_back(x.y);
//            all.push_back(x.z);
//            all.push_back(y.x);
//            all.push_back(y.y);
//            all.push_back(y.z);
//            all.push_back(z.x);
//            all.push_back(z.y);
//            all.push_back(z.z);
//            
//            indicesSize++;
//        }
//    }
//    indices2 = new GLuint[tempIndices.size()];
//    verts = new GLfloat[tempVerts.size()];
//    for (int i = 0; i < tempVerts.size(); i++) { verts[i] = tempVerts.at(i); }
//    for (int i = 0; i < tempIndices.size(); i++) { indices2[i] = tempIndices.at(i); }
//    
//    defaultCameraPosition = { -3 * maxVertex,0,0 };
//    cameraPosition = defaultCameraPosition;
//    vertSize *= 24;
//    indicesSize *= 12;
//    return false;
//}
//
//GLfloat *mapVerts;
//void arrayify() {
//    printf("size= %d\n", all.size());
////    ofstream myfile;
////    myfile.open("example.txt");
//    
//    mapVerts = (GLfloat *)malloc(sizeof(GLfloat) * 3*normalVerticies.size());
//    normalVertArray = (GLfloat *)malloc(sizeof(GLfloat) * all.size());
//    normalFaceArray = (GLfloat *)malloc(sizeof(GLfloat) * all.size());
//    glm::vec3 total;
//    for (int i = 0; i < normalVerticies.size(); i++) {
//        total = { 0,0,0 };
//        for (int j = 0; j < normalVerticies.at(i).size();j++) {
//            total += normalVerticies.at(i).at(j);
//        }
//        total /= (normalVerticies.at(i).size());
//        mapVerts[i * 3] = total.x;
//        mapVerts[i * 3+1] = total.y;
//        mapVerts[i * 3+2] = total.z;
//        //if(i<50)printf("%f %f,%f @%d\n", total.x, total.y, total.z,i);
//    }
//    
//    for (int i = 0; i < all.size()/3; i++) {
//        for (int j = 0; j < 3; j++) {
//            //if(i<12)printf("%f ",mapVerts[3 * indices2[i] + j]);
//            //myfile <<  mapVerts[3 * indices2[i] + j];
//            //myfile << " ";
//            
//            normalVertArray[i*3 + j] = mapVerts[3 * indices2[i] + j];
//        }
//        //myfile << "\n";
//        //	printf("\n");
//    }
//    //printf("all=%d,%d\n", all.size(),sizeof(normalVertArray));
//    
//    for (int i = 0; i < normalFaces.size(); i++) {
//        normalFaceArray[i*3] = normalFaces.at(i).x;
//        normalFaceArray[i*3+1] = normalFaces.at(i).y;
//        normalFaceArray[i*3+2] = normalFaces.at(i).z;
//    }
//        if (flatShading) smoothToFlat();
//        else flatToSmooth();
//        
//    }
//    
//    void flatToSmooth() {
//        for (int i = 0; i < vertSize / 4; i += 6) {
//            verts[i + 3] = mapVerts[i / 2];
//            verts[i + 4] = mapVerts[(i / 2) + 1];
//            verts[i + 5] = mapVerts[(i / 2) + 2];
//        }
//    }
//    
//    void smoothToFlat() {
//        for (int i = 0; i < vertSize / 4; i += 6) {
//            verts[i + 3] = normalFaceArray[i/2];
//            verts[i + 4] = normalFaceArray[(i / 2)+1];
//            verts[i + 5] = normalFaceArray[(i / 2)+2];
//        }
//    }
//
//
//void initBufferObject(void)
//{
//   
//    printf("vert size %d\n", vertSize);
//    printf("indices size %d\n", indicesSize);
//    
//}
//
//void compare(GLuint* vert1, GLuint* vert2)
//{
//    for(int i=0; i<indicesSize/4; i++)
//    {
////        printf("1 @%i %lu \n", i, vert1[i]);
////        printf("2 @%i %lu \n", i, vert2[i]);
//        
//        if(vert1[i]!=vert2[i])
//        {
//            printf("@%i %lu != %lu \n", i, vert1[i], vert2[i]);
//        }
////        else
////        {
////            printf("@%i %f == %f\n", i, vert1[i], vert2[i]);
////        }
//    }
//}
//
//int main(int argc, char **argv) {
//    readFile(path);
//    arrayify();
//    initBufferObject();
//    
////    GLfloat* coord_all_arr = reader.get_coord_array();
//    GLuint* faces_arr = reader.get_face_array();
////    camera_position=reader.get_camera_position();
////    camera_front = glm::normalize(FOCUS_POINT-camera_position);
////    translate(0,0,4);
//    
//    normal.calc_normals(reader.get_vertices(), reader.get_faces(), reader.get_spaced_vertices());
//    GLfloat* vertices_arr = normal.get_vert_all();
////    all_norms_arr=normal.get_all_norms();
//    
//    printf("face size: %d\n", indicesSize);
//
//    //    compare(vertices_arr, verts);
//    GLfloat* norms = normal.get_all_norms();
//    compare(faces_arr, indices2);
//    
//}
