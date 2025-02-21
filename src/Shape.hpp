#pragma once
#include "raymath.h"
#include <iostream>


struct Shape {
    Vector2 *vertices;
    unsigned numberOfVertices;
    Vector2 center;
    float angle;
};

void VectorPrint(Vector2 a) {
    std::cout << a.x << ' ' << a.y << '\n';
}

void ShapeFirstOperation(Shape p) {
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
    {
        p.vertices[i].x -= 0.5f;
        p.vertices[i].y -= 0.5f;
    }
}
void ShapeScale(Shape p, float scalingFactor) {
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
    {
        p.vertices[i].x = (p.vertices[i].x - p.center.x) * scalingFactor + p.center.x;
        p.vertices[i].y = (p.vertices[i].y - p.center.y) * scalingFactor + p.center.y;
    }
}
void ShapeRotateBy(Shape &p, float angle) {
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
        p.vertices[i] = Vector2Add(p.center, Vector2Rotate(Vector2Subtract(p.vertices[i], p.center), angle * DEG2RAD));
    p.angle += angle;
}
void ShapeRotateTo(Shape &p, float angle) {
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
        p.vertices[i] = Vector2Add(p.center, Vector2Rotate(Vector2Subtract(p.vertices[i], p.center), (angle - p.angle) * DEG2RAD));
    p.angle = angle;
}
void ShapeMoveBy(Shape &p, Vector2 move) {
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
        p.vertices[i] = Vector2Add(p.vertices[i], move);
    p.center = Vector2Add(p.center, move);
}
void ShapeMoveTo(Shape &p, Vector2 newPos) {
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
        p.vertices[i] = Vector2Add(Vector2Subtract(p.vertices[i], p.center), newPos);
    p.center = newPos;
}
void ShapeTranslate(Shape &p, Vector2 move) {
    move = Vector2Rotate(move, p.angle * DEG2RAD);
    for (unsigned i = 0; i < p.numberOfVertices; ++i)
        p.vertices[i] = Vector2Add(p.vertices[i], move);
    p.center = Vector2Add(p.center, move);
}

void ShapeProjectOn(Shape p, Shape projection) {
    float minValue = p.vertices[0].x, maxValue = p.vertices[0].x;
    for (unsigned i = 1; i < p.numberOfVertices; ++i) {
        if (minValue > p.vertices[i].x)
            minValue = p.vertices[i].x;
        else if (maxValue < p.vertices[i].x)
            maxValue = p.vertices[i].x;
    };
    projection.vertices[0].x = minValue;
    projection.vertices[1].x = minValue;
    projection.vertices[5].x = minValue;

    projection.vertices[3].x = maxValue;
    projection.vertices[4].x = maxValue;
    projection.vertices[2].x = maxValue;
}