#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// Pilihan Proyeksi
enum ProjectionType {
    // Proyeksi Orthographic
    PLAN_VIEW,      // Top view
    FRONT_VIEW,     // Front view
    SIDE_VIEW,      // Side view
    ISOMETRIC,      // Isometric view
    DIMETRIC,       // Dimetric view
    TRIMETRIC,      // Trimetric view
    
    // Proyeksi Perspective
    ONE_POINT,      // 1 titik hilang
    TWO_POINT,      // 2 titik hilang
    THREE_POINT     // 3 titik hilang
};

enum ProjectionType currentProjection = ISOMETRIC; // default 

// Array untuk menyimpan nama proyeksi
const char* projectionNames[] = {
    "Plan View",
    "Front View",
    "Side View",
    "Isometric View",
    "Dimetric View",
    "Trimetric View",
    "One-Point Perspective",
    "Two-Point Perspective",
    "Three-Point Perspective"
};

// Warna kardus
GLfloat cardboardColor[] = {0.82f, 0.71f, 0.55f, 1.0f};  // Warna coklat kardus
GLfloat darkCardboardColor[] = {0.76f, 0.66f, 0.5f, 1.0f};  // Warna coklat sedikit lebih gelap untuk tutup kardus
GLfloat edgeColor[] = {0.5f, 0.4f, 0.3f, 1.0f};  // Warna lebih gelap untuk tepi kardus

// Fungsi untuk menggambar tepi/outline kubus
void drawCubeEdges(float size) {
    glColor3fv(edgeColor);
    glLineWidth(2.0f);  // Ketebalan garis
    
    glBegin(GL_LINE_LOOP);  // Sisi bawah
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(size/2, -size/2, -size/2);
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(-size/2, -size/2, size/2);
    glEnd();
    
    glBegin(GL_LINE_LOOP);  // Sisi atas
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
    glEnd();
    
    glBegin(GL_LINES);  // Menghubungkan sisi atas dan bawah
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(-size/2, size/2, -size/2);
        
        glVertex3f(size/2, -size/2, -size/2);
        glVertex3f(size/2, size/2, -size/2);
        
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(size/2, size/2, size/2);
        
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
    glEnd();
}

// Fungsi untuk membuat dasar dari kubus untuk kardus 
void drawCardboardBase() {
    // Alas kardus
    glColor3fv(cardboardColor);
    glPushMatrix();
    glScalef(1.0f, 0.5f, 1.0f);  // Ukuran kardus
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Tepi alas
    glPushMatrix();
    glScalef(1.0f, 0.5f, 1.0f);
    drawCubeEdges(1.0f);
    glPopMatrix();
}

// Fungsi untuk membuat tutup kardus
void drawCardboardLid() {
    // Tutup kardus
    glColor3fv(darkCardboardColor);
    glPushMatrix();
    glTranslatef(0.0f, 0.31f, 0.0f);
    glScalef(1.05f, 0.1f, 1.05f);  // Ukuran tutup sedikit lebih besar dan lebih tipis
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Tepi tutup
    glPushMatrix();
    glTranslatef(0.0f, 0.31f, 0.0f);
    glScalef(1.05f, 0.1f, 1.05f);
    drawCubeEdges(1.0f);
    glPopMatrix();
}

void drawBottle(float xOffset, float zOffset) {
    GLUquadric *quad = gluNewQuadric();
    glPushMatrix();

    // Posisi botol di atas kardus
    glTranslatef(xOffset, 0.38f, zOffset);

    // Rotasi botol agar bediri
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // Warna body botol
    GLfloat bodyColor[] = {0.95f, 0.95f, 0.95f, 1.0f};
    glColor4fv(bodyColor);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float radius = 0.07f;
    float bodyHeight = 0.3f;
    float neckHeight = 0.05f;
    float capHeight = 0.05f;

    // Alas botol
    glPushMatrix();
    glutSolidSphere(radius, 32, 32);
    glPopMatrix();

    // Badan botol
    glPushMatrix();
    gluCylinder(quad, radius, radius, bodyHeight, 32, 32);
    glPopMatrix();

    // Leher botol
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, bodyHeight);
    gluCylinder(quad, 0.035f, 0.035f, neckHeight, 32, 32);
    glPopMatrix();

    // Tutup botol
    glColor3f(0.7f, 0.8f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, bodyHeight + neckHeight);
    gluCylinder(quad, 0.04f, 0.04f, capHeight, 32, 32);

    // Tutup bagian atas
    glTranslatef(0.0f, 0.0f, capHeight);
    gluDisk(quad, 0.0f, 0.04f, 32, 1);
    glPopMatrix();

    glDisable(GL_BLEND);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawCardbox() {
    drawCardboardBase();
    drawCardboardLid();

    // 4 botol dengan formasi 2x2 di atas kardus
    float bottleSpacing = 0.12f;

    // Baris depan
    drawBottle(-bottleSpacing, -bottleSpacing);  // kiri depan
    drawBottle(bottleSpacing, -bottleSpacing);   // kanan depan

    // Baris belakang
    drawBottle(-bottleSpacing, bottleSpacing);   // kiri belakang
    drawBottle(bottleSpacing, bottleSpacing);    // kanan belakang
}

// Implementasi proyeksi orthographic
void setOrthographicProjection(int width, int height) {
    float aspect = (float)width / (float)height;
    float size = 3.0f;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (aspect >= 1.0f) {
        glOrtho(-size * aspect, size * aspect, -size, size, 0.1f, 100.0f);
    } else {
        glOrtho(-size, size, -size / aspect, size / aspect, 0.1f, 100.0f);
    }
}

// Implementasi proyeksi perspektif
void setPerspectiveProjection(int width, int height) {
    float aspect = (float)width / (float)height;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Implementasi manual perspektif
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    
    // Menghitung matrix proyeksi perspektif
    float f = 1.0f / tan(fov * M_PI / 360.0f);
    
    float perspective[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farPlane+nearPlane)/(nearPlane-farPlane), -1,
        0, 0, (2*farPlane*nearPlane)/(nearPlane-farPlane), 0
    };
    
    glMultMatrixf(perspective);
}

// Setup kamera berdasarkan jenis proyeksi
void setupProjectionAndCamera() {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    // Set proyeksi berdasarkan jenis
    if (currentProjection <= TRIMETRIC) {
        setOrthographicProjection(width, height);
    } else {
        setPerspectiveProjection(width, height);
    }
    
    // Reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Case untuk posisi kamera berdasarkan jenis proyeksi
    switch (currentProjection) {
        // Proyeksi Orthographic
        case PLAN_VIEW:  // Top view 
            gluLookAt(0.0f, 5.0f, 0.0f,    
                      0.0f, 0.0f, 0.0f,    
                      0.0f, 0.0f, -1.0f);  
            break;
            
        case FRONT_VIEW:  // Front view
            gluLookAt(0.0f, 0.0f, 5.0f,    
                      0.0f, 0.0f, 0.0f,    
                      0.0f, 1.0f, 0.0f);   
            break;
            
        case SIDE_VIEW:  // Side view
            gluLookAt(5.0f, 0.0f, 0.0f,    
                      0.0f, 0.0f, 0.0f,    
                      0.0f, 1.0f, 0.0f);   
            break;
            
        case ISOMETRIC:  // Isometric view 
            gluLookAt(3.464f, 3.464f, 3.464f,  
                      0.0f, 0.0f, 0.0f,        
                      0.0f, 1.0f, 0.0f);       
            break;
            
        case DIMETRIC:  // Dimetric view
            gluLookAt(4.0f, 2.5f, 4.0f,   
                      0.0f, 0.0f, 0.0f,   
                      0.0f, 1.0f, 0.0f);  
            break;
            
        case TRIMETRIC:  // Trimetric view
            gluLookAt(3.5f, 2.0f, 4.5f,   
                      0.0f, 0.0f, 0.0f,   
                      0.0f, 1.0f, 0.0f);  
            break;
            
        // Proyeksi Perspective 
        case ONE_POINT:  // satu titik hilang
            gluLookAt(0.0f, 0.0f, 5.0f,    
                      0.0f, 0.0f, 0.0f,    
                      0.0f, 1.0f, 0.0f);   
            break;
            
        case TWO_POINT:  // dua titik hilang
            gluLookAt(4.0f, 0.0f, 4.0f,    
                      0.0f, 0.0f, 0.0f,    
                      0.0f, 1.0f, 0.0f);   
            break;
            
        case THREE_POINT: // Tiga titik hilang
            gluLookAt(4.0f, 2.5f, 4.0f,    
                      0.0f, 0.0f, 0.0f,    
                      0.0f, 1.0f, 0.3f);   
            break;
    }
}

// Fungsi untuk menampilkan panduan di layar
void drawText(float x, float y, const char *text) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    glOrtho(0, w, 0, h, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(0.0f, 0.0f, 0.0f); // warna text
    glRasterPos2f(x, y);
    
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
}

// Fungsi untuk menampilkan informasi proyeksi dan tombol
void displayInfo() {
    int y_pos = 580;  // Posisi awal y untuk teks
    int line_height = 20;  // Jarak antar baris
    
    char projInfo[100];
    sprintf(projInfo, "Current Projection: %s", projectionNames[currentProjection]);
    
    // Tampilkan info proyeksi saat ini
    drawText(10, y_pos, projInfo);
    y_pos -= line_height;
    
    // Header untuk panduan tombol
    drawText(10, y_pos, "Keyboard Controls:");
    y_pos -= line_height;
    
    // Tampilkan panduan tombol secara vertikal
    drawText(10, y_pos, "1 : Plan View");
    y_pos -= line_height;
    
    drawText(10, y_pos, "2 : Front View");
    y_pos -= line_height;
    
    drawText(10, y_pos, "3 : Side View");
    y_pos -= line_height;
    
    drawText(10, y_pos, "4 : Isometric View");
    y_pos -= line_height;
    
    drawText(10, y_pos, "5 : Dimetric View");
    y_pos -= line_height;
    
    drawText(10, y_pos, "6 : Trimetric View");
    y_pos -= line_height;
    
    drawText(10, y_pos, "7 : 1 Titik Hilang");
    y_pos -= line_height;
    
    drawText(10, y_pos, "8 : 2 Titik Hilang");
    y_pos -= line_height;
    
    drawText(10, y_pos, "9 : 3 Titik Hilang");
    y_pos -= line_height;
    
    drawText(10, y_pos, "ESC : Keluar");
}

// Fungsi untuk menampilkan scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set proyeksi dan posisi kamera
    setupProjectionAndCamera();
    
    // Gambar grid untuk referensi
    glLineWidth(1.5f);
    glBegin(GL_LINES);
        // X axis (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(2.0f, 0.0f, 0.0f);
        
        // Y axis (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 2.0f, 0.0f);
        
        // Z axis (blue)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();
    
    // Gambar objek kardus dan 4 botol minum
    drawCardbox();
    
    // Tampilkan informasi proyeksi dan tombol
    displayInfo();
    
    glutSwapBuffers();
}

// Fungsi untuk mengubah ukuran window
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    setupProjectionAndCamera();
}

// Fungsi untuk setiap case keyboard yang ditekan oleh user
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: // ESC
            exit(0);
            break;
            
        // Proyeksi Orthographic
        case '1':
            currentProjection = PLAN_VIEW;
            break;
        case '2':
            currentProjection = FRONT_VIEW;
            break;
        case '3':
            currentProjection = SIDE_VIEW;
            break;
        case '4':
            currentProjection = ISOMETRIC;
            break;
        case '5':
            currentProjection = DIMETRIC;
            break;
        case '6':
            currentProjection = TRIMETRIC;
            break;
            
        // Proyeksi Perspective 
        case '7':
            currentProjection = ONE_POINT;
            break;
        case '8':
            currentProjection = TWO_POINT;
            break;
        case '9':
            currentProjection = THREE_POINT;
            break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Background putih untuk scene
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
}

// Main program
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Objek 3D Kardus dengan 4 botol diatasnya");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}
