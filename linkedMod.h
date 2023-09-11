#pragma once

#include <string>
class Node {
public:
    std::string data;
    Node* next;

    // Default constructor
    Node()
    {
        data = "";
        next = NULL;
    }

    // Parameterised Constructor
    Node(std::string data)
    {
        this->data = data;
        this->next = NULL;
    }
};
class linkedMod
{
public:
    int clear();
    linkedMod() { head = nullptr; }
    void insertNode(std::string);
    int deleteNode(std::string);
    int hasTag(std::string);
private:
    Node* head;
};