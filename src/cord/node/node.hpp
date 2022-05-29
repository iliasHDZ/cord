#pragma once

#include "../renderer.hpp"
#include <vector>

namespace cord {
    enum NodeTopType {
        NODETT_NODE,
        NODETT_NODE2D,
        NODETT_NODE3D
    };

    class Node {
    public:
        Node();
        virtual ~Node();

        virtual void visitPreRender(Renderer* renderer);

        virtual void visitRender(Renderer* renderer);
        
        virtual void visitUpdate(float delta);

        virtual void preRender(Renderer* renderer);

        virtual void render(Renderer* renderer);

        virtual void update(float delta);

        virtual NodeTopType getTopType();

        void add(Node* node);

        void remove(Node* node);

        unsigned int count();

        Node* get(unsigned int index);

        Node* getParent();

    protected:
        std::vector<Node*> children;
        Node* parent;
    };
}