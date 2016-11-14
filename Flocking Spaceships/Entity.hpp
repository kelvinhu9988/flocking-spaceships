//
//  Entity.hpp
//  Flocking Spaceships
//
//  Created by Kelvin Hu on 11/03/16.
//  Copyright © 2016 Kelvin Hu. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <math.h>
#include <vector>
#include "Vector2.hpp"
#include "LTexture.hpp"
#include "Matrix.h"
#include "Flock.hpp"

class Entity : public LTexture {
public:
    float speed;
    float maxSpeed;
    float rotation;
    float scale;
    float detectionDistance;
    float separationDistance;
    float detectionDistanceSquared;
    float separationDistanceSquared;
    float detectionAreaScale;
    float seperationAreaScale;
    bool inFlock;
    Vector2 velocity;
    Vector2 position;
    Matrix transform;
    SDL_Rect rectangle;
    SDL_Rect boundingRectangle;
    Flock* flock;
    
    Entity(Vector2 position, float speed, float maxSpeed,
           float rotationRadians, float scale,
           float detectionDistance, float separationDistance,
           int nframes, int screenWidth, int screenHeight) : LTexture(nframes)
    {
        this->position = position;
        this->speed = speed;
        this->maxSpeed = maxSpeed;
        this->rotation = rotationRadians;
        this->scale = scale;
        this->detectionDistance = detectionDistance;
        this->detectionDistanceSquared = detectionDistance * detectionDistance;
        this->separationDistance = separationDistance;
        this->separationDistanceSquared = separationDistance * separationDistance;
        this->velocity = Vector2(0, 0);
        this->boundingRectangle = { 0, 0, screenWidth, screenHeight };
        this->flock = NULL;
    }
    
    void setAreaScales()
    {
        this->detectionAreaScale = detectionDistance / (float) getAreaTextureWidth();
        this->seperationAreaScale = separationDistance / (float) getAreaTextureWidth();
    }
    
    
    void calculateMatrix()
    {
        transform.setPosition(position.x, position.y, 0);
        transform.Scale(scale, scale, scale);
        transform.Rotate(rotation);
        transform.Translate(position.x, position.y, 0);
    }
    
    
    void calculateBoundingRectangle()
    {
        if (getEntityTexture() != NULL) {
            SDL_Rect rectEntity = { 0, 0, getEntityTextureWidth(), getEntityTextureHeight() };
            int rectEntityLeft = rectEntity.x;
            int rectEntityRight = rectEntity.x + rectEntity.w;
            int rectEntityTop = rectEntity.y;
            int rectEntityBottom = rectEntity.y + rectEntity.h;
            //
            //            Vector2 leftTop = Vector2.Transform(Vector(rectEntityLeft, rectEntityTop), transform);
            //            Vector2 rightTop = Vector2.Transform(Vector(rectEntityRight, rectEntityTop), transform);
            //            Vector2 leftBottom = Vector2.Transform(Vector2(rectEntityLeft, rectEntityBottom), transform);
            //            Vector2 rightBottom = Vector2.Transform(Vector2(rectEntityRight, rectEntityBottom), transform);
            //
            //            Vector2 min = Vector2.Min(Vector2.Min(leftTop, rightTop),
            //                                      Vector2.Min(leftBottom, rightBottom));
            //            Vector2 max = Vector2.Max(Vector2.Max(leftTop, rightTop),
            //                                      Vector2.Max(leftBottom, rightBottom));
            //
            //            this->rectangle = { (int) min.x, (int) min.y, (int) (max.x - min.x), (int) (max.y - min.y) }
            
            
            
            
        }
    }
    
    
    void update(std::chrono::duration<float> elapsedGameTimeSeconds)
    {
        if (this->velocity != Vector2(0, 0))
        {
            velocity = velocity.Normalize();
            velocity = velocity * speed;
            
            rotation = (float)atan2((double)velocity.y, (double)velocity.x);
            position += velocity * elapsedGameTimeSeconds.count();
            
            
            if (rectangle.x + rectangle.w < boundingRectangle.x
                || rectangle.x > boundingRectangle.x + boundingRectangle.w
                || rectangle.y + rectangle.h < boundingRectangle.y
                || rectangle.y > boundingRectangle.y + boundingRectangle.h)
            {
                moveInsideBoundingRectangle();
            }
            
            calculateMatrix();
            calculateBoundingRectangle();
        }
    }
    
    void moveInsideBoundingRectangle()
    {
        if (rectangle.x + rectangle.w < boundingRectangle.x)
            position.x = boundingRectangle.x + boundingRectangle.w;
        else if (rectangle.x > boundingRectangle.x + boundingRectangle.w)
            position.x = boundingRectangle.x;
        
        if (rectangle.y + rectangle.h < boundingRectangle.y)
            position.y = boundingRectangle.y + boundingRectangle.h;
        else if (rectangle.y > boundingRectangle.y + boundingRectangle.h)
            position.y = boundingRectangle.y;
    }
    
    
    void refreshNeighborList(std::vector<Entity> allEntities)
    {
        findNewNeighbors(allEntities);
    }
    
    
    void findNewNeighbors(std::vector<Entity> allEntities)
    {
        for (Entity e : allEntities)
        {
            if (&e != this && !e.inFlock && position.DistanceSquared(e.position) < detectionDistanceSquared)
            {
                
                
                if (std::find(flock.entities.begin(), flock.entities.end(), e) != flock.entities.end())
                {
                    flock.addMember(e);
                    e.flock = flock;
                    e.inFlock = true;
                }
                
            }
        }
    }
    
    void removeFromFlock()
    {
        flock = NULL;
        inFlock = false;
    }
};


#endif /* Entity2_hpp */