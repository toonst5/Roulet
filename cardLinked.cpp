#include "cardLinked.h"

int cardLinked::clear()
{
    card* temp = head, * temp2 = NULL;
    int ListLen = 0;

    if (head == NULL)
    {
        return 1;
    }

    while (temp != NULL)
    {
        temp = temp->next;
        ListLen++;
    }

    while(ListLen>=0)
    {
        deleteNode(ListLen);
        ListLen--;
    }

    return 0;
}

void cardLinked::insertNode()
{
    card* newNode = new card();
    int id=1;
    if (head == NULL) 
    {
        head = newNode;
        head->id = 0;
        return;
    }

    card* temp = head;
    while (temp->next != NULL) 
    {
        id++;
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->id = id;
}

int cardLinked::deleteNode(int nodeOffset)
{
    card* temp1 = head, * temp2 = NULL;
    int ListLen = 0;

    if (head == NULL) 
    {
        return 1;
    }

    while (temp1 != NULL) 
    {
        temp1 = temp1->next;
        ListLen++;
    }

    if (ListLen < nodeOffset) 
    {
        return 1;
    }


    temp1 = head;


    if (nodeOffset == 1) 
    {

        head = head->next;
        delete temp1;
        return 0;
    }

    while (nodeOffset-- > 1) 
    {

        temp2 = temp1;

        temp1 = temp1->next;
    }

    temp2->next = temp1->next;

    delete temp1;
    return 0;
}

card* cardLinked::get(int id)
{
    if (head == NULL)
    {
        return nullptr;
    }

    card* temp = head;
    while (temp->id != id)
    {
        if (temp->next == NULL)
        {
            return nullptr;
        }
        temp = temp->next;
    }
    return temp;
}
