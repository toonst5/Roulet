#include "cardLinked.h"

int cardLinked::clear()
{
    max = 0;
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
        ListLen--;
        deleteNode(ListLen);
    }

    return 0;
}

void cardLinked::appandNode()
{
    max++;
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

    while (temp1 != NULL)
    {
        for (int i = 0; i < 10; i++)
        {
            if (!temp1->conection[i].empty() && stoi(temp1->conection[i]) >= nodeOffset)
            {
                if (stoi(temp1->conection[i]) == nodeOffset)
                {
                    temp1->conection[i] = "";
                }
                else
                {
                    temp1->conection[i] = std::to_string(stoi(temp1->conection[i]) - 1);
                }
            }
        }
        if (temp1->id > nodeOffset)
        {
            temp1->id--;
        }
        temp1 = temp1->next;
    }

    temp1 = head;
    if (nodeOffset == 0) 
    {
        max--;
        head = head->next;
        delete temp1;
        return 0;
    }

    while (nodeOffset-- > 0) 
    {

        temp2 = temp1;

        temp1 = temp1->next;
    }

    temp2->next = temp1->next;

    delete temp1;
    max--;
    return 0;
}

int cardLinked::insertNode(int idAdd)
{
    idAdd;
    card* newNode = new card();
    card* temp1 = head;
    int ListLen = 0;

    if (head == NULL)
    {
        return 1;
    }
    max++;

    if (idAdd >= max)
    {
        appandNode();
        return 0;
    }

    while (temp1 != NULL)
    {
        for (int i = 0; i < 10; i++)
        {
            if (!temp1->conection[i].empty() && stoi(temp1->conection[i]) > idAdd)
            {
                temp1->conection[i] = std::to_string(stoi(temp1->conection[i]) + 1);
            }
        }
        if (temp1->id > idAdd)
        {
            temp1->id++;
        }
        if (temp1->id == idAdd)
        {
            newNode->next = temp1->next;
            temp1->next = newNode;
            newNode->id = idAdd+1;
            temp1 = newNode;
        }
        temp1 = temp1->next;
    }

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
