#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

int width = 1000;
int height = 600;
std::vector<float> vertices;
std::vector<int> faces;

void readObj(const char* filename) {
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

            int numRead = sscanf_s(line + 2, "%f %f %f", &x, &y, &z);
            if (numRead == 3) {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
        else if (strncmp(line, "f ", 2) == 0) {
            int v1 = 0, v2 = 0, v3 = 0;

            int numRead = sscanf_s(line + 2, "%d %d %d", &v1, &v2, &v3);
            if (numRead == 3) {
                // OpenGL indices start from 0, but OBJ indices start from 1
                // so we need to subtract 1 from each index
                int idx1 = (v1 - 1) * 3;
                int idx2 = (v2 - 1) * 3;
                int idx3 = (v3 - 1) * 3;

                faces.push_back(idx1);
                faces.push_back(idx2);
                faces.push_back(idx3);
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(15.0, 3.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.6, 0.0, 1.0);

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
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Ejemplo");

    readObj("dona.obj");
    init();

    std::vector<int> tempFaces;
    for (int i = 0; i < vertices.size() / 3; i++) {
        tempFaces.push_back(i + 1);
    }

    faces = tempFaces;

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}