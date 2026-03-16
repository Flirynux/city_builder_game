#include <memory>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include <bits/basic_string.h>

class trafficNode {
public:
    float trafficAmount = 0;
    float x = 0.0f;
    float y = 0.0f;
    // Nodes only care about where they can go next
    std::vector<std::weak_ptr<trafficNode>> neighbors;

    trafficNode(float x,float y) : x(x),y(y) {}
    ~trafficNode(){
        neighbors.clear();  
    }

    Color calculateColor(){
        unsigned char r = 255;
        unsigned char g =std::max((int)(255-((trafficAmount-50)*(255/100))),0);
        unsigned char b =std::max((int)(255-((trafficAmount)*(255/100))),0);
        unsigned char a = 255;
        return Color{r,g,b,a};
    }

    void Draw() {
        Color c = calculateColor();
        
        DrawCircle(x,y,10+trafficAmount/10,c);
        if(hover()){
            DrawText(ToString().c_str(),x+50,y,32,SKYBLUE);
        }
        
    }

    void DrawConnections(){
        Color c = calculateColor();
        for (auto it = neighbors.begin(); it != neighbors.end(); ) {
            
            // 1. Attempt to grab the object
            if (auto neighbor = it->lock()) {
                
                // 2. Success! Use 'neighbor' just like a normal pointer
                DrawLineEx({x,y},{neighbor->x,neighbor->y},5,c);
                
                ++it; // Move to next neighbor
            }else {
            // Node is dead! Remove this connection so we stop trying to draw it
            it = neighbors.erase(it); 
        }
        }
    }

    bool hover(){
        return CheckCollisionPointCircle(GetMousePosition(),{x,y},10+trafficAmount/10);
    }
    
    void handleTraffic(){
        if(!neighbors.empty() && trafficAmount > 0){
            for (auto it = neighbors.begin(); it != neighbors.end(); ) {
                
                // 1. Attempt to grab the object
                if (auto neighbor = it->lock()) {

                    // 2. Success! Use 'neighbor' just like a normal pointer
                    float transfer = trafficAmount/2;
                    neighbor->trafficAmount += transfer/neighbors.size();

                    ++it; // Move to next neighbor
                }
            }
        trafficAmount /= 2;
        }
    }

    std::string ToString(){
        return std::to_string(trafficAmount);
    }
};

class RoadManager {
private:
    // This is the ONLY place that "owns" the nodes
    std::vector<std::shared_ptr<trafficNode>> allNodes;

public:
    ~RoadManager(){
        allNodes.clear();
    }

    // Create a fresh node
    std::shared_ptr<trafficNode> createNode(float x=0,float y=0) {
        auto newNode = std::make_shared<trafficNode>(x,y);
        allNodes.push_back(newNode);
        return newNode;
    }

    // Helper to connect two existing nodes (One-Way)
    void connectOneWay(std::shared_ptr<trafficNode> from, std::shared_ptr<trafficNode> to) {
        if (from && to) {
            from->neighbors.push_back(to);
        }
    }

    // Helper to connect two existing nodes (Two-Way)
    void connectTwoWay(std::shared_ptr<trafficNode> a, std::shared_ptr<trafficNode> b) {
        connectOneWay(a, b);
        connectOneWay(b, a);
    }

    std::shared_ptr<trafficNode> getHoveredNode(){
        for (auto& node : allNodes) {
            if(node->hover()){
                return node;
            }
        }
        return nullptr;
    }

    // The "Magic" Deletion
    void removeNode(std::shared_ptr<trafficNode> nodeToDelete) {
        allNodes.erase(
            std::remove(allNodes.begin(), allNodes.end(), nodeToDelete),
            allNodes.end()
        );
        // Once removed from 'allNodes', the shared_ptr count drops.
        // If no one else (like a Car) is holding it, the node is destroyed.
        updateAll();
    }

    void updateAll() {
        for (auto& node : allNodes) {
            // Clean up dead neighbor references periodically
            auto& n = node->neighbors;
            n.erase(std::remove_if(n.begin(), n.end(), 
                    [](std::weak_ptr<trafficNode> w) { return w.expired(); }), 
                    n.end());
        }
    }

    void updateTraffic(){
        for (auto& node : allNodes) {
            // Clean up dead neighbor references periodically
            node->handleTraffic();
        }
    }

    void drawAll(){
        
        for (auto& node : allNodes) {
            // Clean up dead neighbor references periodically
            node->DrawConnections();
        }
        for (auto& node : allNodes) {
            // Clean up dead neighbor references periodically
            node->Draw();
        }
    }
};