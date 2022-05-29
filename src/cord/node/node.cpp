#include "node.hpp"

using namespace cord;

Node::Node()
    : children(), parent(nullptr) {}

Node::~Node() {
    for (auto child : children)
        delete child;
}

void Node::visitPreRender(Renderer* renderer) {
    preRender(renderer);

    for (auto child : children)
        child->visitPreRender(renderer);
}

void Node::visitRender(Renderer* renderer) {
    render(renderer);

    for (auto child : children)
        child->visitRender(renderer);
}

void Node::visitUpdate(float delta) {
    update(delta);

    for (auto child : children)
        child->visitUpdate(delta);
}

void Node::preRender(Renderer* renderer) {}

void Node::render(Renderer* renderer) {}

void Node::update(float delta) {}

NodeTopType Node::getTopType() {
    return NODETT_NODE;
}

void Node::add(Node* node) {
    if (node->parent != nullptr)
        node->parent->remove(node);

    children.push_back(node);
    node->parent = this;
}

void Node::remove(Node* node) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == node) {
            children.erase(children.begin() + i);
            node->parent = nullptr;
            return;
        }
    }
}

unsigned int Node::count() {
    return children.size();
}

Node* Node::get(unsigned int index) {
    return children[index];
}

Node* Node::getParent() {
    return parent;
}
