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
float moveXBuildings = 0.0f;
float rocketScale = 1.0f;      // Scale for the rocket
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

    // Translate and rotate the rocket
    glTranslatef(rocketX, rocketY, 0.0f);
    glScalef(rocketScale, rocketScale, 2.0f);
    glRotatef(rocketRotation, 0.0f, 0.0f, 1.0f);

    // Scale down the airplane
    glScalef(0.5f, 0.5f, 1.0f); 

    // Draw the body
    glColor3f(1.0f, 0.8f, 0.0f); // Golden yellow
    glBegin(GL_QUADS);
	glVertex2f(-0.2f, -0.05f);  // Bottom-left 
	glVertex2f(0.14f, -0.05f);   // Bottom-right
	glVertex2f(0.14f, 0.05f);    // Top-right 
	glVertex2f(-0.2f, 0.05f);   // Top-left 
    glEnd();
    
    // Draw the nose 
	glColor3f(1.0f, 0.8f, 0.0f);  // Golden yellow
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 180; i++) {
	    float angle = i * 3.14159f / 180.0f;
	    glVertex2f(0.11f + 0.1f * cos(angle), -0.05f + 0.1f * sin(angle)); 
	}
	glEnd();

    // Draw the cockpit (semi-circle)
    glColor3f(0.678f, 0.847f, 0.902f); // Light blue
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(-0.1f + 0.1f * cos(angle), 0.05f + 0.05f * sin(angle));
    }
    glEnd();

    // Draw the tail (vertical stabilizer)
    glColor3f(0.5f, 0.5f, 0.5f);  // Golden yellow
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.0f);     // Bottom-left
    glVertex2f(-0.15f, 0.0f);    // Bottom-right
    glVertex2f(-0.15f, 0.1f);    // Top-right
    glVertex2f(-0.2f, 0.1f);     // Top-left
    glEnd();

    // Optional: Add a flame for thrust
    glColor3f(1.0f, 0.4f, 0.0f); // Orange flame
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.21f, -0.05f);  // Base-left
    glVertex2f(-0.21f, 0.05f);   // Base-right
    glVertex2f(-0.4f, 0.0f);     // Tip
    glEnd();

    glPopMatrix();
}

void drawFullSun() {
    glPushMatrix();

    // Scale and position the sun at the top-left corner
    glTranslatef(-0.9f, 0.8f, 0.0f); 

    // Draw the full sun (circle)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow sun
    glBegin(GL_POLYGON);

    for (int i = 0; i < 360; i++) { 
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(0.28f * cos(angle), 0.5f * sin(angle));
    }
    glEnd();

     // Draw sun rays
    glColor3f(1.0f, 0.65f, 0.0f); // Orange 

    int numRays = 20; // Number of rays
    float rayAngleStep = 360.0f / numRays; // Angle

    // Acute triangle measurements
    float sunRadiusX = 0.27f;  // Horizontal radius of the sun
    float sunRadiusY = 0.51f;   // Vertical radius of the sun
    float baseDistance = 0.03f; // Distance from the sun to the base of the rays
    float rayGap = 0.03f;      // gap between rays
    float rayHeight = 0.08f;    // Height of the triangle

    for (int i = 0; i < numRays; i++) {
        // Adjust the angle for spacing between rays
        float startAngle = i * rayAngleStep * 3.14159f / 180.0f;
        float endAngle = (i + 1) * rayAngleStep * 3.14159f / 180.0f - rayGap; 
        float midAngle = (startAngle + endAngle) / 2.0f; // Midpoint angle for the tip

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
        glVertex2f(baseX1, baseY1); // Base point 1
        glVertex2f(baseX2, baseY2); // Base point 2
        glVertex2f(tipX, tipY);     // Tip point
        glEnd();
    }

    glPopMatrix();
}

void drawCityClouds() {
    glColor3f(1.0f, 1.0f, 1.0f); // white
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
	glTranslatef(-0.8f, -0.6f, 0.0f); 
	glScalef(0.15f, 0.6f, 1.0f);       
	glColor3f(0.5f, 0.5f, 0.5f);      
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// Adding windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
		
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
	
	// Adding windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	glColor3f(0.3f, 0.3f, 0.3f); // Dark gray for the building
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
	glColor3f(0.4f, 0.4f, 0.4f); // Dark gray for the building
	
	// Building structure
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -1.0f);
	glVertex2f(0.5f, -1.0f);
	glVertex2f(0.5f, 0.0f);
	glVertex2f(-0.5f, 0.0f);
	glEnd();
	
	// Adding yellow windows
	glColor3f(0.8f, 0.9f, 1.0f); // Light blue color for windows
	
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
        rocketRotation += 5.0f; // Rotate when right mouse is pressed
        if (rocketRotation > 360.0f) rocketRotation -= 360.0f;
    }

    // Movement logic when left mouse button is pressed
    if (isMousePressed) {
        rocketX += 0.01f; // Move forward
        rocketY += 0.02f; // Move up
        if (rocketX >= 0.8f) rocketX = 0.8f; // Prevent moving too far forward
        if (rocketY > 0.95f) rocketY = 0.95f; // Prevent going above the window
    } else {
        // When the left mouse button is released, move backward and fall down
        rocketX -= 0.003f; // Slowly move backward
        rocketY -= 0.009f; // Slowly fall down
        if (rocketX <= -0.8f) rocketX = -0.8f; // Prevent moving too far backward
        if (rocketY < -0.8f) rocketY = -0.8f; // Prevent going below the window
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
        rocketScale += 0.05f;
        if (rocketScale > 2.5f) rocketScale = 2.5f; // Limit the maximum size
    }
    if (button == 4 && state == GLUT_DOWN) { // Scroll down (usually button 4)
        rocketScale -= 0.1f;
        if (rocketScale < 1.0f) rocketScale = 1.0f; // Limit the minimum size
    }

    glutPostRedisplay(); // Request a redraw to update the rocket scale
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
