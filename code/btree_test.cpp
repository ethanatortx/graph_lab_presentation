#include <iostream>
#include "btree.h"

using namespace std;

int main()
{
    btree<int> sbbst;
    cout<<"SelfBalancingBinarySearchTree Test\n";
    int val;
    char ch;
    /*  Perform tree operations  */
    do
    {
        cout<<"\nSelfBalancingBinarySearchTree Operations\n";
        cout<<"1. Insert "<<endl;
        cout<<"2. Count nodes"<<endl;
        cout<<"3. Search"<<endl;
        cout<<"4. Check empty"<<endl;
        cout<<"5. Make empty"<<endl;
        int choice;
        cout<<"Enter your Choice: ";
        cin>>choice;
        switch (choice)
        {
        case 1 :
            cout<<"Enter integer element to insert: ";
            cin>>val;
            sbbst.insert(val);
            break;
        case 2 :
            cout<<"Nodes = " <<sbbst.size()<<endl;
            break;
        case 3:
            cout<<"Enter integer element to search: ";
            cin>>val;
            if (sbbst.search(val).first)
                cout<<val<<" found in the tree"<<endl;
            else
                cout<<val<<" not found"<<endl;
            break;
        case 4 :
            cout<<"Empty status = ";
            if (sbbst.empty())
                cout<<"Tree is empty"<<endl;
            else
                cout<<"Tree is non - empty"<<endl;
            break;
        case 5 :
            cout<<"\nTree cleared\n";
            sbbst.clear();
            break;
        default :
            cout<<"Wrong Entry \n ";
            break;
        }
 
        /*  Display tree*/
        cout<<"\nPost order : ";
        sbbst.postorder();
        cout<<"\nPre order : ";
        sbbst.preorder();
        cout<<"\nIn order : ";
        sbbst.inorder();
        cout<<"\nDo you want to continue (Type y or n): ";
        cin>>ch;
    }
    while (ch == 'Y'|| ch == 'y');
    return 0;
}