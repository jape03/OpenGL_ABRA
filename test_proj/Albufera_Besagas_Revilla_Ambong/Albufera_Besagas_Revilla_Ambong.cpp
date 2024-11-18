/*
TN36

Albufera, Joshua
Besagas, John Paul
Revilla, Karl Alexis
Ambong, Darren Jake

*/

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
float buildingScale = 1.0f; 
float moveXBuildings = 0.0f;
float sunScale = 0.8f;      
float sunRotation = 0.0f;
bool isMousePressed = false;
bool isRightMousePressed = false;

GLuint vbo;

void drawBackground() {

    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 1.0f); 
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 0.5f); 
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

void drawAirplane() {
    glPushMatrix();

    // Translate and rotate the rocket
    glTranslatef(rocketX, rocketY, 0.0f);
    glRotatef(rocketRotation, 0.0f, 0.0f, 1.0f);

    // Scale down the airplane
    glScalef(0.5f, 0.5f, 1.0f); 

    // body
    glColor3f(1.0f, 0.8f, 0.0f); 
    glBegin(GL_QUADS);
	glVertex2f(-0.2f, -0.05f);  
	glVertex2f(0.14f, -0.05f);  
	glVertex2f(0.14f, 0.05f);    
	glVertex2f(-0.2f, 0.05f);  
    glEnd();
    
    // nose 
	glColor3f(1.0f, 0.8f, 0.0f);  
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 180; i++) {
	    float angle = i * 3.14159f / 180.0f;
	    glVertex2f(0.11f + 0.1f * cos(angle), -0.05f + 0.1f * sin(angle)); 
	}
	glEnd();

    // cockpit 
    glColor3f(0.678f, 0.847f, 0.902f); 
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(-0.1f + 0.1f * cos(angle), 0.05f + 0.05f * sin(angle));
    }
    glEnd();

    //  tail 
    glColor3f(0.5f, 0.5f, 0.5f);  
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.0f);    
    glVertex2f(-0.15f, 0.0f);    
    glVertex2f(-0.15f, 0.1f);    
    glVertex2f(-0.2f, 0.1f);     
    glEnd();

    // thrust
    glColor3f(1.0f, 0.4f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.21f, -0.05f);  
    glVertex2f(-0.21f, 0.05f);  
    glVertex2f(-0.4f, 0.0f);     
    glEnd();

    glPopMatrix();
}

void drawFullSun() {
    glPushMatrix();

    // Scale and position the sun at the top-left corner
    glTranslatef(-0.8f, 0.8f, 0.0f); 
    glScalef(sunScale, sunScale, 1.0f); 

    // full sun (circle)
    glColor3f(1.0f, 1.0f, 0.0f); 
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++) { 
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(0.28f * cos(angle), 0.5f * sin(angle));
    }
    glEnd();

     // sun rays
    glColor3f(1.0f, 0.65f, 0.0f); 

    int numRays = 20; 
    float rayAngleStep = 360.0f / numRays; 

    // Acute triangle measurements
    float sunRadiusX = 0.27f;  
    float sunRadiusY = 0.51f;   
    float baseDistance = 0.03f; 
    float rayGap = 0.03f;      
    float rayHeight = 0.08f;    

    for (int i = 0; i < numRays; i++) {
        // the angle for spacing between rays
        float startAngle = i * rayAngleStep * 3.14159f / 180.0f;
        float endAngle = (i + 1) * rayAngleStep * 3.14159f / 180.0f - rayGap; 
        float midAngle = (startAngle + endAngle) / 2.0f; 

        // Base points on the sun's circumference 
        float baseX1 = (sunRadiusX + baseDistance) * cos(startAngle);
        float baseY1 = (sunRadiusY + baseDistance) * sin(startAngle);

        float baseX2 = (sunRadiusX + baseDistance) * cos(endAngle);
        float baseY2 = (sunRadiusY + baseDistance) * sin(endAngle);

        // Tip of the triangle
        float tipX = (sunRadiusX + rayHeight + baseDistance) * cos(midAngle);
        float tipY = (sunRadiusY + rayHeight + baseDistance) * sin(midAngle);

        // Draw triangle 
        glBegin(GL_TRIANGLES);
        glVertex2f(baseX1, baseY1);
        glVertex2f(baseX2, baseY2); 
        glVertex2f(tipX, tipY);     
        glEnd();
    }

    glPopMatrix();
}

void drawCityClouds() {
    glColor3f(1.0f, 1.0f, 1.0f); 
    // Cloud 1
    glPushMatrix();
    glTranslatef(-0.6f + cloudX, 0.55f, 0.0f); 

    // Left circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle) - 0.2f, 0.15f * sin(angle));
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle) + 0.2f, 0.15f * sin(angle));
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.2f * cos(angle), 0.25f * sin(angle) + 0.15f);
    }
    glEnd();

    // Bottom rounded rectangle for cloud base
    glBegin(GL_POLYGON);
    glVertex2f(-0.25f, -0.05f);
    glVertex2f(0.25f, -0.05f);
    glVertex2f(0.25f, 0.05f);
    glVertex2f(-0.25f, 0.05f);
    glEnd();

    // Flat rectangle in the center
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.15f); 
    glVertex2f(0.2f, -0.15f);  
    glVertex2f(0.2f, 0.05f);   
    glVertex2f(-0.2f, 0.05f);  
    glEnd();

    glPopMatrix();

    // Cloud 2 
    glPushMatrix();
    glTranslatef(0.2f + cloudX, 0.25f, -0.5f); 

    // Left circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.12f * cos(angle) - 0.15f, 0.12f * sin(angle)); 
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.12f * cos(angle) + 0.15f, 0.12f * sin(angle));
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle), 0.2f * sin(angle) + 0.1f); 
    }
    glEnd();

    // Flat rectangle in the center
    glBegin(GL_QUADS);
    glVertex2f(-0.17f, -0.12f); 
    glVertex2f(0.17f, -0.12f); 
    glVertex2f(0.18f, 0.11f);  
    glVertex2f(-0.18f, 0.11f);  
    glEnd();

    glPopMatrix();

    // Cloud 3
    glPushMatrix();
    glTranslatef(0.8f + cloudX, 0.75f, -1.0f); 

    // Left circle 
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.08f * cos(angle) - 0.1f, 0.08f * sin(angle)); 
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.08f * cos(angle) + 0.1f, 0.08f * sin(angle));
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.1f * cos(angle), 0.15f * sin(angle) + 0.07f);
    }
    glEnd();

    // Flat rectangle in the center 
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.08f);  
    glVertex2f(0.1f, -0.08f);  
    glVertex2f(0.1f, 0.05f);    
    glVertex2f(-0.1f, 0.05f);   
    glEnd();

    glPopMatrix();
    
    // Cloud 4
    glPushMatrix();
    glTranslatef(1.8f + cloudX, 0.55f, 0.0f); 

    // Left circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle) - 0.2f, 0.15f * sin(angle));
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle) + 0.2f, 0.15f * sin(angle));
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.2f * cos(angle), 0.25f * sin(angle) + 0.15f);
    }
    glEnd();

    // Bottom rounded rectangle for cloud base
    glBegin(GL_POLYGON);
    glVertex2f(-0.25f, -0.05f);
    glVertex2f(0.25f, -0.05f);
    glVertex2f(0.25f, 0.05f);
    glVertex2f(-0.25f, 0.05f);
    glEnd();

    // Flat rectangle in the center
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.15f);
    glVertex2f(0.2f, -0.15f);  
    glVertex2f(0.2f, 0.05f);   
    glVertex2f(-0.2f, 0.05f);  
    glEnd();

    glPopMatrix();
    
    // Cloud 5 
    glPushMatrix();
    glTranslatef(2.8f + cloudX, 0.25f, -0.5f); 

    // Left circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.12f * cos(angle) - 0.15f, 0.12f * sin(angle)); 
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.12f * cos(angle) + 0.15f, 0.12f * sin(angle));
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle), 0.2f * sin(angle) + 0.1f); 
    }
    glEnd();

    // Flat rectangle in the center
    glBegin(GL_QUADS);
    glVertex2f(-0.17f, -0.12f); 
    glVertex2f(0.17f, -0.12f); 
    glVertex2f(0.18f, 0.11f);   
    glVertex2f(-0.18f, 0.11f);  
    glEnd();

    glPopMatrix();

    // Cloud 6
    glPushMatrix();
    glTranslatef(3.3f + cloudX, 0.75f, -1.0f);

    // Left circle 
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.08f * cos(angle) - 0.1f, 0.08f * sin(angle));
    }
    glEnd();

    // Right circle 
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.08f * cos(angle) + 0.1f, 0.08f * sin(angle)); 
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.1f * cos(angle), 0.15f * sin(angle) + 0.07f); 
    }
    glEnd();

    // Flat rectangle in the center 
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.08f);  
    glVertex2f(0.1f, -0.08f);   
    glVertex2f(0.1f, 0.05f);    
    glVertex2f(-0.1f, 0.05f); 
    glEnd();

    glPopMatrix();
    
    // Cloud 7
    glPushMatrix();
    glTranslatef(5.1f + cloudX, 0.55f, 0.0f); 

    // Left circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle) - 0.2f, 0.15f * sin(angle));
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.15f * cos(angle) + 0.2f, 0.15f * sin(angle));
    }
    glEnd();

    // Center top circle
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j += 10) {
        float angle = j * 3.14159f / 180.0f;
        glVertex2f(0.2f * cos(angle), 0.25f * sin(angle) + 0.15f);
    }
    glEnd();

    // Bottom rounded rectangle for cloud base
    glBegin(GL_POLYGON);
    glVertex2f(-0.25f, -0.05f);
    glVertex2f(0.25f, -0.05f);
    glVertex2f(0.25f, 0.05f);
    glVertex2f(-0.25f, 0.05f);
    glEnd();

    // Flat rectangle in the center
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.15f); 
    glVertex2f(0.2f, -0.15f); 
    glVertex2f(0.2f, 0.05f);   
    glVertex2f(-0.2f, 0.05f); 
    glEnd();

    glPopMatrix();
    
}

void drawCityBuildings() {
    glPushMatrix();
    glTranslatef(moveXBuildings, 0.0f, 0.0f); 

    // First set of buildings 
    // Building 1
	glPushMatrix();
	glTranslatef(-0.84f, -0.6f, 0.0f); 
	glScalef(0.15f, 0.6f, 1.0f);       
	glColor3f(0.5f, 0.5f, 0.5f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// windows
	glColor3f(0.8f, 0.9f, 1.0f); 
		
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.8f);
	glVertex2f(-0.1f, -0.8f);
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.3f, -0.6f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.8f);
	glVertex2f(0.3f, -0.8f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(0.1f, -0.6f);
	glEnd();
	
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.4f);
	glVertex2f(-0.1f, -0.4f);
	glVertex2f(-0.1f, -0.2f);
	glVertex2f(-0.3f, -0.2f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.4f);
	glVertex2f(0.3f, -0.4f);
	glVertex2f(0.3f, -0.2f);
	glVertex2f(0.1f, -0.2f);
	glEnd();
	
	glPopMatrix();

    // Building 2
	glPushMatrix();
	glTranslatef(-0.4f, -0.6f, 0.0f); 
	glScalef(0.15f, 0.5f, 1.0f); 
	glColor3f(0.6f, 0.6f, 0.6f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.9f); 
	glVertex2f(-0.15f, -0.9f);
	glVertex2f(-0.15f, -0.7f); 
	glVertex2f(-0.35f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.9f); 
	glVertex2f(0.35f, -0.9f);
	glVertex2f(0.35f, -0.7f); 
	glVertex2f(0.15f, -0.7f);
	glEnd();
	
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.6f); 
	glVertex2f(-0.15f, -0.6f);
	glVertex2f(-0.15f, -0.4f); 
	glVertex2f(-0.35f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.6f); 
	glVertex2f(0.35f, -0.6f);
	glVertex2f(0.35f, -0.4f); 
	glVertex2f(0.15f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.3f); 
	glVertex2f(-0.15f, -0.3f);
	glVertex2f(-0.15f, -0.1f); 
	glVertex2f(-0.35f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.3f); 
	glVertex2f(0.35f, -0.3f);
	glVertex2f(0.35f, -0.1f); 
	glVertex2f(0.15f, -0.1f);
	glEnd();
	
	glPopMatrix();
 
    // Building 3
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f); 
	glScalef(0.15f, 0.5f, 1.0f);       
	glColor3f(0.4f, 0.4f, 0.4f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.8f);
	glVertex2f(-0.15f, -0.8f);
	glVertex2f(-0.15f, -0.6f);
	glVertex2f(-0.35f, -0.6f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.8f);
	glVertex2f(0.35f, -0.8f);
	glVertex2f(0.35f, -0.6f);
	glVertex2f(0.15f, -0.6f);
	glEnd();
	
	// Second row of windows
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.4f);
	glVertex2f(-0.15f, -0.4f);
	glVertex2f(-0.15f, -0.2f);
	glVertex2f(-0.35f, -0.2f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.4f);
	glVertex2f(0.35f, -0.4f);
	glVertex2f(0.35f, -0.2f);
	glVertex2f(0.15f, -0.2f);
	glEnd();
	
	glPopMatrix();


    // Building 4
	glPushMatrix();
	glTranslatef(0.6f, -0.52f, 0.0f); 
	glScalef(0.15f, 0.7f, 1.0f);       
	glColor3f(0.7f, 0.7f, 0.7f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows 
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.9f); 
	glVertex2f(-0.15f, -0.9f);
	glVertex2f(-0.15f, -0.7f); 
	glVertex2f(-0.35f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.9f); 
	glVertex2f(0.35f, -0.9f);
	glVertex2f(0.35f, -0.7f); 
	glVertex2f(0.15f, -0.7f);
	glEnd();
	
	// Second row of windows 
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.6f); 
	glVertex2f(-0.15f, -0.6f);
	glVertex2f(-0.15f, -0.4f); 
	glVertex2f(-0.35f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.6f); 
	glVertex2f(0.35f, -0.6f);
	glVertex2f(0.35f, -0.4f); 
	glVertex2f(0.15f, -0.4f);
	glEnd();
	
	// Third row of windows 
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.3f); 
	glVertex2f(-0.15f, -0.3f);
	glVertex2f(-0.15f, -0.1f); 
	glVertex2f(-0.35f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.3f); 
	glVertex2f(0.35f, -0.3f);
	glVertex2f(0.35f, -0.1f); 
	glVertex2f(0.15f, -0.1f);
	glEnd();
	
	glPopMatrix();

    // Building 5
	glPushMatrix();
	glTranslatef(-0.7f, -0.5f, 0.0f); 
	glScalef(0.2f, 0.5f, 1.0f);       
	glColor3f(0.4f, 0.4f, 0.4f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.9f);
	glVertex2f(-0.1f, -0.9f);
	glVertex2f(-0.1f, -0.7f);
	glVertex2f(-0.3f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.9f);
	glVertex2f(0.3f, -0.9f);
	glVertex2f(0.3f, -0.7f);
	glVertex2f(0.1f, -0.7f);
	glEnd();
	
	// Second row of windows
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.6f);
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.1f, -0.4f);
	glVertex2f(-0.3f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.6f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(0.3f, -0.4f);
	glVertex2f(0.1f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.3f);
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.1f);
	glVertex2f(-0.3f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.3f);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(0.3f, -0.1f);
	glVertex2f(0.1f, -0.1f);
	glEnd();
	
	glPopMatrix();

    // Building 6
	glPushMatrix();
	glTranslatef(0.4f, -0.6f, 0.0f); 
	glScalef(0.18f, 0.7f, 1.0f);       
	glColor3f(0.55f, 0.55f, 0.55f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	//  yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.9f);
	glVertex2f(-0.15f, -0.9f);
	glVertex2f(-0.15f, -0.7f);
	glVertex2f(-0.35f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.9f);
	glVertex2f(0.35f, -0.9f);
	glVertex2f(0.35f, -0.7f);
	glVertex2f(0.15f, -0.7f);
	glEnd();
	
	// Second row of windows
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.6f);
	glVertex2f(-0.15f, -0.6f);
	glVertex2f(-0.15f, -0.4f);
	glVertex2f(-0.35f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.6f);
	glVertex2f(0.35f, -0.6f);
	glVertex2f(0.35f, -0.4f);
	glVertex2f(0.15f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.3f);
	glVertex2f(-0.15f, -0.3f);
	glVertex2f(-0.15f, -0.1f);
	glVertex2f(-0.35f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.3f);
	glVertex2f(0.35f, -0.3f);
	glVertex2f(0.35f, -0.1f);
	glVertex2f(0.15f, -0.1f);
	glEnd();
	
	glPopMatrix();

    // Building 7
	glPushMatrix();
	glTranslatef(-0.28f, -0.35f, 0.0f); 
	glScalef(0.15f, 0.9f, 1.0f);       
	glColor3f(0.55f, 0.55f, 0.55f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows 
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.9f); 
	glVertex2f(-0.1f, -0.9f);
	glVertex2f(-0.1f, -0.7f); 
	glVertex2f(-0.35f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.9f); 
	glVertex2f(0.35f, -0.9f);
	glVertex2f(0.35f, -0.7f); 
	glVertex2f(0.1f, -0.7f);
	glEnd();
	
	// Second row of windows 
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.6f); 
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.1f, -0.4f); 
	glVertex2f(-0.35f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.6f); 
	glVertex2f(0.35f, -0.6f);
	glVertex2f(0.35f, -0.4f); 
	glVertex2f(0.1f, -0.4f);
	glEnd();
	
	// Third row of windows 
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.3f); 
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.1f); 
	glVertex2f(-0.35f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.3f); 
	glVertex2f(0.35f, -0.3f);
	glVertex2f(0.35f, -0.1f); 
	glVertex2f(0.1f, -0.1f);
	glEnd();
	
	glPopMatrix();

   // Building 8
	glPushMatrix();
	glTranslatef(0.8f, -0.4f, 0.0f); 
	glScalef(0.16f, 0.65f, 1.0f);       
	glColor3f(0.75f, 0.75f, 0.75f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.9f);
	glVertex2f(-0.1f, -0.9f);
	glVertex2f(-0.1f, -0.7f);
	glVertex2f(-0.3f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.9f);
	glVertex2f(0.3f, -0.9f);
	glVertex2f(0.3f, -0.7f);
	glVertex2f(0.1f, -0.7f);
	glEnd();
	
	// Second row of windows
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.6f);
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.1f, -0.4f);
	glVertex2f(-0.3f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.6f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(0.3f, -0.4f);
	glVertex2f(0.1f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.3f);
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.1f);
	glVertex2f(-0.3f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.3f);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(0.3f, -0.1f);
	glVertex2f(0.1f, -0.1f);
	glEnd();
	
	glPopMatrix();

    // Building 9
	glPushMatrix();
	glTranslatef(-0.1f, -0.7f, 0.0f); 
	glScalef(0.17f, 0.6f, 1.0f);       
	glColor3f(0.3f, 0.3f, 0.3f); 
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	//  yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.9f);
	glVertex2f(-0.1f, -0.9f);
	glVertex2f(-0.1f, -0.7f);
	glVertex2f(-0.3f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.9f);
	glVertex2f(0.3f, -0.9f);
	glVertex2f(0.3f, -0.7f);
	glVertex2f(0.1f, -0.7f);
	glEnd();
	
	// Second row of windows
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.6f);
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.1f, -0.4f);
	glVertex2f(-0.3f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.6f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(0.3f, -0.4f);
	glVertex2f(0.1f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.3f);
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.1f);
	glVertex2f(-0.3f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.3f);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(0.3f, -0.1f);
	glVertex2f(0.1f, -0.1f);
	glEnd();
	
	glPopMatrix();


    // Building 10
	glPushMatrix();
	glTranslatef(0.7f, -0.7f, 0.0f); 
	glScalef(0.17f, 0.6f, 1.0f);       
	glColor3f(0.5f, 0.5f, 0.5f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.9f);
	glVertex2f(-0.1f, -0.9f);
	glVertex2f(-0.1f, -0.7f);
	glVertex2f(-0.3f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.9f);
	glVertex2f(0.3f, -0.9f);
	glVertex2f(0.3f, -0.7f);
	glVertex2f(0.1f, -0.7f);
	glEnd();
	
	// Second row of windows
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.6f);
	glVertex2f(-0.1f, -0.6f);
	glVertex2f(-0.1f, -0.4f);
	glVertex2f(-0.3f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.6f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(0.3f, -0.4f);
	glVertex2f(0.1f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.3f);
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.1f);
	glVertex2f(-0.3f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.1f, -0.3f);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(0.3f, -0.1f);
	glVertex2f(0.1f, -0.1f);
	glEnd();

	glPopMatrix();

    // Building 11
	glPushMatrix();
	glTranslatef(0.28f, -0.4f, 0.0f);  
	glScalef(0.15f, 0.7f, 1.0f);       
	glColor3f(0.4f, 0.4f, 0.4f); 
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); 
	
	// First row of windows 
	// Window 1
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.9f); 
	glVertex2f(-0.15f, -0.9f);
	glVertex2f(-0.15f, -0.7f); 
	glVertex2f(-0.35f, -0.7f);
	glEnd();
	
	// Window 2
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.9f); 
	glVertex2f(0.35f, -0.9f);
	glVertex2f(0.35f, -0.7f); 
	glVertex2f(0.15f, -0.7f);
	glEnd();
	
	// Second row of windows 
	// Window 3
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.6f); 
	glVertex2f(-0.15f, -0.6f);
	glVertex2f(-0.15f, -0.4f); 
	glVertex2f(-0.35f, -0.4f);
	glEnd();
	
	// Window 4
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.6f); 
	glVertex2f(0.35f, -0.6f);
	glVertex2f(0.35f, -0.4f); 
	glVertex2f(0.15f, -0.4f);
	glEnd();
	
	// Third row of windows
	// Window 5
	glBegin(GL_QUADS);
	glVertex2f(-0.35f, -0.3f);
	glVertex2f(-0.15f, -0.3f);
	glVertex2f(-0.15f, -0.1f); 
	glVertex2f(-0.35f, -0.1f);
	glEnd();
	
	// Window 6
	glBegin(GL_QUADS);
	glVertex2f(0.15f, -0.3f); 
	glVertex2f(0.35f, -0.3f);
	glVertex2f(0.35f, -0.1f); 
	glVertex2f(0.15f, -0.1f);
	glEnd();
	
	glPopMatrix();

    glPopMatrix();
}




void update(int value) {
    // Rotate the rocket when the right mouse button is pressed
    if (isRightMousePressed) {
        rocketRotation += 5.0f; 
        if (rocketRotation > 360.0f) rocketRotation -= 360.0f;
    }

    // Movement logic when left mouse button is pressed
    if (isMousePressed) {
        rocketX += 0.01f; 
        rocketY += 0.02f; 
        if (rocketX >= 0.8f) rocketX = 0.8f; 
        if (rocketY > 0.95f) rocketY = 0.95f; 
    } else {
        // When the left mouse button is released, move backward and fall down
        rocketX -= 0.003f; 
        rocketY -= 0.009f;
        if (rocketX <= -0.8f) rocketX = -0.8f; 
        if (rocketY < -0.8f) rocketY = -0.8f; 
    }

    // Move the cloud
    cloudX -= 0.01f;
    if (cloudX < -4.5f) cloudX = 1.2f;
    
   // Move the buildings
	moveXBuildings -= 0.02f; 
	if (moveXBuildings < -2.0f) {
	    moveXBuildings = 1.65f;
	}
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); 
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
    //  mouse wheel using mouse buttons
    if (button == 3 && state == GLUT_DOWN) { 
        sunScale += 0.1f;
        if (sunScale > 2.0f) sunScale = 2.0f; 
    }
    if (button == 4 && state == GLUT_DOWN) { 
        sunScale -= 0.1f;
        if (sunScale < 0.5f) sunScale = 0.5f; 
    }

    glutPostRedisplay(); 
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

    glEnable(GL_DEPTH_TEST); 
    glClearDepth(1.0f);      
    glDepthFunc(GL_LEQUAL);  

    initVBO();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    // background
    drawBackground();
    //  sun
    drawFullSun();
    //  city buildings
    drawCityBuildings();
    // the rocket
    drawAirplane();
    // city clouds
    drawCityClouds();	
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("SA_PROJ_ABRA");

    glutFullScreen(); 
	
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
