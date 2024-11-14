#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

using namespace std;

// Rocket position and rotation
float rocketY = -0.8f;
float rocketX = -0.9f;
float rocketRotation = 0.0f;
float cloudX = 0.5f;
float buildingScale = 1.0f; // Declare this variable
float sunScale = 1.0f;      // Scale for the sun
float sunRotation = 0.0f;
bool isMousePressed = false;
bool isRightMousePressed = false;

// Vertex Buffer Object
GLuint vbo;

void drawBackground() {
    // Draw a simple background with gradient
    glBegin(GL_QUADS);
    // Sky
    glColor3f(0.2f, 0.6f, 1.0f); // Light blue
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 0.5f); // Dark blue
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

void drawAirplane() {
    glPushMatrix();

    // Translate and rotate the airplane
    glTranslatef(rocketX, rocketY, 0.0f);
    glRotatef(rocketRotation, 0.0f, 0.0f, 1.0f);

    // Scale down the airplane
    glScalef(0.5f, 0.5f, 1.0f); // Reduce the size to 50%

    // Draw the body (main fuselage)
    glColor3f(1.0f, 0.8f, 0.0f); // Golden yellow
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.05f);  // Bottom-left
    glVertex2f(0.2f, -0.05f);   // Bottom-right
    glVertex2f(0.2f, 0.05f);    // Top-right
    glVertex2f(-0.2f, 0.05f);   // Top-left
    glEnd();

    // Draw the cockpit (semi-circle)
    glColor3f(1.0f, 0.9f, 0.6f); // Light gold
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(-0.1f + 0.1f * cos(angle), 0.05f + 0.05f * sin(angle));
    }
    glEnd();

    // Draw the tail (vertical stabilizer)
    glColor3f(1.0f, 0.8f, 0.0f); // Golden yellow
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.0f);     // Bottom-left
    glVertex2f(-0.15f, 0.0f);    // Bottom-right
    glVertex2f(-0.15f, 0.1f);    // Top-right
    glVertex2f(-0.2f, 0.1f);     // Top-left
    glEnd();

    // Draw the wings (move to the tail)
    glColor3f(1.0f, 0.7f, 0.0f); // Orange
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, 0.0f);    // Base-center (left wing)
    glVertex2f(-0.3f, -0.15f);   // Bottom-left
    glVertex2f(-0.15f, -0.05f);  // Near the tail
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, 0.0f);    // Base-center (right wing)
    glVertex2f(-0.3f, 0.15f);    // Top-left
    glVertex2f(-0.15f, 0.05f);   // Near the tail
    glEnd();

    // Draw the propeller (front of airplane)
    glColor3f(0.8f, 0.8f, 0.8f); // Gray
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.0f);      // Center
    glVertex2f(0.2f, 0.1f);      // Top-right
    glVertex2f(0.2f, 0.0f);      // Center
    glVertex2f(0.2f, -0.1f);     // Bottom-right
    glEnd();

    // Optional: Add a flame for thrust
    glColor3f(1.0f, 0.4f, 0.0f); // Orange flame
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.22f, -0.02f);  // Base-left
    glVertex2f(-0.22f, 0.02f);   // Base-right
    glVertex2f(-0.3f, 0.0f);     // Tip
    glEnd();

    glPopMatrix();
}


void drawFullSun() {
    glPushMatrix();

    // Scale and position the sun at the top-left corner
    glTranslatef(-0.8f, 0.8f, 0.0f); // Adjust position for a full sun
    glScalef(sunScale, sunScale, 1.0f); // Apply scaling for sun size

    // Draw the full sun (circle)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow sun
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) { // Full circle (360 degrees)
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(0.3f * cos(angle), 0.3f * sin(angle)); // Radius of 0.3f
    }
    glEnd();

    // Add sun rays
    glColor3f(1.0f, 0.6f, 0.0f); // Orange rays
    for (int i = 0; i < 360; i += 30) { // Create rays every 30 degrees
        float angle = i * 3.14159f / 180.0f;
        glBegin(GL_TRIANGLES);
        glVertex2f(0.3f * cos(angle), 0.3f * sin(angle)); // Base of ray (on the sun circle)
        glVertex2f(0.4f * cos(angle - 0.05f), 0.4f * sin(angle - 0.05f)); // One side of the ray
        glVertex2f(0.4f * cos(angle + 0.05f), 0.4f * sin(angle + 0.05f)); // Other side of the ray
        glEnd();
    }

    glPopMatrix();
}


void drawCityClouds() {
    // Cloud 1
    glPushMatrix();
    glTranslatef(-0.6f + cloudX, 0.8f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -1; i <= 1; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j += 30) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(0.1f * cos(angle) + i * 0.1f, 0.05f * sin(angle));
        }
        glEnd();
    }
    glPopMatrix();

    // Cloud 2
    glPushMatrix();
    glTranslatef(0.4f + cloudX, 0.7f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -1; i <= 1; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j += 30) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(0.1f * cos(angle) + i * 0.1f, 0.05f * sin(angle));
        }
        glEnd();
    }
    glPopMatrix();
}


void drawCityBuildings() {
    // Building 1
    glPushMatrix();
    glTranslatef(-0.8f, -0.6f, 0.0f); // Position the building
    glScalef(0.2f, 0.6f, 1.0f);       // Scale for height and width
    glColor3f(0.5f, 0.5f, 0.5f);      // Gray color for building
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -1.0f);
    glVertex2f(0.5f, -1.0f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    glPopMatrix();

    // Building 2
    glPushMatrix();
    glTranslatef(-0.4f, -0.8f, 0.0f);
    glScalef(0.3f, 0.8f, 1.0f);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -1.0f);
    glVertex2f(0.5f, -1.0f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    glPopMatrix();

    // Building 3
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glScalef(0.4f, 0.5f, 1.0f);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -1.0f);
    glVertex2f(0.5f, -1.0f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    glPopMatrix();

    // Building 4
    glPushMatrix();
    glTranslatef(0.6f, -0.7f, 0.0f);
    glScalef(0.3f, 0.7f, 1.0f);
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -1.0f);
    glVertex2f(0.5f, -1.0f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    glPopMatrix();
}


void update(int value) {
    if (isRightMousePressed) {
        rocketRotation += 5.0f; // Rotate when right mouse is pressed
        if (rocketRotation > 360.0f) rocketRotation -= 360.0f;
    }

    // Vertical movement:
    if (isMousePressed) {
        rocketY += 0.02f; // Move up when left mouse is pressed
        if (rocketY > 0.95f) rocketY = 0.95f; // Prevent going above the window
    } else {
        rocketY -= 0.01f; // Fall down when mouse is not pressed
        if (rocketY < -0.95f) rocketY = -0.95f; // Prevent going below the window
    }

    // Constant horizontal movement
    rocketX += 0.01f;

    // Check if the airplane is off the right side of the screen
    if (rocketX > 1.0f) {
        rocketX = -0.9f;   // Reset to the default X position
        rocketY = 0.0f;    // Reset to the default Y position (within the window bounds)
        rocketRotation = 0.0f; // Reset to the default orientation
    }

    // Move the cloud
    cloudX -= 0.01f;
    if (cloudX < -1.2f) cloudX = 1.2f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Approximately 60 FPS
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        isMousePressed = (state == GLUT_DOWN);
    }
    if (button == GLUT_RIGHT_BUTTON) {
        isRightMousePressed = (state == GLUT_DOWN);
    }
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        buildingScale += 0.1f;
    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
        buildingScale -= 0.1f;
        if (buildingScale < 0.1f) buildingScale = 0.1f;
    }

    // Simulate mouse wheel using mouse buttons
    if (button == 3 && state == GLUT_DOWN) { // Scroll up (usually button 3)
        sunScale += 0.1f;
        if (sunScale > 2.0f) sunScale = 2.0f; // Limit the maximum size
    }
    if (button == 4 && state == GLUT_DOWN) { // Scroll down (usually button 4)
        sunScale -= 0.1f;
        if (sunScale < 0.5f) sunScale = 0.5f; // Limit the minimum size
    }

    glutPostRedisplay(); // Request a redraw to update the sun scale
}

void initVBO() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[] = {
        -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void init() {
    glewExperimental = GL_TRUE;
    glewInit();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearDepth(1.0f);      // Set depth buffer value
    glDepthFunc(GL_LEQUAL);  // Specify the depth comparison function

    initVBO();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    // Draw the background
    drawBackground();

    // Draw the half sun
    drawFullSun();

    // Draw city buildings
    drawCityBuildings();

    // Draw the rocket
    drawAirplane();

    // Draw city clouds
    drawCityClouds();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rocket Flight Game");

    glutFullScreen(); // Make the window full screen
	
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        cerr << "Error initializing GLEW: " << glewGetErrorString(glewStatus) << "\\n";
        return -1;
    }

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}


