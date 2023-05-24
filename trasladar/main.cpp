#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

int width = 2000;
int height = 1000;
std::vector<float> vertices;
std::vector<int> faces;
std::vector<float> vertices2;
std::vector<int> faces2;

GLfloat angulo = 45.0f;

float t = 0.0;

void readObj(const char* filename, vector<float>& vertex, vector<int>& face) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == nullptr) {
        std::cerr << "No se puede abrir el archivo: " << filename << std::endl;
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            float x = 0;
            float y = 0;
            float z = 0;

            int numRead = sscanf_s(line + 2, "%f %f %f %f", &x, &y, &z);
            if (numRead == 3) {
                vertex.push_back(x);
                vertex.push_back(y);
                vertex.push_back(z);
            }
        }
        else if (strncmp(line, "f ", 2) == 0) {
            int v1 = 0, v2 = 0, v3 = 0;

            int numRead = sscanf_s(line + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3);

            if (numRead == 3) {
                face.push_back(v1);
                face.push_back(v2);
                face.push_back(v3);
            }
        }
    }
    fclose(file);
}

void init(void)
{
    glViewport(0, 0, width, height);
    //  select clearing (background) color Color de borrado  
    glClearColor(0.0, 0.0, 0.0, 0.0);

    //  initialize viewing values  Matriz de proyeccion
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //que la haga uno

    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); //Hace una matriz de la proyeccion ortogonal
    gluOrtho2D(0.0, 0.0, (GLfloat)width, (GLfloat)height);

    gluPerspective(45.0, width / height, 0.1, 100.0); 

    gluLookAt(15.0, 3.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

// Definimos los puntos de control de la curva de Bezier
float controlPoints[4][3] = {
    {-5.0, -1.0, 1.0},
    {-2.0, -5.0, 1.0},
    {2.0, -5.0, 1.0},
    {5.0,  0.0, 1.0}
};



void updatePosition(float t) {
    float x, y, z;

    // Calcular la posición en la curva de Bezier
    x = pow(1 - t, 3) * controlPoints[0][0] + 3  * pow(1 - t, 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
    y = pow(1 - t, 3) * controlPoints[0][1] + 3  * pow(1 - t, 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
    z = pow(1 - t, 3) * controlPoints[0][2] + 3  * pow(1 - t, 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

    // Mover el modelo a la posición calculada
    glTranslatef(x, y, z);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //funcion para calcular la curva de bezier
    updatePosition(t);

    t += 0.1;
    if (t > 1.0) {
        t = 0.0;
    }

    // Draw the model
    glColor3f(0.6, 0.0, 1.0);

    //matriz para girar en el angulo z 
    GLfloat rotationMatrix[16] = {
        cos(angulo), -sin(angulo), 0.0f, 0.0f,
        sin(angulo), cos(angulo), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glMultMatrixf(rotationMatrix);

    angulo += 0.1f;

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < faces2.size(); i += 3) {

        int v1 = faces2[i] - 1;
        int v2 = faces2[i + 1] - 1;
        int v3 = faces2[i + 2] - 1;

        glVertex3f(vertices2[v1 * 3], vertices2[v1 * 3 + 1], vertices2[v1 * 3 + 2]);
        glVertex3f(vertices2[v2 * 3], vertices2[v2 * 3 + 1], vertices2[v2 * 3 + 2]);
        glVertex3f(vertices2[v3 * 3], vertices2[v3 * 3 + 1], vertices2[v3 * 3 + 2]);
    }
    glEnd();
    //segundo modelo 
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < faces.size(); i += 3) {

        int v1 = faces[i] - 1;
        int v2 = faces[i + 1] - 1;
        int v3 = faces[i + 2] - 1;


        glVertex3f(vertices[v1 * 3], vertices[v1 * 3 + 1], vertices[v1 * 3 + 2]);
        glVertex3f(vertices[v2 * 3], vertices[v2 * 3 + 1], vertices[v2 * 3 + 2]);
        glVertex3f(vertices[v3 * 3], vertices[v3 * 3 + 1], vertices[v3 * 3 + 2]);
    }
    glEnd();
    glutSwapBuffers();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Ejemplo");

    readObj("Cubo.obj", vertices, faces);
    readObj("dona.obj", vertices2, faces2);
    init();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();

    return 0;
}