#include "linkedMod.h"

int linkedMod::clear()
{
    Node* temp = head, * temp2 = head;

    if (head == NULL)
    {
        return 1;
    }
    if (head->next == NULL)
    {
        delete(head);
        head = NULL;
        return 0;
    }

    do
    {
        temp = head;
        temp2 = temp;
        while (temp->next != NULL)
        {
            temp2 = temp;
            temp = temp->next;
        }
        delete(temp);
        temp2->next=NULL;
    } while (temp2 != head);
    delete(head);
    head = NULL;
    return 0;
}

void linkedMod::insertNode(std::string data)
{
    Node* temp = head;

    while (temp != NULL && !data.empty())
    {
        if (temp->data == data)
        {
            return;
        }
        temp = temp->next;
    }

    Node* newNode = new Node(data);
    if (head == NULL)
    {
        head = newNode;
        return;
    }

    temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

int linkedMod::deleteNode(std::string tag)
{
    Node* temp = head;
    Node* prev = NULL;

    while (temp != NULL)
    {
        if (temp->data == tag)
        {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (temp == head)
    {
        head = temp->next;
        delete(temp);
    }
    else
    {
        prev->next = temp->next;
        delete(temp);
    }

    return 0;
}

int linkedMod::hasTag(std::string tag)
{
    if (tag.empty())
    {
        return 1;
    }
    Node* temp = head;
    bool negetive=false;
    if (tag.at(0) == *"-")
    {
        tag.erase(0, 1);
        negetive = true;
    }
    while (temp != NULL)
    {
        if (temp->data == tag)
        {
            if (negetive)
            {
                return 0;
            }
            return 1;
        }
        temp = temp->next;
    }
    if (negetive)
    {
        return 1;
    }
    return 0;
}

Node* linkedMod::get(int id)
{

    if (this->head == NULL)
    {
        return nullptr;
    }

    Node* temp = this->head;
    int idC = 0;
    while (idC != id)
    {
        if (temp->next == NULL)
        {
            return nullptr;
        }
        temp = temp->next;
        idC++;
    }
    return temp;
}
