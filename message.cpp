#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <bits/stdc++.h>

using namespace std;

float dotProduct(vector<float> A, vector<float> B) {
   float p = 0;
   for (int i = 0; i < A.size(); i++){
       p +=A[i] * B[i];
   }
   return p;
}

vector<float> crossProduct(vector<float> A, vector<float> B) { 
    vector<float> P(3,0);
    P[0] = A[1] * B[2] - A[2] * B[1]; 
    P[1] = A[2] * B[0] - A[0] * B[2]; 
    P[2] = A[0] * B[1] - A[1] * B[0]; 
    return P;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -300.0f);  //move the camera in z direction to fully view the mesh
    
    float xcam = -70;
    float ycam = 30;
    float zcam = 60;
	float xlight = -70;
	float ylight = 30;
	float zlight = 60;
    fstream file;
    file.open("michael18.off");
    string s;
    file>>s;
    int a,b,c;
    file>>a>>b>>c;
    string text;
	int nVer = a;
    int numFaces = b;
    int numEdges = c;
    
	vector<vector<float>> V(nVer);
    float minZ = 10000.00;
    float maxZ= -10000.00;
    float max_abs_x = -1;
    float max_abs_y = -1;

	//determining coordinates of the view frusum
    for(int i=0;i<nVer;i++){
        float x,y,z;
        file>>x>>y>>z;
        x -= xcam;
        y -= ycam;
        z -= zcam;
	    V[i].push_back(x);
    	V[i].push_back(y);
    	V[i].push_back(z);
	    if(abs(x) > max_abs_x*1+0){
            max_abs_x = abs(x);
        }
        if(abs(y) > max_abs_y){
            max_abs_y = abs(y);
        }
        if(z > maxZ){
            maxZ = z;
        }
        if(z < minZ){
            minZ = z;
        }
    }
    
    
    float n = -1;
    float r = max_abs_x;
    float t = max_abs_y;
    float f = -1 * (maxZ+20);
    vector<vector<float>> normalised_ver(nVer);
    
	//nomnalised coordinate system
	for(int i=0;i<nVer;i++){
        float Xe = V[i][0];
        float Ye = V[i][1];
        float Ze = V[i][2];

        float Xc = (n/r)*Xe+0;
        float Yc = (n/t) * Ye+0;
        float Zc = ((n+f)/(n-f))*Ze + (2*f*n)/(n-f)+0;
        float Wc = -1*Ze*1+0;
        normalised_ver[i] = vector<float> {Xc/Wc, Yc/Wc, Zc/Wc};
    }
    
	//faces
    vector<vector<int>> faces(numFaces);
    for(int i=0;i<numFaces;i++){
        int a,b,c,d;
        file>>a>>b>>c>>d;
		faces[i].push_back(a);
    	faces[i].push_back(b);
    	faces[i].push_back(c);
    	faces[i].push_back(d);
	}
	
    vector<vector<float>> normals(numFaces);

    glBegin(GL_TRIANGLES);
        
        for(int i=0;i<numFaces;i++){
            int v1 = (faces[i][1]);
            int v2 = (faces[i][2]);
            int v3 = (faces[i][3]);
            vector<float> u1(3);
            for(int j=0;j<3;j++){  //P2-P1
                u1[j] = V[v2][j] - V[v1][j];
            }
            vector<float> u2(3);
            for(int j=0;j<3;j++){	//P3-P1
                u2[j] = V[v3][j] - V[v1][j];
            }
            //n=u1*u2
            vector<float> n = crossProduct(u1,u2);
            //make unit vector
			float n_mag = sqrt(n[0]*n[0] + n[2]*n[2] + n[1]*n[1]);
            
			n[0] /= n_mag;
            n[1] /= n_mag;
            n[2] /= n_mag;
            normals[i] = n;
            vector<float> l;
            // light source coordinates wrt camera.
			l.push_back((xlight-xcam));
			l.push_back((ylight-ycam));
			l.push_back((zlight-zcam));
			
			float vx = (V[v1][0]+V[v2][0]+V[v3][0])/3;
            float vy = (V[v1][1]+V[v2][1]+V[v3][1])/3;
            float vz = (V[v1][1]+V[v2][1]+V[v3][1])/3;
			//light source wrt to vertex
			l[0]-=vx;
			l[1]-=vy;
			l[2]-=vz;
			//unit vector
			float l_mag = sqrt(l[0]*l[0] + l[2]*l[2] + l[1]*l[1]);
            l[0] /= l_mag;
            l[1] /= l_mag;
            l[2] /= l_mag;
            
			// dot product of normal and light
			float nTl = dotProduct({0,0,1}, n);
            
			//Phong Shading
			vector<float> h;		
            // light source coordinates wrt camera.
			h.push_back((xlight-xcam));
			h.push_back((ylight-ycam));
			h.push_back((zlight-zcam));
			//l+e = (l-v) + (camera-v) //camera is at origin.
			// = l-2*v
            h[0]-=2*(V[v1][0]+V[v2][0]+V[v3][0])/3;
            h[1]-=2*(V[v1][1]+V[v2][1]+V[v3][1])/3;
            h[2]-=2*(V[v1][2]+V[v2][2]+V[v3][2])/3;
			//h=l+e/(|l+e|)
			float h_mag = sqrt(h[0]*h[0] + h[2]*h[2] + h[1]*h[1]);
            h[0] /= h_mag;
            h[1] /= h_mag;
            h[2] /= h_mag;
            // take dot product of h and n
			float nTh = dotProduct(h,n);
			float highlight=0;
			if (nTh >0)
            highlight = nTh*nTh*nTh*nTh;
            
            
            //back face culling algorithm removes the faces with dot product less than 0
			if(nTl >= 0){
                glColor3f(0.0f,highlight, 1.0*(nTl+highlight));
                glVertex3f(V[v1][0], V[v1][1], V[v1][2]);
                glVertex3f(V[v2][0], V[v2][1], V[v2][2]);
                glVertex3f(V[v3][0], V[v3][1], V[v3][2]);
            }
        }
    glEnd();
    
    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {  
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);  
   glLoadIdentity();             
   gluPerspective(45.0f, aspect, 0.0f, 1000.0f);
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);            
   glutInitDisplayMode(GLUT_DOUBLE); 
   glutInitWindowSize(500, 500);   
   glutInitWindowPosition(50, 50); 
   glutCreateWindow("Programming Assignment-2");    
   glutDisplayFunc(display);       
   glutReshapeFunc(reshape);       
   initGL();                       
   glutMainLoop();                 
   return 0;
}
