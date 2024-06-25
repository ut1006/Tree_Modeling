#include "include/draw.h"
#include "include/globals.h"
#include "include/branches.h"
#include "include/branch.h"
#include <cmath>
#include <GL/glu.h>
void drawAxes() {
    // X axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINES);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glEnd();

    // Y axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glBegin(GL_LINES);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glEnd();

    // Z axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void drawGroundPlane() {
    glColor3f(0.5f, 0.5f, 0.5f); // Set the ground plane color to gray
    glBegin(GL_QUADS);

    // Draw a large ground plane
    glVertex3f(-50.0f, -1.0f, -50.0f);
    glVertex3f(50.0f, -1.0f, -50.0f);
    glVertex3f(50.0f, -1.0f, 50.0f);
    glVertex3f(-50.0f, -1.0f, 50.0f);

    glEnd();
}

void drawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    // Draw the cylinder
    glPushMatrix();
    glTranslatef(x1, y1, z1);
    // Calculate the direction vector
    float dirX = x2 - x1;
    float dirY = y2 - y1;
    float dirZ = z2 - z1;

    // Calculate the length of the cylinder
    float length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

    // Normalize the direction vector
    float dirLength = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    dirX /= dirLength;
    dirY /= dirLength;
    dirZ /= dirLength;

    // Calculate the angle and axis for rotation
    float angleX = atan2(dirY, dirZ) * 180.0 / M_PI;
    float angleY = atan2(dirX, sqrt(dirY * dirY + dirZ *dirZ)) * 180.0 / M_PI;
    glRotatef(-angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);


    // Set the color to brown
    glColor3f(0.4f, 0.1f, 0.1f);



    gluCylinder(quad, radius, radius, length, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawEllipse(float majorAxis, float minorAxis) {
    const int numSegments = 36; // Number of segments to approximate ellipse
    const float angleIncrement = 2.0f * M_PI / numSegments; // Angle increment for each segment
    glBegin(GL_TRIANGLE_FAN);
    

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = majorAxis * cos(angle);
        float y = minorAxis * sin(angle);
        glVertex3f(x, y, 0.0f); // Draw a vertex at the calculated point
    }
    glEnd();
}

// Function to find the thickness of the parent branch
std::pair<bool, float> findParentThickness(const std::vector<Branch>& branches, float parent_x, float parent_y, float parent_z) {
    for (const auto& branch : branches) {
        if (branch.child_x == parent_x && branch.child_y == parent_y && branch.child_z == parent_z) {
            return {true, branch.thickness};
        }
    }
    return {false, 0.0f}; // Return false if the parent thickness is not found
}


void drawLeaf(const Branch& branch, const std::vector<Branch>& branches) {
    glPushMatrix();

    // Calculate direction
    float dirX = branch.child_x - branch.parent_x;
    float dirY = branch.child_y - branch.parent_y;
    float dirZ = branch.child_z - branch.parent_z;
    float length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);

    // Normalize direction
    dirX /= length;
    dirY /= length;
    dirZ /= length;

    // Calculate angles
    float angleX = atan2(dirY, dirZ) * 180.0f / M_PI;
    float angleY = atan2(dirX, sqrt(dirY * dirY + dirZ * dirZ)) * 180.0f / M_PI;

    // Move to the starting point
    glTranslatef(branch.parent_x, branch.parent_y, branch.parent_z);

    // Draw stem
    float stemLength = length * 0.2f;
    drawCylinder(0, 0, 0, dirX * 0.1f, dirY * 0.1f, dirZ * 0.1f, 0.005f);

    // Move to the end of the stem
    glTranslatef(dirX * 0.1f, dirY * 0.1f, dirZ * 0.1f);

    // Find parent thickness
    auto parent_thickness = findParentThickness(branches, branch.parent_x, branch.parent_y, branch.parent_z);
    if (parent_thickness.first) {
        float offset = parent_thickness.second * 0.01f; // Adjust leaf position by parent thickness
        glTranslatef(offset * dirX, offset * dirY, offset * dirZ); // Apply offset
    }

    // Calculate the angle and axis for rotation
    glRotatef(-angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(90, 0, 1, 0);

    // Draw leaf
    glColor3f(0.1f, 1.0f, 0.1f);
    float leafLength = length * 0.8f;
    drawEllipse(leafLength, leafLength * 0.5f);

    glPopMatrix();
}

bool canTraceBackToRoot(const std::map<std::tuple<float, float, float>, std::tuple<float, float, float>>& parentChildMap, const std::tuple<float, float, float>& current, const std::tuple<float, float, float>& root) {
    auto it = current;
    while (it != root) {
        auto parentIt = parentChildMap.find(it);
        if (parentIt == parentChildMap.end()) {
            return false;
        }
        it = parentIt->second;
    }
    return true;
}


//check tracability to root. If not, not draw. This calc. is 54%CPU on jetson.
void drawTree(const std::vector<Branch>& branches) {
    if (branches.empty()) return;

    parentChildMap = createParentChildMap(branches);
    BranchMap = createBranchMap(branches);


    std::tuple<float, float, float> root(branches[0].parent_x, branches[0].parent_y, branches[0].parent_z);

    for (const auto& branch : branches) {
        std::tuple<float, float, float> child(branch.child_x, branch.child_y, branch.child_z);
        if (canTraceBackToRoot(parentChildMap, child, root)) {
            if (branch.thickness == -1 && drawLeaves) {  // Check drawLeaves before drawing leaves
                drawLeaf(branch, branches);
            } else if (branch.thickness != -1) {
                drawCylinder(branch.parent_x, branch.parent_y, branch.parent_z,
                             branch.child_x, branch.child_y, branch.child_z,
                             branch.thickness * 0.01);
            }
        }
    }
}

void drawBranchPoints(const std::set<std::tuple<float, float, float>>& branchPoints) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for branch points
    glPointSize(10.0f); // Set point size larger
    glBegin(GL_POINTS);
    for (const auto& point : branchPoints) {
        glVertex3f(std::get<0>(point), std::get<1>(point), std::get<2>(point));
    }
    glEnd();
}
void drawTerminalBranchPoints(const std::set<std::tuple<float, float, float>>& terminalBranchPoints) {
    glColor3f(0.0f, 0.0f, 1.0f); // Red color for branch points
    glPointSize(15.0f); // Set point size larger
    glBegin(GL_POINTS);
    for (const auto& point : terminalBranchPoints) {
        glVertex3f(std::get<0>(point), std::get<1>(point), std::get<2>(point));
    }
    glEnd();
}


void drawTerminalPoints(const std::set <std::tuple<float, float, float>>& terminalPoints) {
    glColor3f(1.0f, 1.0f, 1.0f); // Red color for branch points
    glPointSize(10.0f); // Set point size larger
    glBegin(GL_POINTS);
    for (const auto& point : terminalPoints) {
        glVertex3f(std::get<0>(point), std::get<1>(point), std::get<2>(point));
    }
    glEnd();
}