#include<GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

void display()
{
    glClear (GL_COLOR_BUFFER_BIT); // Clear display window.
    glColor3f (1.0, 1.0, 1.0); // Set point color to black.
    // glScalef(0.1f, 0.1f, 0.1f);
    
    ifstream myfile ("PA2_Models/cat01.off");
    float tempX ,tempY ,tempZ ;
    float projX, projY;
    int face_verti_1,face_verti_2,face_verti_3;
    vector<vector<float>> vertices; //Stores vertices
    vector<vector<float>> proj; //Stores projections of 3D vertices
    vector<vector<int>> faces; //Stores faces i.e. triangles
    int VERTEX_COUNT,FACE_COUNT,EDGE_COUNT;
    float left, right, bottom, top, near, far;
    float cX = -40, cY = -50, cZ = 10;//Camera co-ordinates
    string line;

    if (myfile.is_open()){
        getline (myfile,line);
        if(line == "OFF"){
            getline (myfile,line);
            sscanf(line.c_str(), "%d %d %d", &VERTEX_COUNT, &FACE_COUNT, &EDGE_COUNT);
            getline(myfile,line);
            sscanf(line.c_str(), "%f %f %f", &tempX, &tempY, &tempZ);
            tempX = tempX- cX;//xe - tx
            tempY = tempY- cY;//ye - ty
            tempZ = tempZ- cZ;//ze- - tz
            //tempX *= -1; tempY *= -1; tempZ *= -1;
            vector<float> a; a.push_back(tempX); a.push_back(tempY); a.push_back(tempZ);
            vertices.push_back(a);
            near = tempZ; far = near;
            for(int verti = 1; verti < VERTEX_COUNT; verti++){
                getline(myfile,line);
                sscanf(line.c_str(), "%f %f %f", &tempX, &tempY, &tempZ);
                tempX = tempX - cX;
                tempY = tempY - cY;
                tempZ = tempZ - cZ;
                vector<float> a; a.push_back(tempX); a.push_back(tempY); a.push_back(tempZ);
                vertices.push_back(a);
                near = near < tempZ ? near : tempZ;
                far = far < tempZ ? tempZ : far;
            }   
            tempX = vertices[0][0]; tempY = vertices[0][1]; tempZ = vertices[0][2];
            left = near*tempX/tempZ; right = left;
            bottom = near*tempY/tempZ; top = bottom;
            vector<float> b;
            b.push_back(left); b.push_back(bottom);
            proj.push_back(b);
            for(int verti = 1; verti < VERTEX_COUNT; verti++){
                tempX = vertices[verti][0]; tempY = vertices[verti][1]; tempZ = vertices[verti][2];
                projX = near*tempX/tempZ;
                projY = near*tempY/tempZ;
                left = left < projX ? left : projX;
                right = right < projX ? projX : right;
                bottom = bottom < projY ? bottom : projY;
                top = top < projY ? projY : top;
                vector<float> b; b.push_back(projX); b.push_back(projY);
                proj.push_back(b);
            }
            cout<<"The values of near, far, left, right, bottom, top respectively are :\n";
            cout<<near<<", "<<far<<", "<<left<<", "<<right<<", "<<bottom<<", "<<top<< "\n";
            for(int faci = 0; faci < FACE_COUNT; faci++){
                getline(myfile,line);
                sscanf(line.c_str(), "3 %d %d %d", &face_verti_1, &face_verti_2, &face_verti_3);
                vector<int> a; a.push_back(face_verti_1); a.push_back(face_verti_2); a.push_back(face_verti_3);
                faces.push_back(a);
            }
        }
        myfile.close();
    } 
    else 
    {
        cout << "File Not Found. \n";
    }
    glBegin(GL_TRIANGLES);
    for(int faci = 0; faci < 42000  ; faci++){
    int face_verti_1 = faces[faci][0]; int face_verti_2 = faces[faci][1]; int face_verti_3 = faces[faci][2];
    GLfloat t1X = proj[face_verti_1][0]; GLfloat t1Y = proj[face_verti_1][1];
    GLfloat t2X = proj[face_verti_2][0]; GLfloat t2Y = proj[face_verti_2][1];
    GLfloat t3X = proj[face_verti_3][0]; GLfloat t3Y = proj[face_verti_3][1];
    glVertex2f(t1X, t1Y);
    glVertex2f(t2X, t2Y);
    glVertex2f(t3X, t3Y);
}

glEnd();
glFlush();
}


int main (int argc, char** argv)
{
    glutInit (&argc, argv); // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // Set display mode.
    glutInitWindowPosition (0, 0); // Set top-left display-window position.
    glutInitWindowSize (500, 500); // Set display-window width and height.
    
    glutCreateWindow ("PA2"); // Create display window.
    
    glClearColor (0.0, 0.0, 0.0, 0.0); // display window color is black.
    glMatrixMode (GL_PROJECTION); // projection parameters.
    glLoadIdentity();
    //gluOrtho2D (-100.0, 0.0, -100.0, 0.0);
    gluOrtho2D (-10.0, 0.0, -20.0, 0.0);

    glutDisplayFunc(display);
    glutMainLoop ( ); // Display everything and wait.
    return 0;
}