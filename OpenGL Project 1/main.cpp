#include <GLFW/glfw3.h>
#include <cmath>
#include <array>

const float PI = 3.14159265358979323846f;

int windowWidth = 640;
int windowHeight = 480;

void setPerspective(float fovy, float aspect, float zNear, float zFar)
{
    float tanHalfFovy = tan(fovy * PI / 360.0f); // Optimized to combine division and radians conversion
    std::array<float, 16> matrix = {
        1.0f / (aspect * tanHalfFovy), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / tanHalfFovy, 0.0f, 0.0f,
        0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f,
        0.0f, 0.0f, -(2.0f * zFar * zNear) / (zFar - zNear), 0.0f
    };
    glMultMatrixf(matrix.data());
}

void setLookAt(float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ,
    float upX, float upY, float upZ)
{
    float f[3] = { centerX - eyeX, centerY - eyeY, centerZ - eyeZ };
    float length = std::sqrt(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
    f[0] /= length; f[1] /= length; f[2] /= length;

    float s[3] = { f[1] * upZ - f[2] * upY, f[2] * upX - f[0] * upZ, f[0] * upY - f[1] * upX };
    length = std::sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[0] /= length; s[1] /= length; s[2] /= length;

    float u[3] = { s[1] * f[2] - s[2] * f[1], s[2] * f[0] - s[0] * f[2], s[0] * f[1] - s[1] * f[0] };

    std::array<float, 16> matrix = {
        s[0], u[0], -f[0], 0.0f,
        s[1], u[1], -f[1], 0.0f,
        s[2], u[2], -f[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glMultMatrixf(matrix.data());
    glTranslatef(-eyeX, -eyeY, -eyeZ);
}

void drawCube()
{
    static const std::array<float, 72> vertices = {
        // Front face
        -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
        // Back face
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  -0.5f, 0.5f, -0.5f,
        // Top face
        -0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,
        // Left face
        -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, -0.5f,
        // Right face
        0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, -0.5f
    };

    glBegin(GL_QUADS);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
    }
    glEnd();
}

void drawDesk()
{
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 2.0f);  // Move the desk up slightly and forward

    // Desk top
    glColor3f(0.3f, 0.2f, 0.1f);  // Dark brown color for the desk
    glPushMatrix();
    glScalef(1.6f, 0.08f, 0.8f);  // Adjust size: wider, thinner, and less deep
    drawCube();
    glPopMatrix();

    // Desk front panel
    glPushMatrix();
    glTranslatef(0.0f, -0.3f, 0.35f);
    glScalef(1.5f, 0.5f, 0.05f);
    drawCube();
    glPopMatrix();

    // Desk legs
    glColor3f(0.2f, 0.1f, 0.05f);  // Darker color for the legs
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(i * 0.75f, -0.35f, j * 0.35f);
            glScalef(0.08f, 0.6f, 0.08f);
            drawCube();
            glPopMatrix();
        }
    }

    glPopMatrix();
}

void drawChair(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef(x, y, z);  // Position the chair based on parameters

    // Chair seat and back color
    glColor3f(0.4f, 0.2f, 0.1f);  // Darker brown color

    // Chair seat
    glPushMatrix();
    glScalef(0.6f, 0.1f, 0.6f);
    drawCube();
    glPopMatrix();

    // Chair back
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, -0.25f);
    glScalef(0.6f, 0.8f, 0.1f);
    drawCube();
    glPopMatrix();

    // Chair legs
    glColor3f(0.3f, 0.15f, 0.05f);  // Even darker brown for legs
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(i * 0.25f, -0.5f, j * 0.25f);
            glScalef(0.1f, 1.0f, 0.1f);
            drawCube();
            glPopMatrix();
        }
    }

    glPopMatrix();
}

void drawChairRows()
{
    // Left row
    for (int i = 0; i < 3; i++) {
        drawChair(-1.5f, -1.0f, -1.0f - i * 2.0f);
    }

    // Right row
    for (int i = 0; i < 3; i++) {
        drawChair(1.5f, -1.0f, -1.0f - i * 2.0f);
    }
}

void drawWindow()
{
    glPushMatrix();
    glTranslatef(3.9f, 0.5f, -5.0f);  // Position the window on the right wall

    // Window frame
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray color for frame
    glPushMatrix();
    glScalef(0.1f, 1.5f, 1.0f);
    drawCube();
    glPopMatrix();

    // Window glass
    glColor4f(0.1f, 0.1f, 0.3f, 0.3f);  // Dark blue, semi-transparent
    glPushMatrix();
    glTranslatef(-0.05f, 0.0f, 0.0f);
    glScalef(0.01f, 1.4f, 0.9f);
    drawCube();
    glPopMatrix();

    glPopMatrix();
}

void drawRoom()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setPerspective(45.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up moonlight
    GLfloat light_position[] = { 2.0f, 2.0f, -5.0f, 1.0f };  // Position of the light (window)
    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.2f, 1.0f };    // Soft blue ambient light
    GLfloat light_diffuse[] = { 0.2f, 0.2f, 0.3f, 1.0f };    // Soft blue diffuse light
    GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };   // Reduced specular reflection

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Adjust material properties for a softer look
    GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_shininess[] = { 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    setLookAt(0.0f, 1.5f, 14.0f, 0.0f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);

    // Floor
    glColor3f(0.5f, 0.5f, 0.5f);  // Gray
    glVertex3f(-4.0f, -1.5f, -10.0f);
    glVertex3f(4.0f, -1.5f, -10.0f);
    glVertex3f(4.0f, -1.5f, 4.0f);  // Extend the floor forward
    glVertex3f(-4.0f, -1.5f, 4.0f);  // Extend the floor forward

    // Ceiling
    glColor3f(1.0f, 1.0f, 1.0f);  // White
    glVertex3f(-4.0f, 3.0f, -10.0f);  // Raise the ceiling
    glVertex3f(4.0f, 3.0f, -10.0f);   // Raise the ceiling
    glVertex3f(4.0f, 3.0f, 4.0f);     // Extend and raise the ceiling
    glVertex3f(-4.0f, 3.0f, 4.0f);    // Extend and raise the ceiling

    // Left wall
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glVertex3f(-4.0f, -1.5f, -10.0f);
    glVertex3f(-4.0f, -1.5f, 4.0f);
    glVertex3f(-4.0f, 3.0f, 4.0f);   // Raise the wall
    glVertex3f(-4.0f, 3.0f, -10.0f); // Raise the wall

    // Right wall
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glVertex3f(4.0f, -1.5f, -10.0f);
    glVertex3f(4.0f, -1.5f, 4.0f);
    glVertex3f(4.0f, 3.0f, 4.0f);   // Raise the wall
    glVertex3f(4.0f, 3.0f, -10.0f); // Raise the wall

    // Back wall
    glColor3f(0.3f, 0.7f, 0.3f);  // Light green
    glVertex3f(-4.0f, -1.5f, -10.0f);
    glVertex3f(4.0f, -1.5f, -10.0f);
    glVertex3f(4.0f, 3.0f, -10.0f); // Raise the wall
    glVertex3f(-4.0f, 3.0f, -10.0f); // Raise the wall

    glEnd();

    // Draw the chairs
    drawChairRows();

    // Draw the window
    drawWindow();

    // Draw the teacher's desk
    drawDesk();
}

// Window size callback
void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

int main(void)
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Lab Room", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, windowWidth, windowHeight);

        drawRoom();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
