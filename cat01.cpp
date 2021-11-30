#include <GL/glut.h> // (or others, depending on the system in use)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

vector<string> splitString(string s){
    vector<string> outp;
    string temp = "";
    for(int i=0;i<s.length();i++){
        if(s[i] == ' '){
            outp.push_back(temp);
            temp = "";
        }
        else{
            temp += s[i];
        }
    }
    if(temp != ""){
        outp.push_back(temp);
    }
    return outp;
}

void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void display() {   
    printf("In CircleIUA algorithm\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -10.0f);  // Move right and into the screen
    glScalef(0.3f,0.3f,0.3f);

    float xlight = -70;
	float ylight = 30;
	float zlight = 60;

     vector<float> l;		
            // light source coordinates wrt camera.
			l.push_back((xlight));
			l.push_back((ylight));
			l.push_back((zlight));

    ifstream model_file;
    model_file.open("PA2_Models/cat01.off");
    string text;
    getline(model_file, text);
    getline(model_file, text);
    vector<string> details = splitString(text);
    int numVertices = stoi(details[0]);
    int numFaces = stoi(details[1]);
    int numEdges = stoi(details[2]);
    vector<vector<string>> vertices(numVertices);
    for(int i=0;i<numVertices;i++){
        getline(model_file, text);
        vertices[i] = splitString(text);
    }
    vector<vector<string>> faces(numFaces);
    for(int i=0;i<numFaces;i++){
        getline(model_file, text);
        faces[i] = splitString(text);
    }
    glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 1.0f, 0.0f);
        for(int i=0;i<numFaces;i++){
            int v1 = stoi(faces[i][1]);
            int v2 = stoi(faces[i][2]);
            int v3 = stoi(faces[i][3]);
            glVertex3f(stof(vertices[v1][0])/10, stof(vertices[v1][1])/10, stof(vertices[v1][2])/10);
            glVertex3f(stof(vertices[v2][0])/10, stof(vertices[v2][1])/10, stof(vertices[v2][2])/10);
            glVertex3f(stof(vertices[v3][0])/10, stof(vertices[v3][1])/10, stof(vertices[v3][2])/10);
        }
    glEnd();
    glutSwapBuffers();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(500, 500);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow("PA 2");          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}